#ifndef SDL_H
#define SDL_H

#include "LasVegas.h"



void drawGame(GameState game,SDL_Renderer *renderer, SDL_Texture **allImages, SDL_Texture **dicesImages, SDL_Texture ***dicesPlayersImages, TTF_Font *font);

void drawLines(SDL_Renderer *renderer);
void drawDiceImages(SDL_Renderer *renderer, SDL_Texture **dicesImages);
void drawCasino(SDL_Renderer *renderer, SDL_Texture **allImages);
void drawBanknotes(GameState game, SDL_Renderer *renderer, SDL_Texture **allImages);
void drawPlayersThrows(GameState game, SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages);
void draw1Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw2Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw3Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw4Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw5Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw6Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw7Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void draw8Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice);
void drawDicesOverCasino(GameState game, SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int player);
void drawBackground(GameState game, SDL_Renderer *renderer, SDL_Texture **allImages);
void drawText(SDL_Renderer *renderer, GameState game, TTF_Font *font);

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages);
void loadTexturesDices(SDL_Renderer *renderer, SDL_Texture ***dicesImages);
void loadDicesPlayersTextures(SDL_Renderer *renderer, SDL_Texture ***dicesPlayersImages);
int mainSDL(GameState game, SDL_Renderer *renderer, SDL_Texture **allImages, SDL_Texture **dicesImages, SDL_Texture ***dicesPlayersImages);

int checkHitbox(GameState game);
int repartizeDice(GameState game);
#endif