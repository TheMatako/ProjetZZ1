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

int simulation(GameState game) // , Node choice)
{   
    srand(time(NULL));
    int r1 = -1;
    int r2 = -1;
    Node intermediateNode = state; // Jeu qui sera mis à jour, nous le gardons pas
    // Node intermediateGame = choice; // Un noeud qui sera mis à jour au fur et à mesure, on ne le gardera pas
    while(!choice.GameState.roundFinished)
    {
        if(game.roundFinished)
        {
            switch(game.playerTurn)
            {
                case 0 :
                    return -game.player[game.playerTurn].totalMoney;
                    break;
                case 1 :
                    return game.player[game.playerTurn].totalMoney;
                    break;
                default :
                    return game.player[game.playerTurn].totalMoney;
            }   
        }
        else
        {
            throwDices(GameState);
            while(!game.player[game.playerTurn].currentThrow[r])
                r1 = rand()%6+1;
            r2 = rand()%6+1;
            state.player[game.playerTurn].dicesLeft -= game.player[game.playerTurn].currentThrow[r];
            game.player[game.playerTurn].dicesChosen = r1;
            game.player[game.playerTurn].casinoChosen = r2;
            game.casino[r2].dicesPlaced[game.player] += currentThrow[r];

            if(!game.player[0].dicesLeft && !game.player[1].dicesLeft)
            {
                game.roundFinished = true;
            }
        }

    }

    free(Intermediate);

    return // valeur gagnée
}

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
                if(game.roundFinished)
                {

                }

        }
    }
}