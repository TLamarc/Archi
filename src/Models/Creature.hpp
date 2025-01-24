#pragma once
// This file serves as a header for Creatures
#include <array>

#include "../View/Environment.hpp"
// #include "Cluster.hpp"
#define GRAPHIC_MODE_FOR_THE_ENGINE // test_GRAPHIC_MODE_FOR_THE_ENGINE

#ifdef GRAPHIC_MODE_FOR_THE_ENGINE
#include "../View/Animation.hpp"
#endif

namespace Model{
    
using Coords = View::Env::Coords;
using CoupleCoordinates = std::array<double, 2>;

template <typename T> class Cluster;

class Fish;
class Predator;

//Abstract class
class Creature{
protected:
    using Env = View::Env;
public:
    struct HitBox{
        CoupleCoordinates pos;
        CoupleCoordinates dim;
        CoupleCoordinates d_gravityCenter;

        double Center(size_t direction) const;
    };

    using Animation = View::Animation;
    Animation animation;
protected:
    HitBox core;
    CoupleCoordinates velocity;
    double velocitY;
    double rotation;

public:
    Creature(SDL_Texture* spriteSheet , std::vector<SDL_Rect>* spriteClips);
    virtual ~Creature(){}
public:
    operator SDL_Rect() const;
    CoupleCoordinates fetch_gCenter() const;
    double oCenter(size_t dim) const;

private:
    static double Standard_Evaluation(double k, double l);
    bool adjustOutOfBoundPos(); // returns 1 or 0 if an out of bound has been detected
protected:
    double Critique_Probability(const Model::Predator& predator, double p, double l,double s,double B, double *EscapeAngle = nullptr);
//Virtual methods
public:
    virtual CoupleCoordinates Center_Gravity(const Cluster<Fish>&, double (*Evaluate)(double, double) = Standard_Evaluation);
    virtual void determineMotion(Model::Cluster<Model::Fish>&, Model::Cluster<Model::Predator>&) = 0;
    virtual void MoveOneFrame();
    // virtual void animateCreature() = 0;
    void RenderEntity(SDL_Renderer* renderer ,SDL_Rect *relocation = nullptr) const;



};

}