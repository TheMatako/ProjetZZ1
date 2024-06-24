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

bool isPointInsideRect(SDL_Point point, SDL_Rect rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.w &&
            point.y >= rect.y && point.y <= rect.y + rect.h);
}

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
        if (isValidBarrierPosition(gameState, gameState->dragRect, gameState->draggedBarrier->isHorizontal)) {
            gameState->draggedBarrier->isPlaced = true;
            gameState->draggedBarrier->rect = gameState->dragRect; // Mettre à jour la position de la barrière
            gameState->players[gameState->playerTurn].barriersLeft--; // Décrémenter le nombre de barrières
            gameState->playerTurn = (gameState->playerTurn + 1) % 2; // Changer le tour après un placement valide
        }
        gameState->isDragging = false;
        gameState->draggedBarrier = NULL;
    }
}




bool isValidBarrierPosition(GameState *game, SDL_Rect proposedRect, bool isHorizontal) {
    // Vérifier que la barrière ne chevauche pas d'autres barrières placées
    for (int i = 0; i < BARRIER_NUMBER; i++) {
        if (game->barriers[i].isPlaced && SDL_HasIntersection(&proposedRect, &game->barriers[i].rect)) {
            return false; // Retourne faux si une intersection est trouvée
        }
    }

    // Assurer que la barrière reste dans les limites du plateau
    if (proposedRect.x < SPACE_LENGTH || proposedRect.x + proposedRect.w > WINDOW_WIDTH - SPACE_LENGTH ||
        proposedRect.y < TOP_LENGTH || proposedRect.y + proposedRect.h > WINDOW_HEIGHT - BOTTOM_LENGTH) {
        return false;
    }

    // Flexibilité dans l'alignement sur la grille
    const int tolerance = SPACE_LENGTH*3/4;  // Une demi-largeur de l'espace entre les cases

    if (isHorizontal) {
        // Pour les barrières horizontales, vérifier l'alignement horizontal avec une tolérance
        int gridLineX = SPACE_LENGTH;
        while (gridLineX <= WINDOW_WIDTH - SPACE_LENGTH) {
            if (abs((proposedRect.x + proposedRect.w / 2) - gridLineX) <= tolerance) {
                return true;  // La barrière est approximativement alignée
            }
            gridLineX += BOX_WIDTH + SPACE_LENGTH;
        }
    } else {
        // Pour les barrières verticales, vérifier l'alignement vertical avec une tolérance
        int gridLineY = TOP_LENGTH;
        while (gridLineY <= WINDOW_HEIGHT - BOTTOM_LENGTH) {
            if (abs((proposedRect.y + proposedRect.h / 2) - gridLineY) <= tolerance) {
                return true;  // La barrière est approximativement alignée
            }
            gridLineY += BOX_HEIGHT + SPACE_LENGTH;
        }
    }

    return false;  // Aucun alignement acceptable trouvé
}

