#pragma once
#include "../View/Render.hpp"
#include "../View/aquaPopulation.hpp"


namespace Controller{
class aquaEngine{//}: public View::aquaPopulation{
public:
    Model::Cluster<Model::Fish> FishCluster ;
    Model::Cluster<Model::Predator> PredatorCluster;

private:
    View::Render render;
public:
    aquaEngine(size_t initFish, size_t initPred);
    aquaEngine(){}
    // void populate(size_t initFish, size_t initPred);
    void start();

    void Animate();
    void preComputeEntities();
};
}