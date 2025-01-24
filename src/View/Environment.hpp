#pragma once

#include<vector>
#include<random>
#include<string>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


namespace View
{

struct Env{
    enum Coords{
        pX = 0,
        pY = 1
    };
    // env variables :
    inline static std::array<int, 2> WINDOW = {1200, 700};

    inline static int SAFERADIUS = 150;
    inline static int CRITIQUERADIUS = 75;


    inline static int FishHeadBoxW = 25;
    inline static int FishHeadBoxH = 15;

    inline static int PredatorHeadBoxW = 84;
    inline static int PredatorHeadBoxH = 40;

    
    inline static int INITIAL_FISH_VELOCITY = 3;
    inline static int INITIAL_PREDATOR_VELOCITY = 2;

    inline static bool low_debugmode = false;
    inline static bool rotational_debugmode = false;
    inline static const std::string where_fish = "assets/animales2.png";
    inline static const std::string where_background = "assets/cx3t_mc71_230315.jpg";
    inline static const std::string where_predateur = "assets/Whale.png";


    inline static std::vector<std::vector<SDL_Rect>> FishSprits;
    inline static std::vector<std::vector<SDL_Rect>> PredatorSprits;
    inline static SDL_Texture *spriteSheet_Fish = NULL;
    inline static SDL_Texture *spriteSheet_Predator = NULL;
    inline static void initSprits();

    inline static std::array<u_char, 4> d_FishSquaredColor = {255, 0, 255, 255};
    inline static std::array<u_char, 4> d_PredatorSquaredColor = {255, 0, 0, 255};

};

}
