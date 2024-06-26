#ifndef LASVEGAS_H
#define LASVEGAS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
#define MAX_BILLETS_PER_CASINO 5 // si jamais on pioche 5 billets de 10k
                                 // sinon on a besoin de moins de 5 billets
                                 // pour que leur somme dépasse 50k

typedef struct {
    int idPlayer; // l'ID du joueur : 0, 1 ... --> corrélé à une couleur, 0: bleu, 1: jaune, 2: rouge, 3: vert, 4: violet 
    int dicesLeft; // Le nombre de dés qu'il lui reste à placer
    int totalMoney; // La somme qu'il a emmagasiné
    int currentThrow[NUMBER_DICES]; // Le lancer qu'il vient de faire
    int dicesChosen; // Le groupe de dés qu'il a choisi
                     // Le casino qu'il a choisi
} Players;

typedef struct {
    int number; // Entre 1 et 6.
   
    int dicesPlaced[NUMBER_PLAYERS]; // dicesPlaced[0] = 4
                                     // --> 4 dés sont placés
                                     // par le joueur 0 sur ce
                                     // casino.
    int associatedValues[MAX_BILLETS_PER_CASINO]; // La liste des billets disposés sur ce casino.
    int rectCasino[4];
    int rectDice[4];
} Casino;

typedef struct {
    Players player[NUMBER_PLAYERS];
    int playerTurn; // 0 ou 1 ou ...
    Casino casino[6];
    int round;
    int turn;
    bool roundFinished;
    int Banknotes[10];
} GameState;

// Fonction d'Initialisation du Jeu
GameState initGame();
// Fonction d'Initialisation un Casino
Casino initCasino(int number);
// Fonction d'Initialisation du Round
GameState initRound(GameState game);
// Fonction qui génère un billet aléatoire respectant les probabilitées individuelles
int randBankNotes(GameState * game);

// Fonction qui renvoit la somme d'une Liste
int sumList(int Tab[],int lenght);
// Fonction qui renvoit la POSITION du maximum d'une Liste
int max(int Tab[],int lenght);
// Fonction qui vérifie si une liste contient un doublon
bool doublons(int Tab[],int lenght);
// La Fonction du tri à bulle
void bubbleTri(int tab[],int lenght);
// Fonction qui compte l'occurence d'une valeur ans une liste
int occurrences(int tab[],int lenght,int number);

// Fonction de tirage de billet
GameState throwBanknotes(GameState game);
// Fonction d'un tirage de dés
GameState throwDices(GameState * game);

// Fonction qui distribue les billets à la fin d'un round
GameState distributeMoney(GameState game);

// Affiche le jeu
void gameDisplay(GameState game);

#endif