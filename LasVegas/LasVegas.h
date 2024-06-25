#ifndef LASVEGAS_H
#define LASVEGAS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL>
#include <SDL2/SDL_image>

#define NUMBER_PLAYERS 2
#define NUMBER_DICES 8
#define NUMBER_ROUNDS 4

// Constantes des billets :
#define NUMBER_TOTAL_BILLET 48
#define NUMBER_BILLET_10k 5
#define NUMBER_BILLET_20k 7
#define NUMBER_BILLET_30k 7
#define NUMBER_BILLET_40k 5
#define NUMBER_BILLET_50k 6
#define NUMBER_BILLET_60k 5
#define NUMBER_BILLET_70k 5
#define NUMBER_BILLET_80k 4
#define NUMBER_BILLET_90k 4

typedef struct {
    int idPlayer; // l'ID du joueur : 0, 1 ...
    int dicesLeft; // Le nombre de dés qu'il lui reste à placer
    int totalMoney; // La somme qu'il a emmagasiné
    int currentThrow[NUMBER_DICES]; // Le lancer qu'il vient de faire
    int dicesChosen; // Le groupe de dés qu'il choisit de poser
} Players;

typedef struct {
    int number; // Entre 1 et 6.
    int associatedValues[]; // La liste des billets disposés sur ce
                            // casino.
    int dicesPlaced[NUMBER_PLAYERS]; // dicesPlaced[0] = 4
                                     // --> 4 dés sont placés
                                     // par le joueur 0 sur ce
                                     // casino.

} Casino;

typedef struct {
    Players player[NUMBER_PLAYERS];
    int playerTurn; // 0 ou 1 ou ...
    Casino casino[6];
    int round;
    bool roundFinished;
} GameState;

// Fonction d'initialisation
GameState initGame();
// Fonction d'initialisation du round
void initRound(GameState game);
// Fonction de tirage de billet
void throwBanknotes(GameState game);
// Fonnction d'un tirage de dés
void throwDices(GameState game);
// Fonction qui distribue les billets à la fin d'un round
void distributeMoney(GameState game);