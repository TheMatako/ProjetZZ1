#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "LasVegas.h"

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
    memset(casino.dicesPlaced, 0, NUMBER_PLAYERS * sizeof(int)); // Initialiser les dés à 0 0
    return casino;
}

void initRound(GameState game)
{
    game.round += 1;
    game.playerTurn = game.round % 2;
    game.roundFinished = !game.roundFinished; 
}

/* int randBanknotes(GameState game)
{
    int r=rand()%game.bank
}
    

void throwBanknotes(Gamestate * game) 
{
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
    for (int i = 0; i < game->player[game->playerTurn].dicesLeft; i++) 
    {
        int value = rand() % 6 + 1;  //générer un nombre entre 1 et 6 (reprédente les faces des dés)
        game->player[game->playerTurn].currentThrow[i] = value;
    }
}

/* void distributeMoney(GameState game){

}
*/

void gameDisplay(GameState game)
{
    printf("| Il s'agit du tour de %d :\n" game.playerTurn);

    int i=0;
    for(i=0;i<=5;i++)
    {
        printf("| ")
    }
}