#ifndef MCTS_H
#define MCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LasVegas.h"

// Constantes pour les formules
#define C sqrt(2)
#define EPSILON 0.3
#define NUMBER_PLAYED_GAMES 10^5

// On suppose que seuls 6 coups sont possibles, car nous choisissons parmi 6 groupes de dés ou moins
#define NUMBER_POSSIBLE_MOVES 6
#define POSSIBLE_MOVES {1,2,3,4,5,6}

// Structure d'un noeud
typedef struct Node {
    GameState currentGame; // On aura besoin des placements des billets, des placements des dés des joueurs
                           // des dés qu'il nous reste et de notre tir (les sommes)
    
    int value; // la fameuse valeur de la fonction hachage, elle est UNIQUE

    int attendance; // nombre de fois que le noeud a été visité (n)
    int averageGain; // Moyenne des simulations effectuées sur ce noeud (G)
    int potential; // Voir la formule, dépend de C
    int interest; // La valeur d'Intérêt, voir formule

    struct Node * next;
} Node_t;

// On stockera les noeud dans une table de hachage
typedef struct List_Node {
    int length;
	Node_t ** tab;
} hashTable;

// Si il y a collision, on fera une lise chaînée (normalement on en a pas besoin)
typedef struct {
    struct Node * head;
    int length;
} List_Node;

// Fonction qui créer un noeud, et seulement s'il s'agit d'un nouvel état
Node_t * newNode();
// Fonction qui créer une liste de noeud, ne sert uniquement qu'en cas de collision
List_Node * newList();
// Fonction qui ajoute un noeud à une liste
List_Node * addList(List_Node * List,Node_t * adding);
// Fonction qui affiche une liste
void displayList(List_Node * List, hashTable * hash);
// Fonction qui libére une liste
void freeList(List_Node * freeing);

// Fonction qui créer une liste de table de hachage
hashTable * createHashTable();
// LA FONCTION (!!) de hachage
int hashing(Node_t * hashed);
// Fonction qui ajoute un noeud à la table de hachage, fait appel à la fonction HASHING et gère les collisions
hashTable * addToHashTable(hashTable * hTable, Node_t * added);
// Fonction vérifie si un noeud est déjà dans la table de hachage
bool isPresentNode(hashTable * hTable, Node_t * Node);
// Fonction qui libére une table de hachage
void freeHashTable(hashTable * hash);

// J'avoue je sais pas trop mais ça a l'air intéressant à comprendre
int compare(const void * a, const void * b);
// Fonction qui supprime toutes les répétitions, agit directement sur le tableau
void removeDuplicates(int * array, int * length);

// Fonction qui applqiue tout simplement un seul tour
GameState applyOneTurn(GameState game);
// Fonction qui simule une fin de partie à partir d'un état (attention à l'alternance des joueurs)
int simulation(GameState game,int profit,int player);
// Fonction qui liste les Noeuds possibles et les simule une seule fois, faire une throwDice() et l'appliquer au jeu AVANT
List_Node * listing_And_Simulating_Moves(GameState game, hashTable * hash, int player);
// Fonction MCTS
void MCTS();

#endif