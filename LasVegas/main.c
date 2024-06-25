#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MCTS.h"

int main()
{
    GameState game = initGame();
    game = throwBanknotes(game);
    game = throwDices(&game);
    
    gameDisplay(game);
    // GameState game = initGame();
    // initRound(game);
    // int n = game.round;
    // int r = game.playerTurn;
    // printf("%d,%d\n",n,r);
    
    // int c = randBanknotes(&game);
    // printf("%d\n",c);
   
    throwBanknotes(&game);
    
    // int a,b;
    // throwDices(&game);
    // for (int i=0; i < game.player[game.playerTurn].dicesLeft;i++)
    // {   a=game.player[game.playerTurn].currentThrow[i];
    //     b=game.player[game.playerTurn].dicesLeft;
    //     printf("%d,%d, %d \n", a,b, i); 
    // }

/* int i;

    while(game.round != 4)
    {    
        initRound(game);
        for(i=0);
        while(!game.roundFinished)
        {

        }
    }

    gameDisplay(game);*/

    return 0;
gameDisplay(game);*/
    // return 0;
}