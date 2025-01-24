#pragma once
#include "aquaPopulation.hpp"

namespace View{
class Render { //public aquaPopulation
    using Coords = Env::Coords;
private:
    inline static size_t grandCounter = 0;
    SDL_Window* window;
    SDL_Event* event;
    bool keepRunning;
public:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;

private:
    static void SDL_initer();
    void LoadSprits();
public:
    Render();
    ~Render();
    void processEvents();
    bool stillRunning() const;

public:
    void DrawEntity(const Model::Creature& entity) const;
public:
    // void DrawFish();
    // void DrawPredator();
    // void DrawEntity(Model::Creature& entity);
};
}