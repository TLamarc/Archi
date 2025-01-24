#include "Creature.hpp"
#include "Cluster.hpp"
#include "../View/CurvedGeometry.hpp"
#include "Fish.hpp"
#include "Predator.hpp"
#include "../Ext/Helper.hpp"

Model::Creature::Creature(SDL_Texture* spriteSheet , std::vector<SDL_Rect>* spriteClips): 
    animation(spriteSheet, spriteClips), velocitY(0), rotation(0){
    core.pos = {
        Ext::ranGen() * Env::WINDOW[Coords::pX],
        Ext::ranGen() * Env::WINDOW[Coords::pY]
    };
}

double Model::Creature::HitBox::Center(size_t direction) const{
    switch(direction){
        case Coords::pX:
        case Coords::pY:
            return pos[direction] + dim[direction]/2;
            break;
        default:
            return -1.0;
            break;
    }
}


Model::Creature::operator SDL_Rect() const{
    return SDL_Rect{(int) std::round(core.pos[Coords::pX]), (int) std::round(core.pos[Coords::pY]), 
                    (int) std::round(core.dim[Coords::pX]), (int) std::round(core.dim[Coords::pY])};
}

double Model::Creature::Standard_Evaluation(double k, double l) 
{
    return k * l;
}

Model::CoupleCoordinates Model::Creature::Center_Gravity(const Cluster<Fish>& currentCluster, double (*Evaluate)(double, double)){
    CoupleCoordinates returnedCouple{};
    
    double totalDistance = 0;
    CoupleCoordinates fictif_Axis{};

    
    for (const Fish& currentEntity : currentCluster) 
    {
        double dist_native =
            View::Geometry::dotProduct( CoupleCoordinates{core.Center(Coords::pX), core.Center(Coords::pY)},
                                        CoupleCoordinates{currentEntity.core.Center(Coords::pX), currentEntity.core.Center(Coords::pY)});
       
        fictif_Axis[Coords::pX] = View::Geometry::getFictifCoordinates(core.Center(Coords::pX), currentEntity.core.Center(Coords::pX), Env::WINDOW[Coords::pX]);
        fictif_Axis[Coords::pY] = View::Geometry::getFictifCoordinates(core.Center(Coords::pY), currentEntity.core.Center(Coords::pY), Env::WINDOW[Coords::pY]);
        
        
        returnedCouple[Coords::pX] += Evaluate(fictif_Axis[Coords::pX] , dist_native);
        returnedCouple[Coords::pY] += Evaluate(fictif_Axis[Coords::pY] , dist_native);
        totalDistance += Evaluate(1,dist_native); // Attention not always 1, give it the neutral element of the group you are operating on, in my case I operated in (R,*) group
    }
    if(totalDistance)
    {
        returnedCouple[Coords::pX] /= totalDistance;
        returnedCouple[Coords::pY] /= totalDistance;
    }

    core.d_gravityCenter = returnedCouple;

    return returnedCouple;
}

bool Model::Creature::adjustOutOfBoundPos(){
    bool needAdjust = false;
    if ( (-1 * core.dim[Coords::pX] > core.pos[Coords::pX] ) || 
            (core.pos[Coords::pX] > Env::WINDOW[Coords::pX] + 1 * core.dim[Coords::pX]  ))
    {
        if(core.pos[Coords::pX]< 0) core.pos[Coords::pX] += Env::WINDOW[Coords::pX] ;
        else if(core.pos[Coords::pX] > Env::WINDOW[Coords::pX] )
            core.pos[Coords::pX] -= Env::WINDOW[Coords::pX] ;

        
    }
    if((-1 * core.dim[Coords::pY]  > core.pos[Coords::pY]) ||
            (core.pos[Coords::pY] > Env::WINDOW[Coords::pY] + 1 * core.dim[Coords::pY] ))
    {
        if(core.pos[Coords::pY]< 0) core.pos[Coords::pY] = Env::WINDOW[Coords::pY] - fabs(core.pos[Coords::pY]);
        else if(core.pos[Coords::pY] > Env::WINDOW[Coords::pY])
            core.pos[Coords::pY] -= Env::WINDOW[Coords::pY] ;
    }

    return needAdjust;
}

void Model::Creature::MoveOneFrame(){
    
    core.pos[Coords::pX] += velocity[Coords::pX];
    core.pos[Coords::pY] += velocity[Coords::pY];

    adjustOutOfBoundPos();
}




double Model::Creature::Critique_Probability(const Predator& predator, double p, double l,double s,double B,double *EscapeAngle)
{
    //weird, need to be modifed later.


    double x = View::Geometry::Folded_Space_Distance(  CoupleCoordinates{core.Center(Coords::pX), core.Center(Coords::pY)} ,
                                                    CoupleCoordinates{predator.core.Center(Coords::pX), predator.core.Center(Coords::pY)});
    
    //eaten checking : 
    // if(x < 3* Env::CRITIQUERADIUS/4) Eaten(creature);
    double proba = 0;
    
    double a = Env::CRITIQUERADIUS;
    double b = Env::SAFERADIUS;

    if(x<a) proba = 2.0 - pow(2.0 - p, pow(x/a,s)); // F function
    else 
    {
        //double B = -1;
        s = Ext::ln(p/l)/Ext::ln((b-B)/(a-B));
        proba= sqrt(p*l) * pow((a-B)*(b-B)/pow(x-B,2) ,s/2); //G function
    }

    if(EscapeAngle)
    {
        CoupleCoordinates fictifPredator = {
            View::Geometry::getFictifCoordinates(  core.Center(Coords::pX),
                                                    predator.core.Center(Coords::pX), Env::WINDOW[Coords::pX]),
            View::Geometry::getFictifCoordinates(  core.Center(Coords::pY),
                                                    predator.core.Center(Coords::pY), Env::WINDOW[Coords::pY])
        };                                        
        double detailed = std::atan2(  core.Center(Coords::pY) - fictifPredator[Coords::pY],
                                core.Center(Coords::pX) - fictifPredator[Coords::pX]);
        *EscapeAngle =  M_PI + detailed;

    }
    
    return proba;
}


void Model::Creature::RenderEntity(SDL_Renderer* renderer , SDL_Rect *relocation) const{
    SDL_Rect EntityBox = (SDL_Rect) *this;
    if(!relocation) relocation = &EntityBox;


    if(!Env::low_debugmode && animation.hasSprites()){
        SDL_RenderCopyEx(renderer, animation.getSpriteSheet(), 
                &animation.nextSprite(), 
                relocation, 
                animation.Srotation, NULL, SDL_FLIP_NONE);
        return;
    }

    SDL_RenderFillRect(renderer, relocation);
}

Model::CoupleCoordinates Model::Creature::fetch_gCenter() const{
    return core.d_gravityCenter;
}

double Model::Creature::oCenter(size_t dim) const{
    return core.Center(dim);
}