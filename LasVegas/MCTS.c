#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LasVegas.h"
#include "MCTS.h"

Node newNode(GameState newState)
{
    Node * newMove = malloc(sizeof(noeud));

    if(newMove)
    {
        Node->attendance = 1;
        return *noeud;
    }
    return NULL;
}

List_Node generer_Noeuds(GameState currentGame);
{
    List_Node * newList = malloc(sizeof(List_Node));

    if(newList)
        return *newList;
    return NULL;
}

List_Node addList(Node adding)
{}

void freeList(List_Node freeing)
{}

hashTable createHashTable()
{
    hashTable * newHash = malloc(sizeof(hashTable));

    if(newHash)
    {
        newHash->lenght = 10^6;
        return *newHash;
    }
    return NULL;
}

int hashing(Node hashed);
{   
    if(hashed)
    {
        int value = (17 * hashed.averageGain) + (31 * potential) + (43 * interest);
        return value;        
    }
    return 0;
}

void addToHashTable(hashTable Table, Node added)
{
    int value = hashing(added);

    if(!hashTable[value])
        hashTable[value] = &added;

    return NULL;
}

bool isPresentNode(hashTable hTable, Node vNode)
{
    bool 0 = false;
    int value = hashing(vNode);
    if(hTable.tab[value])
        O = true;
    return O;
}

// int simulation(Node choice)
// {
        // Node Intermediate = choice; // Un noeud qui sera mis à jour au fur et à mesure, on ne le gardera pas
//     while(!choice.GameState.roundFinished)
//     {
//         // on déroule un tour du joueur, donc un lancé de dés + un posage de dés au hasard
        
//         // if(tout le monde a posé ses dés)
//             choice.GameState.roundFinished = true;
//         else
//         {
//             choice.GameState.
//         }
        
//     }

    // free(Intermediate);

//     return // valeur gagnée
// }

int main()
{
    GameState game = initGame();
    int N = NUMBER_PLAYED_GAMES;

    while(N >= 0)
    {
        GameState Intermediate = game;

        switch(game.playerTurn)
        {
            case 0 :
                srand(time(NULL));
                int r = rand()%6


        }
    }
} // main = MCTS