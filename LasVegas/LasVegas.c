
#include "LasVegas.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "LasVegas.h"

GameState initGame()
{
    GameState game;
    game.playerTurn = 0;
    game.round = 0;
    game.roundFinished =true;
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
                n = 1;                  // 1 sera associé au billet de 10k
            
            else {
                s+=game->Banknotes[2];
                if (r<=s) n = 2; 

                else{ 
                    s+=game->Banknotes[3];
                    if (r<=s) n = 3;
                    
                    else {
                        s+=game->Banknotes[4];
                        if (r<=s) n = 4;
                           
                        else {
                            s+=game->Banknotes[5];
                            if (r<=s) n = 5;

                            else{ 
                                s+=game->Banknotes[6];
                                if (r<=s) n = 6;

                                else{
                                    s+=game->Banknotes[7];
                                    if (r<=s) n = 7;

                                    else{ s+=game->Banknotes[8];
                                    if (r<=s) n = 8;

                                    else {
                                        if (r<=game->Banknotes[0]) n = 9;
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
 


void throwBanknotes(GameState *game) 
{
    for (int i =0; i<6; i++)
    {
       int Banknotes_1_cas1 = randBanknotes(game);
       game->Banknotes[0]--;
       game->Banknotes[Banknotes_1_cas1]--;
       game->casino[i].associatedValues[0]= Banknotes_1_cas1;
       printf("on distribue  un billet de %d0k\n", Banknotes_1_cas1);     //juste un test

       if (Banknotes_1_cas1<=5)
       {
        int Banknotes_2_cas1 = randBanknotes(game);
        game->Banknotes[0]--;
        game->Banknotes[Banknotes_2_cas1]--;
        game->casino[i].associatedValues[1]= Banknotes_2_cas1;
        

        printf("on distribue aussi un billet de %d0k\n", Banknotes_2_cas1);   //encore un test

        if(Banknotes_1_cas1+Banknotes_2_cas1<5) 
        {
            int Banknotes_3_cas1 = randBanknotes(game);
            game->Banknotes[0]--;
            game->Banknotes[Banknotes_3_cas1]--;
            game->casino[i].associatedValues[2]= Banknotes_3_cas1;
            printf("on distribue aussi un billet de %d0k\n", Banknotes_3_cas1);  


            if(Banknotes_1_cas1+Banknotes_2_cas1+Banknotes_3_cas1<5) 
            {
                int Banknotes_4_cas1 = randBanknotes(game);
                game->Banknotes[0]--;
                game->Banknotes[Banknotes_4_cas1]--;
                game->casino[i].associatedValues[3]= Banknotes_4_cas1;
                
                if(Banknotes_1_cas1+Banknotes_2_cas1+Banknotes_3_cas1+Banknotes_4_cas1<50) 
                {
                    int Banknotes_5_cas1 = randBanknotes(game);
                    game->Banknotes[0]--;
                    game->Banknotes[Banknotes_5_cas1]--;
                    game->casino[i].associatedValues[4]= Banknotes_5_cas1;   // le cas où le casino
                                                                             //a 5 billets de 10k
                }
    
            }
        }       
       }
    }
}

void throwDices(GameState*game)
{
    srand(time(NULL));
    for (int i =0 ;i<NUMBER_DICES;i++)
    { 
        int value = rand()%6 +1;
        game->player[game->playerTurn].currentThrow[i]= value;
        
    }
}




int max(int Tab[])
{
    int i = 0; int max = 0;
    while(Tab[i] || Tab[i] == 0)
    {
        if(Tab[i] > Tab[max])
            max = i;
        i++;
    }
    return max;
}

bool doublons(int Tab[])
{
    int i = 0; int j = 0; bool O = false;

    while(Tab[i] || Tab[i] == 0)
    {
        while(Tab[j] || Tab[i] == 0)
        {
            if(Tab[i] == Tab[j] et tab[i] == number)
                O = true;
            j++;
        }
        i++;
    }
    return O;
}

void distributeMoney(GameState game)
{   
    int i; int j;
    for (i = 0; i <= 5; i++) 
    {
        Casino currentCasino = game.casino[i];

        for(j = 0; j <= 1; j++)
        {
            if(!doublons(currentCasino.dicesPlaced))
            {
                int winner = max(currentCasino.dicesPlaced);
                int maxNote = max(currentCasino.associatedValues);

                game.player[winner].totalMoney += maxNote;

                currentCasino.dicesPlaced[winner] = 0;
                currentCasino.associatedValues[maxNote] = 0;
            }
            else
            {
                for(j=0;j<NUMBER_PLAYERS;j++)
                {
                    if(game.casino[i].dicesPlaced[j] == game.casino[i].dicesPlaced[winner])
                        game.casino[i].dicesPlaced[j] = 0;
                }
            }
        }
    }
}

void gameDisplay(GameState game)
{
    printf("| Il s'agit du tour de %d :\n",game.playerTurn);

    int i; int u; int b;

    for(i=0;i<=5;i++)
    {
        Casino currentCasino = game.casino[i];
        printf("| CASINO N° %d : ",currentCasino.number);

        for(u=0;u<=1;u++)
            printf("%d dés pour le joueur %d , ",currentCasino.dicesPlaced[u],u);
        printf("\n");
        for(b=0;b<=MAX_BILLETS_PER_CASINO-1;b++)
            printf("1 billet de valeur : %d , ",currentCasino.associatedValues[b]);

        printf("\n\n");
    }
}

