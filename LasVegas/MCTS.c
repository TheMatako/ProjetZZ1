#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL>
#include <SDL2/SDL_image>

#include "MCTS.h"

Node newNode(Gamestate newState)
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
        return newHash;
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

void MCTS()
{
    int
}