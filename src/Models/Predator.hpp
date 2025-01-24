#pragma once

#include "Creature.hpp"


namespace Model{
    class Fish;
class Predator: public Creature{
public:
    Predator();
    void determineMotion(Model::Cluster<Model::Fish>&, Model::Cluster<Model::Predator>&) override;
friend class Fish;
};
}