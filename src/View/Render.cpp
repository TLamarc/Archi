#include "Render.hpp"
#include <stdexcept> // For std::runtime_error
#include <iostream>  // For std::cerr


static void throwFancyError(std::string openningString){
    throw std::runtime_error(openningString + SDL_GetError() + "\n");
}

void View::Render::SDL_initer(){
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throwFancyError("Erreur lors de l'initialisation de SDL :");
    }
}

View::Render::Render(): window(nullptr),event(nullptr),keepRunning(true) { 
    if(grandCounter++ > 0) SDL_initer();

    // Créer la fenêtre
    window = SDL_CreateWindow("Fish Engineoo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Env::WINDOW[Coords::pX], Env::WINDOW[Coords::pY], SDL_WINDOW_SHOWN);
    if (window == NULL) {
        // SDL_Quit();
        throwFancyError("Erreur lors de la création de la fenêtre : ");
    }

    // Créer le renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        window = nullptr;
        // SDL_Quit();
        throwFancyError("Erreur lors de la création du renderer : ");
    }


    LoadSprits();

    //background loading
    {
        SDL_Surface* surface_background = IMG_Load(Env::where_background.c_str());
        backgroundTexture = nullptr;
        if(surface_background){
            backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface_background);
            SDL_FreeSurface(surface_background);
        }
    }

    event = new SDL_Event();
}

void View::Render::LoadSprits(){

    // load sprites for fish
    {
    SDL_Surface* spriteSurface = IMG_Load((Env::where_fish).c_str());
    if(spriteSurface){
        Env::spriteSheet_Fish = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        if(Env::spriteSheet_Fish){
            auto getClipsSequence = [](int fish_type) -> std::vector<SDL_Rect>{
                const int FSPRITE_WIDTH = 32;
                const int FSPRITE_HEIGHT = 32;
                std::vector<SDL_Rect> current;
                for (int i = 0; i < 3; ++i) 
                {
                    // current.push_back
                    SDL_Rect thisClip = {
                        (3 * (fish_type %4 ) + i) * FSPRITE_WIDTH,
                        (4 * (fish_type /4)) * FSPRITE_HEIGHT,
                        FSPRITE_WIDTH,
                        FSPRITE_HEIGHT
                    };
                    current.emplace_back(thisClip);
                }
                return current;
            };

            Env::FishSprits.emplace_back(getClipsSequence(0));
            Env::FishSprits.emplace_back(getClipsSequence(1));
            Env::FishSprits.emplace_back(getClipsSequence(6));
            

        }

        SDL_FreeSurface(spriteSurface);
    }
    }

    // for predator
    {
    SDL_Surface* spriteSurface = IMG_Load(Env::where_predateur.c_str());

    if(spriteSurface){
        Env::spriteSheet_Predator = SDL_CreateTextureFromSurface(renderer, spriteSurface);

        if(Env::spriteSheet_Predator){
            const int PSPRITE_WIDTH = 128;
            const int PSPRITE_HEIGHT = 61;
            std::vector<SDL_Rect> current;
            current.reserve(10);
            for (int i = 0; i < 10; ++i) 
            {
                current.emplace_back(SDL_Rect{
                    i * PSPRITE_WIDTH,
                    0,
                    PSPRITE_WIDTH,
                    PSPRITE_HEIGHT
                });
            }
            Env::PredatorSprits.emplace_back(current);

        }

        SDL_FreeSurface(spriteSurface);
    }
    }
}


static void drawRotatedRect(SDL_Renderer* renderer, double x, double y, double width, double height, double angle) {
    double radians = angle * (M_PI / 180.0);
    double centerX = x + width / 2;
    double centerY = y + height / 2;

    // Calculate the four corner points of the rotated rectangle
    double x1 = centerX + (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y1 = centerY + (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x2 = centerX - (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y2 = centerY - (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x3 = centerX - (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y3 = centerY - (width / 2) * sin(radians) - (height / 2) * cos(radians);

    double x4 = centerX + (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y4 = centerY + (width / 2) * sin(radians) - (height / 2) * cos(radians);

    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
    SDL_RenderDrawLine(renderer, (int)x2, (int)y2, (int)x3, (int)y3);
    SDL_RenderDrawLine(renderer, (int)x3, (int)y3, (int)x4, (int)y4);
    SDL_RenderDrawLine(renderer, (int)x4, (int)y4, (int)x1, (int)y1);
}

static int* getMaxDims(double width, double height, double angle) {
    double radians = -angle * (M_PI / 180.0);
    // Calculate the four corner points of the rotated rectangle
    double x1 = + (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y1 = + (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x2 = - (width / 2) * cos(radians) - (height / 2) * sin(radians);
    double y2 = - (width / 2) * sin(radians) + (height / 2) * cos(radians);

    double x3 = - (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y3 = - (width / 2) * sin(radians) - (height / 2) * cos(radians);

    double x4 = + (width / 2) * cos(radians) + (height / 2) * sin(radians);
    double y4 = + (width / 2) * sin(radians) - (height / 2) * cos(radians);

    int* Return_max = (int*)malloc(2*sizeof(int));
    Return_max[0] = fmax(fmax(x1,x2),fmax(x3,x4));
    Return_max[1] = fmax(fmax(y1,y2),fmax(y3,y4));
    return Return_max;
}


static void drawOutlineRectangles(SDL_Renderer *renderer ,double alpha, SDL_Rect * rect , SDL_Rect ** Returned_fictif_rect)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    
    int* Max_dims = getMaxDims((*rect).w,(*rect).h,alpha);
    
    if(View::Env::rotational_debugmode)
            drawRotatedRect(renderer, (*rect).x + (*rect).w/2 - Max_dims[0], 
                        (*rect).y + (*rect).h/2 - Max_dims[1] , 2*Max_dims[0], 2*Max_dims[1], 0);

    SDL_Rect *fictif_Rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    
    fictif_Rect->x = (*rect).x + (*rect).w/2 - Max_dims[0];
    fictif_Rect->y = (*rect).y + (*rect).h/2 - Max_dims[1];
    fictif_Rect->w = 2*Max_dims[0];
    fictif_Rect->h = 2*Max_dims[1];
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    if(View::Env::rotational_debugmode) drawRotatedRect(renderer, (*rect).x , (*rect).y , (*rect).w, (*rect).h, alpha);
    if(Max_dims)free(Max_dims);

    if(Returned_fictif_rect) *Returned_fictif_rect = fictif_Rect;
    else if(fictif_Rect) free(fictif_Rect);
    
    
}


void View::Render::DrawEntity(const Model::Creature& entity) const{
    // need maintenance

    
    if(entity.animation.debugColor){
        auto& r = *entity.animation.debugColor;
        SDL_SetRenderDrawColor(renderer, r[0], r[1], r[2], r[3]);
    }
    else SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    
    entity.RenderEntity(renderer);

    SDL_Rect decoy = (SDL_Rect) entity;
    SDL_Rect *fictif_rect = &decoy;

    int MirrorX = decoy.x ;
    int MirrorY = decoy.y ;

    int fictifMirrorX = (*fictif_rect).x ;
    if(fictifMirrorX < 0) 
    {
        fictifMirrorX = Env::WINDOW[Coords::pX] - abs(fictifMirrorX);
        MirrorX += Env::WINDOW[Coords::pX];
    }
    else if(fictifMirrorX >= Env::WINDOW[Coords::pX] - (*fictif_rect).w) 
    {
        fictifMirrorX -= Env::WINDOW[Coords::pX];
        MirrorX -= Env::WINDOW[Coords::pX];
    }

    int fictifMirrorY = (*fictif_rect).y ;
    if(fictifMirrorY < 0) 
    {
        fictifMirrorY = Env::WINDOW[Coords::pY] - abs(fictifMirrorY);
        MirrorY += Env::WINDOW[Coords::pY];
    }
    else if(fictifMirrorY >= Env::WINDOW[Coords::pY] - (*fictif_rect).h) 
    {
        fictifMirrorY -= Env::WINDOW[Coords::pY];
        MirrorY -= Env::WINDOW[Coords::pY];
    }

    SDL_Rect Mirror_rect;
    Mirror_rect.x = 0;
    Mirror_rect.y = 0;
    Mirror_rect.w = decoy.w;
    Mirror_rect.h = decoy.h;



    if(fictifMirrorX != (*fictif_rect).x && fictifMirrorY != (*fictif_rect).y)
    {
        Mirror_rect.x = MirrorX;
        Mirror_rect.y = MirrorY;
        SDL_SetRenderDrawColor(renderer,4, 255, 2, 255);
        entity.RenderEntity(renderer, &Mirror_rect);
        drawOutlineRectangles(renderer,entity.animation.Srotation, &Mirror_rect, NULL);
    }
    if(fictifMirrorX != (*fictif_rect).x)
    {
        Mirror_rect.x = MirrorX;
        Mirror_rect.y = decoy.y;
        SDL_SetRenderDrawColor(renderer,4, 255, 2, 255);
        entity.RenderEntity(renderer, &Mirror_rect);
        drawOutlineRectangles(renderer,entity.animation.Srotation, &Mirror_rect, NULL);
    }
    if(fictifMirrorY != (*fictif_rect).y)
    {
        Mirror_rect.x = decoy.x;
        Mirror_rect.y = MirrorY;
        SDL_SetRenderDrawColor(renderer,4, 255, 2, 255);
        entity.RenderEntity(renderer, &Mirror_rect);
        drawOutlineRectangles(renderer,entity.animation.Srotation, &Mirror_rect, NULL);
    }

    if(Env::low_debugmode){
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, (int) entity.fetch_gCenter()[Coords::pX], (int) entity.fetch_gCenter()[Coords::pY],
                        (int) entity.oCenter(Coords::pX), (int) entity.oCenter(Coords::pY));
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

}

View::Render::~Render(){
    delete event;
    SDL_DestroyTexture(Env::spriteSheet_Fish);
    SDL_DestroyTexture(Env::spriteSheet_Predator);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::Render::processEvents(){
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            keepRunning = false;
        }
    }
}

bool View::Render::stillRunning() const{
    return keepRunning;
}