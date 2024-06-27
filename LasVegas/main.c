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

    while(game.round != 4)
    {
        game = initRound(game);
        game = throwBanknotes(game);
        
        while(!game.roundFinished)
        {
            if(game.player[game.playerTurn].dicesLeft)
            {
                game = throwDices(&game);
                gameDisplay(game);
                int diceChosen = mainSDL(game);
                game = choiceDice(game, diceChosen);
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

