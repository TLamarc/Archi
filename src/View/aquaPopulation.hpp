#pragma once

#include "../Models/Creature.hpp"
#include "../Models/Fish.hpp"
#include "../Models/Predator.hpp"
#include "../Models/Cluster.hpp"

namespace View{
    class aquaPopulation{
    protected:
        Model::Cluster<Model::Fish> FishCluster ;
        Model::Cluster<Model::Predator> PredatorCluster;

        aquaPopulation(size_t initFish, size_t initPred){
            populate(initFish,initPred);
        }
        aquaPopulation(){}
    public:
        void populate(size_t initFish, size_t initPred = 0){
            for(;initFish;initFish--) FishCluster.push_back(Model::Fish());
            for(;initPred;initPred--) PredatorCluster.push_back(Model::Predator());
        }
    };
}