#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LasVegas.h"
#include "MCTS.h"

int main()
{
    GameState game = initGame();
    game = initRound(game);

    int d = -1; int c = -1;
    
    while(game.round != 4)
    {
        game.roundFinished = false;
        game = throwBanknotes(game);

        while(!game.roundFinished)
        {
            game = throwDices(&game);
            gameDisplay(game);
            printf("Alors, quel groupe de dés choisis-tu ? ");
            while(!game.player[game.playerTurn].currentThrow[d])
            {
                scanf("%d",&d);
                if(!game.player[game.playerTurn].currentThrow[d])
                    printf("\nNope ! quel groupe de dés choisis-tu ? ");
            }
            printf("\nEt donc, sur quel Casino ? ");
            while(!(c-- >= 0 && c-- <= 5))
            {
                scanf("%d",&c);
                if(!game.player[game.playerTurn].currentThrow[c])
                    printf("\nNope ! Sur quel Casino mises-tu ? ");
            }

            game.player[game.playerTurn].dicesLeft -= game.player[game.playerTurn].currentThrow[d];
            game.player[game.playerTurn].dicesChosen = d;
            game.player[game.playerTurn].casinoChosen = c;
            game.casino[d].dicesPlaced[game.playerTurn] = game.player[game.playerTurn].currentThrow[d];

            if(game.playerTurn == 0)
                game.playerTurn = 1;
            else
                game.playerTurn = 0;

            game.turn++;
 
            if(!game.player[0].dicesLeft && !game.player[1].dicesLeft)
            {
                game.round++; game.roundFinished = true;
            }
        }  
    }
}