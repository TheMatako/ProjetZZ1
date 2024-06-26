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
            group = 0;
            printf("Alors, quel groupe de dés choisis-tu ? ");
            while(group == 0)
            {
                scanf("%d%*c",&dice);
                group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
                if(group == 0)
                    printf("\nNope ! quel groupe de dés choisis-tu ?");
            }
            printf("\n\n||||||||||||||||||||||||||||||||||||||||||||||\n\n");
            game.player[game.playerTurn].dicesLeft -= group;
            game.player[game.playerTurn].dicesChosen = dice-1;
            game.casino[dice-1].dicesPlaced[game.playerTurn] = group;

            game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
            game.turn++;
 
            if(!game.player[0].dicesLeft && !game.player[1].dicesLeft)
            {
                game.round++; game.roundFinished = true;
            }
        }  
    }
}