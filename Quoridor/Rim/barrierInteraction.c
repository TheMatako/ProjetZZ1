#include "../structureQuoridor.h"
#include "barrierInteraction.h"




void handleMouseDown(SDL_MouseButtonEvent *event, GameState *gameState) {
    if (event->button == SDL_BUTTON_LEFT) {
        SDL_Point mousePos = {event->x, event->y};
        for (int i = 0; i < BARRIER_NUMBER; i++) {
            if (!gameState->barriers[i].isPlaced && isPointInsideRect(mousePos, gameState->barriers[i].rect)) {
                gameState->isDragging = true;
                gameState->draggedBarrier = &gameState->barriers[i];
                bool isHorizontal = gameState->barriers[i].isHorizontal;

                printf("Dragging a %s barrier\n", isHorizontal ? "horizontal" : "vertical");

                int dragWidth = isHorizontal ? BARRIER_WIDTH : BARRIER_HEIGHT;
                int dragHeight = isHorizontal ? BARRIER_HEIGHT : BARRIER_WIDTH;

                printf("Initial dimensions: %d x %d\n", dragWidth, dragHeight);

                gameState->dragRect = (SDL_Rect){
                    mousePos.x - (dragWidth / 2),
                    mousePos.y - (dragHeight / 2),
                    dragWidth,
                    dragHeight
                };
                break;
            }
        }
    }
}


/* void handleMouseMotion(SDL_MouseMotionEvent *motion, GameState *gameState) {
    if (gameState->isDragging && gameState->draggedBarrier!=NULL) {
        bool isHorizontal = gameState->draggedBarrier->isHorizontal; // Utilisation de la propriété stockée
        int dragWidth = isHorizontal ? BARRIER_WIDTH : BARRIER_HEIGHT;
        int dragHeight = isHorizontal ? BARRIER_HEIGHT : BARRIER_WIDTH;

        gameState->dragRect.x = motion->x - (dragWidth / 2);
        gameState->dragRect.y = motion->y - (dragHeight / 2);

    }
}

*/
void handleMouseMotion(SDL_MouseMotionEvent *motion, GameState *gameState) {
    if (gameState->isDragging && gameState->draggedBarrier != NULL) {
        bool isHorizontal = gameState->draggedBarrier->isHorizontal;
        int dragWidth = isHorizontal ? BARRIER_WIDTH : BARRIER_HEIGHT;
        int dragHeight = isHorizontal ? BARRIER_HEIGHT : BARRIER_WIDTH;

        gameState->dragRect.x = motion->x - (dragWidth / 2);
        gameState->dragRect.y = motion->y - (dragHeight / 2);
        gameState->dragRect.w = dragWidth;  // Gardez les dimensions originales
        gameState->dragRect.h = dragHeight; // Gardez les dimensions originales

        
    }
}



void handleMouseUp(SDL_MouseButtonEvent *event, GameState *gameState) {
    if (event->button == SDL_BUTTON_LEFT && gameState->isDragging) {
        // Validation de la position pourrait être implémentée ici
        /*if (isValidBarrierPosition(gameState, gameState->dragRect, gameState->draggedBarrier->isHorizontal)) {*/
            gameState->draggedBarrier->isPlaced = true;
            gameState->draggedBarrier->rect = gameState->dragRect; // Mettre à jour la position de la barrière
        
        gameState->isDragging = false;
        gameState->draggedBarrier = NULL;
    }
}


/*
bool isValidBarrierPosition(GameState *game, int x, int y) {
    // Implémentez la logique pour vérifier si la position est valide pour placer une barrière
    return true;  
}*/


bool isPointInsideRect(SDL_Point point, SDL_Rect rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.w &&
            point.y >= rect.y && point.y <= rect.y + rect.h);
}

