#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <array>

namespace View {

class Animation {
protected:
    size_t current;
    
public:
    std::array<u_char, 4>* debugColor;
    double Srotation;
    std::vector<SDL_Rect>* spriteClips;
    SDL_Texture* spriteSheet;

    Animation(SDL_Texture* spriteSheet, std::vector<SDL_Rect>* spriteClips) :
        current(0), debugColor(nullptr),Srotation(0), spriteClips(spriteClips), spriteSheet(spriteSheet) {}

    bool hasSprites() const {
        return spriteClips != nullptr;
    }

    SDL_Texture* getSpriteSheet() const {
        return spriteSheet;
    }

    void incrementCounter() {
        if (spriteClips->size())
            current = (current + 1) % spriteClips->size();
    }

    SDL_Rect& nextSprite() const {
    return spriteClips->at(current);
}

};


}