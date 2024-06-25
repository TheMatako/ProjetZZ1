#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "LasVegas.h"

int main()
{
    GameState game = initGame();
    initRound(game);
    int n = game.round;
    int r = game.playerTurn;
    printf("%d,%d\n",n,r);
    
   
    throwBanknotes(&game);
    
   
    
    int a,b;
    throwDices(&game);
    for (int i=0; i < game.player[game.playerTurn].dicesLeft;i++)
    {   a=game.player[game.playerTurn].currentThrow[i];
        b=game.player[game.playerTurn].dicesLeft;
        printf("%d,%d, %d \n", a,b, i); 
    }

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
}