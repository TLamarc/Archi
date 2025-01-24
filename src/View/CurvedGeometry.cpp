#include "CurvedGeometry.hpp"
#include "Environment.hpp"
#include <cmath>

double View::Geometry::getFictifCoordinates(const double& pivot,const double& target,int axis_length){
    double fictif_target = target;
    if(std::abs(target - pivot) > abs(target - (axis_length + pivot)) )
    {
        fictif_target = -axis_length + target;
    }
    else if(std::abs(target - pivot) > std::abs((axis_length + target) - pivot) )
    {
        fictif_target = axis_length + target;
    }

    return fictif_target;
}

double View::Geometry::getFictifCoordinates(CoupleCoordinates vec,int axis_length){
    return getFictifCoordinates(vec[Env::Coords::pX], vec[Env::Coords::pY], axis_length);
}

double View::Geometry::dotProduct(const CoupleCoordinates& vec1, const CoupleCoordinates& vec2){
    using Coords = Env::Coords;
    double fictif_targetX = getFictifCoordinates(vec1[Coords::pX], vec2[Coords::pX], Env::WINDOW[Coords::pX]);
    double fictif_targetY = getFictifCoordinates(vec1[Coords::pY], vec2[Coords::pY], Env::WINDOW[Coords::pY]);
    
    return pow(vec1[Coords::pX] - fictif_targetX, 2) 
            + pow(vec1[Coords::pY] - fictif_targetY, 2);
}


double View::Geometry::Folded_Space_Distance(const CoupleCoordinates& vec1, const CoupleCoordinates& vec2){
    return sqrt(dotProduct(vec1, vec2));
}