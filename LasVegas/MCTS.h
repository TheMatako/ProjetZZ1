#ifndef MCTS_H
#define MCTS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Constantes pour les formules
#define C sqrt(2)
#define EPSILON 0.3
#define NUMBER_PLAYED_GAMES 10^5

// On suppose que seuls 6 coups sont possibles, car nous choisissons parmi 6 groupes de dés ou moins
#define NUMBER_POSSIBLE_MOVES 6
#define POSSIBLE_MOVES {1,2,3,4,5,6}

// Structure d'un noeud
typedef struct {

    Casino; // On a besoin des placements des billets, des placements des dés des joueurs

    Players chosenMove; // On aura besoin de dicesChosen, (dices left, currentThrow)

    int * value; // la fameuse valeur de la fonction hachage, elle est UNIQUE

    int attendance; // nombre de fois que le noeud a été visité (n)
    int averageGain; // Moyenne des simulations effectuées sur ce noeud (G)
    int potential; // Voir la formule, dépend de C
    int interest; // La valeur d'Intérêt, voir formule

    Node * next;
} Node;

// On stockera les noeud dans une table de hachage
typedef struct {
    int lenght;
	Node ** tab;
} hashTable;

// Si il y a collision, on fera une lise chaînée (normalement on en a pas besoin)
typedef struct {
    Node * head;
    int lenght;
} List_Node;

// Fonction qui créer un noeud, et seulement s'il s'agit d'un nouvel état
Node newNode(Gamestate newState);
// Fonction qui créer une liste de noeud, ne sert uniquement qu'en cas de collision
List_Node newList();
// Fonction qui ajoute un noeud à une liste
List_Node addList(Node adding);
// Fonction qui libére une liste
void freeList(List_Node freeing);

// Fonction qui créer une liste de table de hachage
hashTable createHashTable();
// LA FONCTION (!!) de hachage
int hashing(Node hashed);
// Fonction qui ajoute un noeud à la table de hachage, fait appel à la fonction HASHING et gère les collisions
void addToHashTable(hashTable Table,Node added);
// Fonction vérifie si un noeud est déjà dans la table de hachage
bool isPresentNode(hashTable hTable,Node vNode);

// Fonction qui compare si deux noeuds sont exactement les mêmes
bool nodeCompare(Node node1, Node node2)
// Fonction qui simule une fin de partie à partir d'un état (attention à l'alternance des joueurs)
int simulation(Node choice);
// Fonction UCB
Coup UCB(GameState);

// Fonction MCTS
/* Ce sera comme un main */