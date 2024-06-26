#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "SDL.h"
#include "LasVegas.h"
#include "MCTS.h"

int main()
{
    GameState game = initGame();
    game = throwBanknotes(game);

    game.round = 0;
    game.playerTurn = 0;

    int dice; int group;

    while(game.round != 4)
    {
        game = initRound(game);
        while(!game.roundFinished)
        {
            dice = game.player[game.playerTurn].dicesLeft;
            game = throwDices(&game);
            gameDisplay(game);
            mainSDL(game);
            group = 0;
            printf("Alors, quel groupe de dés choisis-tu ? ");
            while(group == 0)
            {
                scanf("%d%*c",&dice);
                printf("\nVous avez choisis les : %d\n", dice);
                group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
                printf("\nIl y a %d %d dans le lancé\n", group, dice);
                if(group == 0)
                    printf("\nNope ! quel groupe de dés choisis-tu ?");
            }
            printf("\n\n||||||||||||||||||||||||||||||||||||||||||||||\n\n\n");
            game.player[game.playerTurn].dicesLeft -= group;
            game.player[game.playerTurn].dicesChosen = dice-1;
            game.casino[dice-1].dicesPlaced[game.playerTurn] += group;

            game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
            if (game.playerTurn == 1) game.turn++;
            printf("Dé restant joueur 0: %d, joueur 1: %d\n", game.player[0].dicesLeft == 0, game.player[1].dicesLeft == 0);
            if(game.player[0].dicesLeft == 0 && game.player[1].dicesLeft == 0)
            {
                game.round++; 
                game.roundFinished = true;
            }
        }  
    }
}