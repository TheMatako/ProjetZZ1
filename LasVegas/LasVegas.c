
#include "LasVegas.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>


GameState initGame() 
{
    GameState game;
    game.playerTurn = 0;
    game.round = 0;
    game.roundFinished = true;
    int Banknotes[10] = {48,5,7,7,5,6,5,5,4,4}; 
    memcpy(game.Banknotes, Banknotes, 10 * sizeof(int)); 

    for (int i = 0; i < NUMBER_PLAYERS; i++) {
        game.player[i].idPlayer = i;
        game.player[i].dicesLeft = 8;
        game.player[i].totalMoney = 0;
        memset(game.player[i].currentThrow, 0, NUMBER_DICES * sizeof(int));
    }

    for (int i = 0; i < 6; i++) {
        game.casino[i] = initCasino(i + 1);
    }

    return game;
}





Casino initCasino(int number) 
{
    Casino casino;
    casino.number = number;
    memset(casino.associatedValues, 0, MAX_BILLETS_PER_CASINO * sizeof(int)); // Initialiser les billets à 0
    memset(casino.dicesPlaced, 0, NUMBER_PLAYERS * sizeof(int)); // Initialiser les dés à 0 
    return casino;
}




void initRound(GameState game)
{
    game.round += 1;
    game.playerTurn = game.round % 2;
    game.roundFinished = !game.roundFinished; 
    
}



 int randBanknotes(GameState *game){
            srand(time(NULL));
            int r=rand()% game->Banknotes[0] +1;
            printf("le nbr aléa %d \n", r); 
            int n=0;
            int s = game->Banknotes[1];
            if(r<=s)
                n = 10;
            
            else {
                s+=game->Banknotes[2];
                if (r<=s) n = 20; 

                else{ 
                    s+=game->Banknotes[3];
                    if (r<=s) n = 30;
                    
                    else {
                        s+=game->Banknotes[4];
                        if (r<=s) n = 40;
                           
                        else {
                            s+=game->Banknotes[5];
                            if (r<=s) n = 50;

                            else{ 
                                s+=game->Banknotes[6];
                                if (r<=s) n = 60;

                                else{
                                    s+=game->Banknotes[7];
                                    if (r<=s) n = 70;

                                    else{ s+=game->Banknotes[8];
                                    if (r<=s) n = 80;

                                    else {
                                        if (r<=game->Banknotes[0]) n = 90;
                                         }
                                        }  
                                    }
                                }
                            }
                        }
                    }
               }
   return n; 
 }
 
/* 
void throwBanknotes() {
    srand(time(NULL)); 
    for (int i = 0; i < 6; i++) {
        int banknote = generateWeightedRandom();
        printf("Casino %d receives a banknote of type %d\n", i + 1, banknote);
    }
}
*/


void throwDices(GameState *game) 
{
    srand(time(NULL));
    for (int i = 0; i < game->player[game->playerTurn].dicesLeft; i++) {

        int value = rand() % 6 + 1;  //générer un nombre entre 1 et 6 (reprédente les faces des dés)
        game->player[game->playerTurn].currentThrow[i] = value;
    }
    
}


/* void distributeMoney(GameState game){

}
*/

