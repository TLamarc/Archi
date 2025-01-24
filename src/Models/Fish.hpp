#pragma once

#include <vector>
#include "Cluster.hpp"
namespace Model{
class Fish: public Creature{
// private:
//     class Animation: public Creature::Animation{
//     public:
//         Animation();
//         std::vector<SDL_Rect>* AssignSpirts() override;
//         SDL_Rect& nextSprite() override;
//     };

// private:
//     Animation animation;

public:
    Fish();
    void determineMotion(Model::Cluster<Model::Fish>&, Model::Cluster<Model::Predator>&) override;


friend class Predator;
};
}