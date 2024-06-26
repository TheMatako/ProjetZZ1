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

    game.round = 0;
    game.playerTurn = 1;

    int dice; int group;
    while(game.round != 4)
    {
        game = initRound(game);
        game = throwBanknotes(game);
        
        while(!game.roundFinished)
        {
            if(game.player[game.playerTurn].dicesLeft)
            {
                dice = game.player[game.playerTurn].dicesLeft;
                game = throwDices(&game);
                gameDisplay(game);
                //mainSDL(game);
                group = 0;
                printf("Alors, quel groupe de dés choisis-tu ? ");
                while(group == 0)
                {
                    scanf("%d%*c",&dice);
                    group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
                    if(group == 0)
                        printf("\nNope ! quel groupe de dés choisis-tu ?");
                }
                game.player[game.playerTurn].dicesLeft -= group;
                game.player[game.playerTurn].dicesChosen = dice-1;
                game.casino[dice-1].dicesPlaced[game.playerTurn] += group;
                if (game.playerTurn%NUMBER_PLAYERS== 0) game.turn++;
            }
            printf("\n\n||||||||||||||||||||||||||||||||||||||||||||||\n\n\n");
            if (game.player[game.playerTurn+1].dicesLeft) game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
            printf("Dé restant joueur 0: %d, joueur 1: %d\n", game.player[0].dicesLeft == 0, game.player[1].dicesLeft == 0);
            if(game.player[0].dicesLeft <= 0 && game.player[1].dicesLeft <= 0)
            {
                game.roundFinished = true;
                game = distributeMoney(game);
            }
        }

    }
}

