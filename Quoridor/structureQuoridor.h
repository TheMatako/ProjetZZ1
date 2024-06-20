#ifndef QUORIDOR_H
#define QUORIDOR_H

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, GameState Jeu);
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages);
GameState initGame();
void getCursorIndex(GameState game, int *positionX, int *positionY, bool *mouvementEffectue);

#endif