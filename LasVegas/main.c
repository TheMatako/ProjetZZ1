#include <stdio.h>
#include <time.h>

#include "LasVegas.h"

int main () 
{
    GameState game = initGame();
    initRound(game);
    int n = game.round;
    int r = game.playerTurn;
    printf("%d,%d\n",n,r);

    int a,b;
    throwDices(&game);
    for (int i=0; i < game.player[game.playerTurn].dicesLeft;i++)
    {   a=game.player[game.playerTurn].currentThrow[i];
        b=game.player[game.playerTurn].dicesLeft;
        printf("%d,%d, %d \n", a,b, i); 
    }
   
    return 0;

}