#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "structureQuoridor.h"

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(16 * sizeof(SDL_Texture *));
    initSDL(&window, &renderer);

    // On load toutes les images nécessaires au jeu
    // Actuellement de manière pas propre
    // Images 0 à 9 de 0 à 9, Box = 10, Player 1 = 11, Player 2 = 12, BarrièreHorizon = 13, BarrièreVerti = 14, Curseur = 15
    loadTextures(renderer, &allImages);

    // On initialise le jeu et ses composantes
    GameState Jeu = initGame();
    bool mouvementEffectue = false;
    while (Jeu.isGameRunning) 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                Jeu.isGameRunning = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)  // On gère les déplacements en fonction des flèches directionnelles pressées
                {                                   // On évite de permettre au serpent de faire un demi tour d'un coup
                    case SDL_SCANCODE_ESCAPE:
                        Jeu.isGameRunning = false;
                        break;
                    case SDL_SCANCODE_SPACE:
                        Jeu.players[0].barriersLeft -= 1;
                        break;
                    default:
                        break;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    getCursorIndex(Jeu, &Jeu.players[Jeu.playerTurn].pos.x, &Jeu.players[Jeu.playerTurn].pos.y, &mouvementEffectue);
                    if (mouvementEffectue)
                    {
                        Jeu.playerTurn = (Jeu.playerTurn + 1) % 2;
                        mouvementEffectue = false;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_RenderClear(renderer);

        drawGame(renderer, allImages, Jeu);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}