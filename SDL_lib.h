#ifndef SDL_LIB_H_INCLUDED
#define SDL_LIB_H_INCLUDED

#include <iostream>
#include <string>
#include <SDL.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 627;
const std::string WINDOW_TITLE = "Game 2048";

// xuất thông báo lỗi
void logSDLError(std::ostream &os, const std::string &msg, bool fatal = false);
// Mở cửa sổ
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
// đóng của sổ
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
// Lấy ảnh
SDL_Texture* loadTexture(SDL_Renderer* renderer,std::string path);

#endif // SDL_LIB_H_INCLUDED
