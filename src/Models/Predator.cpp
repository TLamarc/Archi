#include "Predator.hpp"
#include "../Ext/Helper.hpp"
#include "Cluster.hpp"

// Animations Definitions
static std::vector<SDL_Rect>* AssignSpirts(){
    size_t size = View::Env::PredatorSprits.size();
    return (size)? &View::Env::PredatorSprits[Ext::ranGen() * size]: nullptr;
}

Model::Predator::Predator(): Creature(Env::spriteSheet_Predator, AssignSpirts()) {
    animation.debugColor = &Env::d_PredatorSquaredColor;
	core.dim ={
        (double) Env::PredatorHeadBoxW, (double) Env::PredatorHeadBoxH
    };
}

static double Customized_Selection(double k, double l)
{
    double opt = exp(-sqrt(l)/80);
    return k*opt;
}

void Model::Predator::determineMotion(Model::Cluster<Model::Fish>& FishCluster, Model::Cluster<Model::Predator>& PredatorCluster){

    CoupleCoordinates ClusterPull = Center_Gravity(FishCluster, Customized_Selection);

    double dist = sqrt(pow(core.pos[Coords::pX] - ClusterPull[Coords::pX], 2) 
                    + pow(core.pos[Coords::pY] - ClusterPull[Coords::pY], 2));
    //

    // double forwardAngle =  atan2(creature->core.pos[pY] + creature->core.dim[pY]/2 - CenterY, creature->core.pos[pX] + creature->core.dim[pX]/2 - CenterX);
    double forwardAngle = atan2(core.Center(Coords::pY) - ClusterPull[Coords::pY], 
                            core.Center(Coords::pX) - ClusterPull[Coords::pX]);
    double Diverging_probability = 0;
    

    double target_angle = 0;
    double minReversedDivergeProb = 1;
    for(const Predator& predator: PredatorCluster){
        if(this != &predator){
            double escapeAngle;
            Diverging_probability = Critique_Probability(predator,0.85,0.15,7,-1, &escapeAngle);
            minReversedDivergeProb = std::min(minReversedDivergeProb, 1-Diverging_probability);
            target_angle += Diverging_probability * escapeAngle;
        }
    }
    target_angle += minReversedDivergeProb*forwardAngle;

    
    rotation = target_angle ;
    
    animation.Srotation = target_angle * 180/M_PI -180;
    //
  
    double tex = 1.0/M_PI_2 * atan(dist/30.0);

    velocity[Coords::pX] = - tex * cos(rotation);
    velocity[Coords::pY] = - tex * sin(rotation);
}