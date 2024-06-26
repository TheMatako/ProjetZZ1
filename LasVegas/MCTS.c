#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MCTS.h"

Node newNode()
{
    Node * newMove = malloc(sizeof(Node));
    if(newMove)
    {
        newMove->attendance = 1;
        return *newMove;
    }
}

List_Node newList()
{
    List_Node * newList = malloc(sizeof(List_Node));
    if(newList)
    {
        newList->head = 0;
        newList->lenght = 0;
        return *newList;
    }
}

// List_Node addList(Node adding) {}

// void freeList(List_Node freeing) {}

hashTable createHashTable()
{
    hashTable * newHash = malloc(sizeof(hashTable));

    if(newHash)
    {
        newHash->lenght = 10^6;
        return *newHash;
    }
}

int hashing(Node hashed)
{
    int value = (17 * hashed.averageGain) + (31 * hashed.potential) + (43 * hashed.interest);
    return value;
}

void addToHashTable(hashTable hTable, Node added)
{
    int value = hashing(added);
    if(!hTable.tab[value])
        hTable.tab[value] = &added;
}

bool isPresentNode(hashTable hTable, Node vNode)
{
    bool O = false;
    int value = hashing(vNode);
    if(hTable.tab[value])
        O = true;
    return O;
}

int simulation(GameState game) // , Node choice)
{   
    srand(time(0));
    int r1 = -1;
    int r2 = -1;
    // GameState intermediateGame = game; // Copie du jeu qui sera mis à jour, nous le gardons pas
    // Node intermediateGame = choice; // Un noeud qui sera mis à jour au fur et à mesure, on ne le gardera pas
    if(!game.roundFinished)
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
                break;
        }
    }
    else
    {
        throwDices(&game);
        while(!game.player[game.playerTurn].currentThrow[r1])
            r1 = rand()%6+1;
        r2 = rand()%6+1;
        game.player[game.playerTurn].dicesLeft -= game.player[game.playerTurn].currentThrow[r1];
        game.player[game.playerTurn].dicesChosen = r1;
        game.player[game.playerTurn].casinoChosen = r2;
        game.casino[r2].dicesPlaced[game.playerTurn] += game.player[game.playerTurn].currentThrow[r1];

        if(!game.player[0].dicesLeft && !game.player[1].dicesLeft)
            game.roundFinished = true;
        else
        {
            if(game.playerTurn == 0)
                game.playerTurn = 1;
            else
                game.playerTurn = 0;

            return simulation(game);  
        }
    }
}

int main()
{
    // GameState game = initGame();
    // game = throwBanknotes(game);
    // game = throwDices(&game);
    
    // gameDisplay(game);

    // int N = NUMBER_PLAYED_GAMES;
    // printf("Simulation : %d\n",simulation(game));

    // while(N >= 0)
    // {
    //     GameState Intermediate = game;

    //     switch(game.playerTurn)
    //     {
    //         case 0 :
    //             if(game.roundFinished)
    //             {

    //             }

    //     }
    // }
}