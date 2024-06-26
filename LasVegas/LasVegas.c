#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MCTS.h"

GameState initGame()
{
    GameState game;
    game.playerTurn = 0;
    game.round = 0;
    game.roundFinished = true;
    game.turn = 1;
    int Banknotes[10] = {48,5,7,7,5,6,5,5,4,4}; 
    memcpy(game.Banknotes, Banknotes, 10 * sizeof(int)); 

    for (int i = 0; i < NUMBER_PLAYERS; i++) 
    {
        game.player[i].idPlayer = i;
        game.player[i].dicesLeft = 8;
        game.player[i].totalMoney = 0;
        memset(game.player[i].currentThrow, 0, NUMBER_DICES * sizeof(int));
    }
    return game;
}

Casino initCasino(int number)
{
    int i; int j;
    Casino casino;
    casino.number = number;
    for(i = 0 ; i < NUMBER_PLAYERS ; i++)
        casino.dicesPlaced[i] = 0;
    for(j = 0 ; j < MAX_BILLETS_PER_CASINO; j++)
        casino.associatedValues[j] = 0;
    memset(casino.associatedValues, 0, MAX_BILLETS_PER_CASINO * sizeof(int)); // Initialiser les billets à 0
    memset(casino.dicesPlaced, 0, NUMBER_PLAYERS * sizeof(int)); // Initialiser les dés à 0

    // Initialise les positions utiles pour la SDL des casinos et des zones de dés
    int column = ((number-1)%3);
    int line = (number-1)/3;
    casino.rectCasino[0] = 240+400*column;
    casino.rectCasino[1] = 255+360*line;
    casino.rectCasino[2] = 400;
    casino.rectCasino[3] = 180;
    printf("Casino n°%d en colonne : %d et ligne : %d\n", number, column, line);
    return casino;
}

GameState initRound(GameState game)
{
    game.round += 1;
    int Banknotes[10] = {48,5,7,7,5,6,5,5,4,4}; 
    memcpy(game.Banknotes, Banknotes, 10 * sizeof(int)); 
    game.playerTurn = game.round % NUMBER_PLAYERS;
    game.roundFinished = false;
    
    game.turn = 1;
    for (int i = 0; i < 6; i++) 
    {
        game.casino[i] = initCasino(i + 1);
    }
        
    for (int i = 0; i < NUMBER_PLAYERS; i++) 
    {
        game.player[i].idPlayer = i;
        game.player[i].dicesLeft = 8;
        memset(game.player[i].currentThrow, 0, NUMBER_DICES * sizeof(int));
    }
    return game;
}

int randBankNotes(GameState * game)
{
    int r = rand() % game->Banknotes[0] + 1;
    int n = 0;
    int s = game->Banknotes[1];
    if(r<=s)
        n = 1; // 1 sera associé au billet de 10k par exemple
    else
    {
        s+=game->Banknotes[2];
        if (r<=s) n = 2;

        else
        {
            s+=game->Banknotes[3];
            if (r<=s) n = 3;
            else
            {
                s+=game->Banknotes[4];
                if (r<=s) n = 4;
                else 
                {
                    s+=game->Banknotes[5];
                    if (r<=s) n = 5;
                    else
                    { 
                        s+=game->Banknotes[6];
                        if (r<=s) n = 6;
                        else
                        {
                            s+=game->Banknotes[7];
                            if (r<=s) n = 7;
                            else
                            {
                                s+=game->Banknotes[8];
                                if (r<=s) n = 8;
                                else
                                {
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


int sumList(int Tab[],int lenght)
{
    int sum = 0; int i;
    for(i = 0 ; i < lenght ; i++)
        sum = sum + Tab[i];
    return sum;
}

int max(int Tab[],int lenght)
{
    int i = 0; int maximum = 0;
    for(i = 0 ; i < lenght ; i++)
    {
        if(Tab[i] > Tab[maximum])
            maximum = i;
        i++;
    }
    return maximum;
}

bool doublons(int Tab[],int lenght, int number)
{
    int i = 0; int j = 0;
    for(i = 0 ; i < lenght ; i++)
    {
        for(j = 0 ; j < lenght ; j++)
        {
            if(Tab[i] == Tab[j] && Tab[i] == number)
                return true;
            j++;
        }
        i++;
    }
    return false;
}

// Tri le tableau avec le tri à bulle dans l'ordre croissant pour 0 et decroissant pour 1
void bubbleTea(int tab[],int lenght, int croissant)
{
    if (croissant)
    {
        int i, j, temp;
        for (i = 0; i < lenght-1; i++)
        {
            for (j = 0; j < lenght-i-1; j++)
            {
                if (tab[j] < tab[j+1]) 
                {
                    temp = tab[j];
                    tab[j] = tab[j+1];
                    tab[j+1] = temp;   
                }
            }
        }
    }
    else
    {
        int i, j, temp;
        for (i = 0; i < lenght-1; i++)
        {
            for (j = 0; j < lenght-i-1; j++)
            {
                if (tab[j] > tab[j+1]) 
                {
                    temp = tab[j];
                    tab[j] = tab[j+1];
                    tab[j+1] = temp; 
                }
            }
        }
    }
}


int occurrences(int tab[],int lenght,int number)
{   int c = 0;
    for(int i = 0; i<lenght; i++)
    {
        if(tab[i] == number) c++;
    }
    return c;
}

GameState throwBanknotes(GameState game)
{
    srand(time(0));
    int bankNote; int j = 0;
    for (int i = 0; i < 6; i++)
    {   
        while(sumList(game.casino[i].associatedValues,MAX_BILLETS_PER_CASINO) < 5)
        {
            bankNote = randBankNotes(&game);
            game.casino[i].associatedValues[j] = bankNote;
            game.Banknotes[0]--;
            game.Banknotes[bankNote]--;
            j++;
            printf("le Casino N° %d reçoit un billet de valeur %d0k \n", i + 1, bankNote);
        }
        printf("SOMME : %d0k\n",sumList(game.casino[i].associatedValues,MAX_BILLETS_PER_CASINO));
        j = 0;
        printf("\n");
    }
    return game;
}

GameState throwDices(GameState * game)
{
    srand(time(0));
    for (int i = 0 ;i < game->player[game->playerTurn].dicesLeft; i++)
    { 
        int value = rand()%6 +1;
        game->player[game->playerTurn].currentThrow[i]= value;
    }
    return *game;
}

GameState distributeMoney(GameState game)
{
    for (int i = 0; i < 6; i ++)
    {
        printf("Casino n°%d\n",i);
        game = distributeBiggestBanknote(game, game.casino[i]);
    }
    return game;
}

GameState distributeBiggestBanknote(GameState game, Casino casino)
{

    bubbleTea(casino.associatedValues, MAX_BILLETS_PER_CASINO, 1);
    printf("Casino %d [ ", casino.number);
    for (int w = 0; w < NUMBER_PLAYERS; w++)
    {
        printf("%d0K ", casino.associatedValues[w]);
    }
    printf("]\n");
    
    printf("[ ");
    for (int y = 0; y < NUMBER_PLAYERS; y++)
    {
        printf("%d ", casino.dicesPlaced[y]);
    }
    printf("]\n");

    printf("Plus gros billet = %d0K\n", casino.associatedValues[0]);
    
    int playerMaxed = max(casino.dicesPlaced, NUMBER_PLAYERS);
    if (doublons(casino.dicesPlaced, NUMBER_PLAYERS, playerMaxed))
    {
        for (int i = 0; i < MAX_BILLETS_PER_CASINO; i ++)
        {
            casino.associatedValues[i] = 0;
            for (int j = 0; j < NUMBER_PLAYERS; j++)
            {
                casino.dicesPlaced[j] = 0;
            }
        }
        return game;
    }
    else
    {
        int biggestBanknote = casino.associatedValues[0];
        casino.associatedValues[0] = 0;
        game.player[playerMaxed].totalMoney += biggestBanknote;
        casino.dicesPlaced[playerMaxed] = 0;
        printf("distribué au player %d\n",playerMaxed);
        for (int k = 0; k < NUMBER_PLAYERS; k++)
        {
            printf("Money du joueur %d = %d\n", k, game.player[k].totalMoney);
        }
        return distributeBiggestBanknote(game, casino);
    }
}


void gameDisplay(GameState game)
{   
    printf("|| Round N° %d , Tour N° %d ||\n\n",game.round,game.turn);
    printf("Joueur N° 0 : %d Dés Restants, %d0k Money\n",game.player[0].dicesLeft,game.player[0].totalMoney);
    printf("Joueur N° 1 : %d Dés Restants, %d0k Money\n\n",game.player[1].dicesLeft,game.player[1].totalMoney);
    printf("| Il s'agit du tour du joueur %d |\n\n",game.playerTurn);

    int i; int j;

    for(i=0;i<6;i++)
    {
        printf("| CASINO N° %d : %d dés pour le joueur 0 , %d dés pour le joueur 1 |",game.casino[i].number,game.casino[i].dicesPlaced[0],
        game.casino[i].dicesPlaced[1]);
        printf("\n");
        bubbleTea(game.casino[i].associatedValues, MAX_BILLETS_PER_CASINO, 1);
        if (game.casino[i].associatedValues[1]) // Affichage terminal 
        {
            if (game.casino[i].associatedValues[2])
            {
                if (game.casino[i].associatedValues[3])
                {
                    if (game.casino[i].associatedValues[4])
                    {
                        printf("Valeur des billets : %d0k %d0k %d0k %d0k %d0k , ",game.casino[i].associatedValues[0]
                                                        ,game.casino[i].associatedValues[1]
                                                        ,game.casino[i].associatedValues[2]
                                                        ,game.casino[i].associatedValues[3]
                                                        ,game.casino[i].associatedValues[4]);
                    }
                    else
                    {
                        printf("Valeur des billets : %d0k %d0k %d0k %d0k , ",game.casino[i].associatedValues[0]
                                                        ,game.casino[i].associatedValues[1]
                                                        ,game.casino[i].associatedValues[2]
                                                        ,game.casino[i].associatedValues[3]);
                    }
                }
                else
                {
                    printf("Valeur des billets : %d0k %d0k %d0k , ",game.casino[i].associatedValues[0]
                                                        ,game.casino[i].associatedValues[1]
                                                        ,game.casino[i].associatedValues[2]);
                }
            }
            else
            {
                printf("Valeur des billets : %d0k %d0k , ",game.casino[i].associatedValues[0]
                                                        ,game.casino[i].associatedValues[1]);
            }
        }
        else
        {
            printf("Valeur des billets : %d0k , ",game.casino[i].associatedValues[0]);
        }


        printf("\n\n");
    }
    printf("Joueur N° %d, Tu as fait ce lancer : \n",game.playerTurn);

    bubbleTea(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft, 0);

    for(j=0;j<game.player[game.playerTurn].dicesLeft;j++)
        printf("%d ",game.player[game.playerTurn].currentThrow[j]);
    printf("\n\n");
}