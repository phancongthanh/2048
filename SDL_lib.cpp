#include "SDL_lib.h"

void logSDLError(std::ostream &os, const std::string &msg, bool fatal) {
    os << msg << " Error! : " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout,"SDL_Init",true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout,"CreateWindow",true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout,"CreateRenderer",true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
/* Cho máy ảo
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout,"SDL_Init",true);

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout,"CreateWindow",true);

    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout,"CreateSoftRenderer",true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
*/
void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(SDL_Renderer* renderer,std::string path) {
    //SDL_Surface* image = IMG_Load(path.c_str());
    SDL_Surface* image = SDL_LoadBMP(path.c_str());
    if (image == nullptr) logSDLError(std::cout,"IMG_Load",true);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,image);
    if (texture == nullptr) logSDLError(std::cout,"CreateTextureFromSurface",true);
    SDL_FreeSurface(image);
    return texture;
}
