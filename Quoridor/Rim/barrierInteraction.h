#ifndef BARRIER_INTERACTION_H
#define BARRIER_INTERACTION_H

#include <SDL2/SDL.h>
#include "../structureQuoridor.h"
/*
void handleMouseDown(SDL_MouseButtonEvent *btn, GameState *game);
void handleMouseMotion(SDL_MouseMotionEvent *motion, GameState *game);
void handleMouseUp(SDL_MouseButtonEvent *btn, GameState *game);
bool isValidBarrierPosition(GameState *game, SDL_Rect proposedRect,bool isHorizontal);*/
bool isPointInsideRect(int positionX, int positionY, SDL_Rect rect);

#endif
