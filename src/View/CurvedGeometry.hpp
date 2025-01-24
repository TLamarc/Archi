#pragma once
#include <array>
using Coords = std::array<double, 2>;

namespace View{
class Geometry{
public:
    using CoupleCoordinates = std::array<double, 2>;
    static double getFictifCoordinates(const double& pivot,const double& target,int axis_length);
    static double getFictifCoordinates(CoupleCoordinates vec,int axis_length);
    static double dotProduct(const CoupleCoordinates&, const CoupleCoordinates&);
    static double Folded_Space_Distance(const CoupleCoordinates&, const CoupleCoordinates&);

};
}