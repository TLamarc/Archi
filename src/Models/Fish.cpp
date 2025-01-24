#include "Fish.hpp"
#include "Predator.hpp"
#include "../View/CurvedGeometry.hpp"
#include "../Ext/Helper.hpp"

static std::vector<SDL_Rect>* AssignSpirts(){
    size_t size = View::Env::FishSprits.size();
    if(!size) printf("no spirts");
    return (size)? &View::Env::FishSprits[Ext::ranGen() * size]: nullptr;
}

//Fish definition
Model::Fish::Fish():
    Creature(Env::spriteSheet_Fish, AssignSpirts()) {
    animation.debugColor = &Env::d_FishSquaredColor;
	core.dim ={
        (double) Env::FishHeadBoxW, (double) Env::FishHeadBoxH
    };
}

[[maybe_unused]] static double Modified_Selection(double k, double l)
{
    double opt = sqrt(l)/40;
    return k * opt;
}

[[maybe_unused]] static double Customized_Selection(double k, double l)
{

    double opt = exp(-sqrt(l)/30);
    return k*opt;
}

void Model::Fish::determineMotion(Model::Cluster<Model::Fish>& FishCluster, Model::Cluster<Model::Predator>& PredatorCluster){

    CoupleCoordinates ClusterPull = Center_Gravity(FishCluster, Modified_Selection);

    double dist = sqrt( pow(core.pos[Coords::pX] - ClusterPull[Coords::pX], 2) + 
                        pow(core.pos[Coords::pY] - ClusterPull[Coords::pY], 2));
    

    // //Regular
    double disk_radius = 50;
    double x = dist/disk_radius;
    double n = 0.001;
    double A = 1.001;
    double Collapsing = pow(A, - x);
    double AntiCollapsing = 1/pow(x+ pow(2,1/n) ,n);


    //Alternative
    // disk_radius = 40;
    // double x = dist/disk_radius;
    // // double n = 90;
    // // double p = 0.97;
    // // double s = 0.6;
    // // double l = 50;
    
    // double p = 0.6;
    // double s = 1.5;
    // double Collapsing = (x>=1.0/2)? pow(p, pow(fabs(2*x - 1),s) ) : 1;
    // // double Collapsing = pow(p, pow(fabs(2*x - 1),s) );
    // double l = 0.8;
    // double n = 20;
    // double AntiCollapsing = 1.0/2 + 1/pow(x*l+ pow(2,1/n) ,n);

    double forwardAngle = atan2(core.Center(Coords::pY) - ClusterPull[Coords::pY], 
                            core.Center(Coords::pX) - ClusterPull[Coords::pX]);
    
    double Revolving_theta = AntiCollapsing * Collapsing;
    
    double target_angle = forwardAngle + Revolving_theta* M_PI; // to modify the + or - M_PI in function of the
    
    
    rotation = 0;
    double minReversedPeaceTime = 1.0;
    for(const Predator& predator: PredatorCluster ){
        double EscapeAngle;
        double peace_time = Critique_Probability(predator,0.85,0.15,7,-1,&EscapeAngle);
        minReversedPeaceTime = std::min(minReversedPeaceTime, 1-peace_time);
        rotation += EscapeAngle * peace_time;
    }
    rotation += minReversedPeaceTime * target_angle;

    animation.Srotation = rotation * 180/M_PI + 90;

    const double super_cts = Env::INITIAL_FISH_VELOCITY / M_PI_2;
    x = super_cts * std::atan(x);

    velocity[Coords::pX] = - x * std::cos(rotation);
    velocity[Coords::pY] = - x * std::sin(rotation);
}
