

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "structureQuoridor.h"
#include "Rim/barrierInteraction.h" 



//inutile
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
        if (Jeu.players[0].pos.y == 0)
        {
            Jeu.isGameRunning = false;
            printf("Joueur 1 a gagné !\n");
        }
        if (Jeu.players[1].pos.y == 8)
        {
            Jeu.isGameRunning = false;
            printf("Joueur 2 a gagné !\n");
        }
        
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:
                    Jeu.isGameRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) 
                    {
                        case SDL_SCANCODE_ESCAPE:
                            Jeu.isGameRunning = false;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) 
                        {
                            getCursorIndex(Jeu, &Jeu.players[Jeu.playerTurn].pos.x, &Jeu.players[Jeu.playerTurn].pos.y, &mouvementEffectue, boxesPlayable);
                            if (mouvementEffectue) 
                            {
                                Jeu.playerTurn = (Jeu.playerTurn + 1) % 2;
                                mouvementEffectue = false;
                            }
                        
                            // Barri�re Verticale
                            int barrierVerticalWidth, barrierVerticalHeight;
                            SDL_QueryTexture(allImages[14], NULL, NULL, &barrierVerticalWidth, &barrierVerticalHeight); 
                            float barrierVerticalX = WINDOW_WIDTH - (3*SPACE_LENGTH+BOX_WIDTH)+ barrierVerticalWidth/2;
                            int barrierVerticalY = WINDOW_HEIGHT - 125;
                            SDL_Rect dstRectbarrierVertical = {barrierVerticalX, barrierVerticalY, SPACE_LENGTH, 100};
                            SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectbarrierVertical);
                            // Barri�re Horizontale
                            int barrierHorizontalWidth, barrierHorizontalHeight;
                            SDL_QueryTexture(allImages[13], NULL, NULL, &barrierHorizontalWidth, &barrierHorizontalHeight); 
                            float barrierHorizontalX = SPACE_LENGTH + BOX_WIDTH/2;
                            int barrierHorizontalY = WINDOW_HEIGHT - 85;
                            SDL_Rect dstRectbarrierHorizontal = {barrierHorizontalX, barrierHorizontalY, 2*(BOX_WIDTH+SPACE_LENGTH), 20};

                            if (Jeu.isDragging)
                            {
                                Jeu.isDragging = 0;   
                            }
                            else
                            {
                                int positionX, positionY;
                                SDL_GetMouseState(&positionX, &positionY);
                                if (isPointInsideRect(positionX, positionY, dstRectbarrierVertical))
                                {
                                    Jeu.isDragging = 2;
                                }
                                else if (isPointInsideRect(positionX, positionY, dstRectbarrierHorizontal))
                                {
                                    Jeu.isDragging = 1;
                                }
                            }
        
                        }
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
