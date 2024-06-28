#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "SDL.h"
#include "LasVegas.h"
#include "MCTS.h"
#include <SDL2/SDL_ttf.h>



int main()
{
    //mainWithSdlAndAi();
    mainWithSdlWithoutAi();
    //mainTerminalWithAI();
}

int mainWithSdlAndAi()
{
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(18 * sizeof(SDL_Texture *));
    SDL_Texture **dicesImages = malloc(16 * sizeof(SDL_Texture *));

    // Allocation du tableau de pointeurs vers les lignes
    SDL_Texture ***dicesPlayersImages = malloc(NUMBER_PLAYERS * sizeof(SDL_Texture **));
    if (!dicesPlayersImages) 
    {
        fprintf(stderr, "Failed to allocate memory for dicesPlayersImages.\n");
        SDL_Quit();
        return 0;
    }

    TTF_Init();
    // Allocation de chaque ligne
    for (int i = 0; i < NUMBER_PLAYERS; i++) 
    {
        dicesPlayersImages[i] = malloc(6 * sizeof(SDL_Texture *));
        if (!dicesPlayersImages[i]) 
        {
            fprintf(stderr, "Failed to allocate memory for dicesPlayersImages[%d].\n", i);
            // Libération de la mémoire déjà allouée
            for (int j = 0; j < i; j++) 
            {
                free(dicesPlayersImages[j]);
            }
            free(dicesPlayersImages);
            SDL_Quit();
            return 0;
        }
        // Initialiser chaque pointeur de texture à NULL
        for (int j = 0; j < 6; j++) 
        {
            dicesPlayersImages[i][j] = NULL;
        }
    }

    if (!allImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 0;
    }
    if (!dicesImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 0;
    }
    for (int i = 0; i < 16; i++) 
    {
        if (i < 6) dicesImages[i] = NULL;
        allImages[i] = NULL;
    }
    initSDL(&window, &renderer);
    // Chargez toutes les images nécessaires au jeu
    loadTextures(renderer, &allImages);
    loadTexturesDices(renderer, &dicesImages);
    loadDicesPlayersTextures(renderer, dicesPlayersImages);
    

    hashTable * hash = createHashTable();
    int N = 1;
    GameState game = initGame();
    Node_t * AIMOVE;
    game.round = 0;
    game.playerTurn = 0;
    int running = 1;
    while(game.round != 4 && running == 1)
    {
        game = initRound(game);
        game = throwBanknotes(game);
        int dice = 0;
        int diceChosen = 0;
        int appliedTurn = 1;

        
        while(!game.roundFinished && running == 1)
        {
            if (game.player[game.playerTurn].dicesLeft <= 0 && game.player[(game.playerTurn+1)%NUMBER_PLAYERS].dicesLeft > 0 )
            {
                game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            if (appliedTurn)
            {
                game = throwDices(&game);
                appliedTurn = 0;
            }
            //gameDisplay(game);
            SDL_Event event;
            while (SDL_PollEvent(&event)) 
            {
                switch (event.type) 
                {
                    case SDL_QUIT:
                        running = 0;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.scancode) 
                        {
                            case SDL_SCANCODE_ESCAPE:
                                running = 0;
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT && game.playerTurn == 0) 
                        {
                            diceChosen = checkHitbox(game);
                            if (diceChosen > 0)
                            {
                                appliedTurn = 1;
                                game = applyOneTurn(game,diceChosen);
                                break;
                            }
                        }
                }
            }
            

            drawGame(game, renderer, allImages, dicesImages, dicesPlayersImages);

            SDL_RenderPresent(renderer);
            SDL_Delay(50); // Délai pour limiter la fréquence de mise à jour
            if (game.playerTurn == 1 && game.player[1].dicesLeft > 0)
            {
                AIMOVE = MCTS(game,hash,1,N);
                dice = AIMOVE->currentGame.player[1].dicesChosen;
                appliedTurn = 1;
                game = applyOneTurn(game,dice);
            }
        }
    }



    // Libération des ressources allouées
    for (int i = 0; i < 18; i++) 
    {
        if (i < 6)
        {
            if (dicesImages[i] != NULL) SDL_DestroyTexture(dicesImages[i]);
        }
        if (allImages[i] != NULL) SDL_DestroyTexture(allImages[i]);
    }
    // Libération de la mémoire
    for (int i = 0; i < NUMBER_PLAYERS; i++) 
    {
        for (int j = 0; j < 6; j++) 
        {
            if (dicesPlayersImages[i][j] != NULL) 
            {
                SDL_DestroyTexture(dicesPlayersImages[i][j]);
            }
        }
        free(dicesPlayersImages[i]);
    }

    free(dicesPlayersImages);
    free(dicesImages);
    free(allImages);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

int mainWithSdlWithoutAi()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(18 * sizeof(SDL_Texture *));
    SDL_Texture **dicesImages = malloc(16 * sizeof(SDL_Texture *));

    // Allocation du tableau de pointeurs vers les lignes
    SDL_Texture ***dicesPlayersImages = malloc(NUMBER_PLAYERS * sizeof(SDL_Texture **));
    if (!dicesPlayersImages) 
    {
        fprintf(stderr, "Failed to allocate memory for dicesPlayersImages.\n");
        SDL_Quit();
        return 0;
    }

    // Allocation de chaque ligne
    for (int i = 0; i < NUMBER_PLAYERS; i++) 
    {
        dicesPlayersImages[i] = malloc(6 * sizeof(SDL_Texture *));
        if (!dicesPlayersImages[i]) 
        {
            fprintf(stderr, "Failed to allocate memory for dicesPlayersImages[%d].\n", i);
            // Libération de la mémoire déjà allouée
            for (int j = 0; j < i; j++) 
            {
                free(dicesPlayersImages[j]);
            }
            free(dicesPlayersImages);
            SDL_Quit();
            return 0;
        }
        // Initialiser chaque pointeur de texture à NULL
        for (int j = 0; j < 6; j++) 
        {
            dicesPlayersImages[i][j] = NULL;
        }
    }

    if (!allImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 0;
    }
    if (!dicesImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 0;
    }
    for (int i = 0; i < 16; i++) 
    {
        if (i < 6) dicesImages[i] = NULL;
        allImages[i] = NULL;
    }
    initSDL(&window, &renderer);
    // Chargez toutes les images nécessaires au jeu
    loadTextures(renderer, &allImages);
    loadTexturesDices(renderer, &dicesImages);
    loadDicesPlayersTextures(renderer, dicesPlayersImages);

    GameState game = initGame();
    game.round = 0;
    game.playerTurn = 0;
    int running = 1;
    while(game.round != 4 && running == 1)
    {
        game = initRound(game);
        game = throwBanknotes(game);
        int dice = 0;
        int diceChosen = 0;
        int appliedTurn = 1;
        while(!game.roundFinished && running == 1)
        {
            if (game.player[game.playerTurn].dicesLeft <= 0) (game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            if (appliedTurn)
            {
                game = throwDices(&game);
                appliedTurn = 0;
            }
            //gameDisplay(game);
            SDL_Event event;
            while (SDL_PollEvent(&event)) 
            {
                switch (event.type) 
                {
                    case SDL_QUIT:
                        running = 0;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.scancode) 
                        {
                            case SDL_SCANCODE_ESCAPE:
                                running = 0;
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) 
                        {
                            diceChosen = checkHitbox(game);
                            if (diceChosen > 0)
                            {
                                appliedTurn = 1;
                                game = applyOneTurn(game,dice);
                                break;
                            }
                        }
                }
            }


            drawGame(game, renderer, allImages, dicesImages, dicesPlayersImages);
            SDL_RenderPresent(renderer);
            SDL_Delay(50); // Délai pour limiter la fréquence de mise à jour
        }
    }



    // Libération des ressources allouées
    for (int i = 0; i < 18; i++) 
    {
        if (i < 6)
        {
            if (dicesImages[i] != NULL) SDL_DestroyTexture(dicesImages[i]);
        }
        if (allImages[i] != NULL) SDL_DestroyTexture(allImages[i]);
    }
    // Libération de la mémoire
    for (int i = 0; i < NUMBER_PLAYERS; i++) 
    {
        for (int j = 0; j < 6; j++) 
        {
            if (dicesPlayersImages[i][j] != NULL) 
            {
                SDL_DestroyTexture(dicesPlayersImages[i][j]);
            }
        }
        free(dicesPlayersImages[i]);
    }

    free(dicesPlayersImages);
    free(dicesImages);
    free(allImages);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


int mainTerminalWithAI()
{
    hashTable * hash = createHashTable();
    int N = 1;
    GameState game = initGame();
    game.playerTurn = 0;
    game.round = 0;
    game.roundFinished = false;

    Node_t * AIMOVE;

    while(game.round != 4)
    {
        game = initRound(game);
        game.playerTurn = 0;
        game.round = 1;
        game.roundFinished = false;
        while(!game.roundFinished)
        {
            game.roundFinished = false;
            game = throwBanknotes(game);
            int dice = -1; int group = 0;
            if(game.player[game.playerTurn].dicesLeft)
            {
                game = throwDices(&game);
                gameDisplay(game);
                if(game.playerTurn == 0) // CETTE VERMINE D'HUMAIN JOUE
                {
                    while(group == 0)
                    {
                        printf("Alors, quel groupe de dés choisis-tu ? ");
                        scanf("%d%*c",&dice);
            
                        group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
                        if(group == 0)
                            printf("\nNope ! quel groupe de dés choisis-tu ?");
                    }
                }
                else // MA GRANDE MAJESTE JOUE
                {
                    AIMOVE = MCTS(game,hash,game.playerTurn,N);
                    dice = AIMOVE->currentGame.player[game.playerTurn].dicesChosen;
                    printf("\nMoi, grand IA, j'ai choisi le dé : %d\n",dice);
                }
            }
            game = applyOneTurn(game,dice);
        }
    }
    return 0;
}
