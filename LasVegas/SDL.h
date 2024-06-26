#ifndef SDL_H
#define SDL_H

#include "LasVegas.h"



void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, SDL_Texture **dicesImages);

void drawLines(SDL_Renderer *renderer);
void drawDiceImages(SDL_Renderer *renderer, SDL_Texture **dicesImages);
void drawCasino(SDL_Renderer *renderer, SDL_Texture **allImages);

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages);
void loadTexturesDices(SDL_Renderer *renderer, SDL_Texture ***dicesImages);
#endif