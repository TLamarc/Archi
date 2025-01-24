#include "aquaEngine.hpp"

void Controller::aquaEngine::preComputeEntities(){
    for(Model::Creature& entity: FishCluster){
        render.DrawEntity(entity);
        entity.determineMotion(FishCluster, PredatorCluster);
    }
    for(Model::Predator& entity: PredatorCluster){
        render.DrawEntity(entity);
        entity.determineMotion(FishCluster, PredatorCluster);
    }
}

void Controller::aquaEngine::Animate(){
    for(Model::Fish& entity: FishCluster){
        entity.MoveOneFrame();
    }
    for(Model::Predator& entity: PredatorCluster){
        entity.MoveOneFrame();
    }
} 


Controller::aquaEngine::aquaEngine(size_t initFish, size_t initPred): render(View::Render()){
    // aquaPopulation(initFish, initPred);
    for(;initFish;initFish--) FishCluster.push_back(Model::Fish());
    for(;initPred;initPred--) PredatorCluster.push_back(Model::Predator());
    // populate(initFish, initPred);
}

// void Controller::aquaEngine::populate(size_t initFish, size_t initPred):{
//     render.populate(initFish, initPred);
// }


void Controller::aquaEngine::start(){
    bool quit = false;
    while (!quit) 
    {
        render.processEvents();
        quit = !render.stillRunning();
        
        // Effacer l'écran
        SDL_RenderClear(render.renderer);
        if(!View::Env::low_debugmode && render.backgroundTexture) 
            SDL_RenderCopy(render.renderer, render.backgroundTexture, NULL, NULL);
        else SDL_SetRenderDrawColor(render.renderer, 0, 0, 0, 255);

        // render.DrawFish();
        // render.DrawPredator();
        preComputeEntities();


        Animate();
        
        SDL_RenderPresent(render.renderer);
        
    }
}