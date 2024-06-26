#ifndef SDL_H
#define SDL_H

#include "LasVegas.h"







void drawGame1(SDL_Renderer *renderer);
void drawGame2(SDL_Renderer *renderer);
void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages);

#endif