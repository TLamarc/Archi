#pragma once
#include <random>
namespace Ext{

    inline double ranGen(){
        static std::mt19937 gen((1<<9) - 1);
        static std::uniform_real_distribution<> distrib(0.0, 1.0);
        return distrib(gen);
    }
    
    inline double ln(double x)
    {
        return log(x)/log(exp(1));
    }
}