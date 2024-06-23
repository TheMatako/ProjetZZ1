

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "structureQuoridor.h"
#include "Rim/barrierInteraction.h" 

int main() 
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(16 * sizeof(SDL_Texture *));
    if (!allImages) {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 1;
    }

    initSDL(&window, &renderer);

    // Chargez toutes les images nécessaires au jeu
    loadTextures(renderer, &allImages);

    // Initialisation du jeu et ses composantes
    GameState Jeu = initGame(allImages[14]);
    bool mouvementEffectue = false;

    while (Jeu.isGameRunning) 
    {
        int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE] = {0}; // Zones jouables, initialisées à zéro
        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_RenderClear(renderer);

        drawGame(renderer, allImages, Jeu, boxesPlayable); // Dessine le jeu actuel

        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) {
                case SDL_QUIT:
                    Jeu.isGameRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            Jeu.isGameRunning = false;
                            break;
                        case SDL_SCANCODE_SPACE:
                            Jeu.players[Jeu.playerTurn].barriersLeft -= 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        getCursorIndex(Jeu, &Jeu.players[Jeu.playerTurn].pos.x, &Jeu.players[Jeu.playerTurn].pos.y, &mouvementEffectue, boxesPlayable);
                        if (mouvementEffectue) {
                            Jeu.playerTurn = (Jeu.playerTurn + 1) % 2;
                            mouvementEffectue = false;
                        }
                        /*if (event.button.button == SDL_BUTTON_LEFT) {
                            ;
                            */
                        bool onBarrier = false;
                        for (int i = 0; i < BARRIER_NUMBER && !onBarrier; i++) {
                            SDL_Point mousePos = {event.button.x, event.button.y};
                            if (isPointInsideRect(mousePos, Jeu.barriers[i].rect)) {
                               
                               onBarrier = true;
                        }
                        
                        }

                        if (onBarrier) {
                        handleMouseDown(&event.button, &Jeu); // Handle drag start only if the cursor is on a barrier
                        }
    
                        }
                         break;
                        case SDL_MOUSEMOTION:
                        handleMouseMotion(&event.motion, &Jeu); // Gérer le mouvement du drag
                        break;
                        case SDL_MOUSEBUTTONUP:
                        handleMouseUp(&event.button, &Jeu); // Gérer le relâchement du drag
                        break;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100); // Délai pour limiter la fréquence de mise à jour
    }

    // Libération des ressources allouées
    for (int i = 0; i < 16; i++) {
        SDL_DestroyTexture(allImages[i]);
    }
    free(allImages);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
