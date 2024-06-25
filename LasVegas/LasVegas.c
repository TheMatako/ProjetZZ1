
#include "LasVegas.h"
#include <stddef.h>
#include <stdlib.h> 

GameState initGame() {
    GameState game;
    game.playerTurn = 0;
    game.round = 0;
    game.roundFinished = true;

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


Casino initCasino(int number) {
    Casino casino;
    casino.number = number;
    memset(casino.associatedValues, 0, MAX_BILLETS_PER_CASINO * sizeof(int)); // Initialize the billets to 0
    memset(casino.dicesPlaced, 0, NUMBER_PLAYERS * sizeof(int)); // Initialize the dice placed to 0
    return casino;
}

  


void initRound(GameState game){
    game.round += 1;
    game.playerTurn = game.round % 2;
    game.roundFinished = !game.roundFinished; 
    
}
/*void throwBanknotes(GameState game){

}
*/


void throwDices(GameState *game) {
    for (int i = 0; i < game->player[game->playerTurn].dicesLeft; i++) {
        int value = rand() % 6 + 1;  // Generates a random number between 1 and 6
        game->player[game->playerTurn].currentThrow[i] = value;
    }
    //game->player[game->playerTurn].dicesLeft = ;  // All dices have been thrown
}


/* void distributeMoney(GameState game){

}
*/

