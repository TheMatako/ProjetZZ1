#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MCTS.h"

Node_t * newNode()
{
    Node_t * newMove = malloc(sizeof(Node_t));
    if(newMove)
    {
        newMove->attendance = 0;
        newMove->sumGain = newMove->averageGain = newMove->potential = newMove->interest = 0;
        newMove->value = 0;
        newMove->next = NULL;
        return newMove;
    }
    return NULL;
}

List_Node * newList()
{
    List_Node * newList = malloc(sizeof(List_Node));
    newList->head = NULL;
    newList->length = 0;
    return newList;
}

List_Node * addList(List_Node * list,Node_t * adding)
{
    adding->next = list->head;
    list->head = adding;
    list->length++;
    return list;
}

void displayList(List_Node * List, hashTable * hash)
{
    Node_t * currentNode = List->head;
    while(currentNode)
    {
        printf("| %d %d %d %d %d %d %d %p %p|->",currentNode->value,currentNode->attendance,currentNode->sumGain,
                currentNode->averageGain,currentNode->potential,currentNode->interest,
                currentNode->currentGame.player[1].dicesChosen,hash->tab[currentNode->value],currentNode->next);
        currentNode = currentNode->next;
    }
    printf("FIN\n\n");
}

void freeList(List_Node * freeing)
{
    Node_t * currentNode = freeing->head;
    Node_t * precedentNode = currentNode;
    while(currentNode)
    {
        currentNode = currentNode->next;
        free(precedentNode);
        precedentNode = currentNode;
    }
    free(freeing);
}

hashTable * createHashTable()
{
    hashTable * newHash = malloc(sizeof(hashTable));

    if(newHash)
    {
        newHash->length = 10000;
        newHash->tab = malloc(newHash->length * sizeof(Node_t *));
        for (int i = 0; i < newHash->length; i++)
            newHash->tab[i] = NULL;
        return newHash;
    }
    return NULL;
}

int hashing(Node_t * hashed)
{
    int s1 = 0; int s2 = 0; int s3 = 0; int s4; int s5; int value;
    for(int i = 0 ; i<5 ; i++)
    {
        for(int j = 0 ; j < MAX_BILLETS_PER_CASINO ; j++)
            s1+=hashed->currentGame.casino[i].associatedValues[j];
        for(int k = 0 ; k < NUMBER_PLAYERS ; k++)
            s2+=hashed->currentGame.casino[i].dicesPlaced[k];
    }
    for(int n = 0 ; n < hashed->currentGame.player[1].dicesLeft ; n++)
        s3+=hashed->currentGame.player[1].currentThrow[n]; // un peu dangereux de mettre juste 1
    s4 = hashed->currentGame.player[1].dicesLeft;
    s5 = hashed->currentGame.player[1].dicesChosen;
    value = 13*s1 + 17*s2 + 19*s3 + 23*s4 + 29*s5 ;
    return value;
}

hashTable * addToHashTable(hashTable * hTable, Node_t * added)
{
    int value = hashing(added);
    if(!hTable->tab[value])
    {
        added->value = value;
        hTable->tab[added->value] = added;
    }
    // printf("VALUE : %d , CONSULTING HASH : %p\n",added->value,hTable->tab[added->value]);
    return hTable;
}

bool isPresentNode(hashTable * hTable, Node_t * Node)
{
    bool Ok = false;
    int value = hashing(Node);
    if(hTable->tab[value])
        Ok = true;
    return Ok;
}

void freeHashTable(hashTable * hash) 
{
    if(hash)
    {
        if(hash->tab)
        {
            for(int i = 0; i < hash->length; i++)
            {
                Node_t * current = hash->tab[i];
                while(current)
                {
                    Node_t * temp = current;
                    current = current->next;
                    free(temp);
                }
            }
            free(hash->tab);
        }
        free(hash);
    }
}

int compare(const void* a, const void* b) {return (*(int*)a - *(int*)b);}
    
void removeDuplicates(int * array, int * length)
{   
    int MAX_VAL = 10000;
    if (*length == 0)
        return;
    bool seen[MAX_VAL];
    for (int i = 0; i < MAX_VAL; i++)
        seen[i] = false;
    int uniqueIndex = 0;
    for (int i = 0; i < *length; i++)
    {
        if (!seen[array[i]])
        {
            seen[array[i]] = true;
            array[uniqueIndex++] = array[i];
            // printf("%d",array[i]);
        }
    }

    *length = uniqueIndex;

    bubbleTea(array,*length,0);
    for(int j=0 ; j<*length ; j++)
        printf("%d ",array[j]);
    printf("\n");
}

GameState applyOneTurn(GameState game,int dice)
{ // Applique un seul tour, FAIRE UN THROWDICES AVANT

    int group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
    game.player[game.playerTurn].dicesLeft -= group;
    game.player[game.playerTurn].dicesChosen = dice;
    game.casino[dice-1].dicesPlaced[game.playerTurn] += group;
    game.turn++;
    if(game.player[0].dicesLeft <= 0 && game.player[1].dicesLeft <= 0)
    {
        game.roundFinished = true;
        game = distributeMoney(game);
        gameDisplay(game);
    }
    game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
    return game;
}

GameState applyOneTurnRandom(GameState game)
{ // Applique un seul tour, FAIRE UN THROWDICES AVANT
    game = throwDices(&game);
    if(game.player[game.playerTurn].dicesLeft)
    {
        int dice = 100; int group = 0; // game = throwDices(&game);
        while(group == 0)
        {
            dice = rand()%6 + 1;
            group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
        }
        game.player[game.playerTurn].dicesLeft -= group;
        game.player[game.playerTurn].dicesChosen = dice-1;
        game.casino[dice-1].dicesPlaced[game.playerTurn] += group;
        game.turn++;
    }
    game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
    if(game.player[0].dicesLeft <= 0 && game.player[1].dicesLeft <= 0)
    {
        game.roundFinished = true;
        game = distributeMoney(game);
        gameDisplay(game);
    }

    return game;
}

int simulation(GameState game,int profit,int player)
{
    if(!game.roundFinished)
    {
        game = throwDices(&game);
        // gameDisplay(game);
        if(game.player[game.playerTurn].dicesLeft)
        {
            int dice = 100; int group = 0; // game = throwDices(&game);
            while(group == 0)
            {
                dice = rand()%6 + 1;
                group = occurrences(game.player[game.playerTurn].currentThrow,game.player[game.playerTurn].dicesLeft,dice);
            }
            game.player[game.playerTurn].dicesLeft -= group;
            game.player[game.playerTurn].dicesChosen = dice-1;
            game.casino[dice-1].dicesPlaced[game.playerTurn] += group;
            game.turn++;
        }
        game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
        if(game.player[0].dicesLeft <= 0 && game.player[1].dicesLeft <= 0)
        {
            game.roundFinished = true;
            game = distributeMoney(game);
            // gameDisplay(game);
        }

        return simulation(game,profit,player);
    }

    profit = game.player[player].totalMoney;
    return profit;
}

List_Node * listing_And_Simulating_Moves(GameState game, hashTable * hash, int interestPlayer, int N)
{
    List_Node * list = newList(); // Contiendra tout les noeuds visités
    int s; // Sera la valeur de simulation
    int i; int j;
    int group;
    int length = game.player[game.playerTurn].dicesLeft;
    int throw[length];
    memcpy(throw,game.player[game.playerTurn].currentThrow,(length) * sizeof(int));
    removeDuplicates(throw,&length);
    GameState intermediate = game; // Une copie du jeu pour ne pas modifier l'original

    if(list)
    {
        for(i = 0; i<6; i++)
        {
            for(j = 0; j<length; j++)
            {
                if(throw[j] == i + 1 /* && !isPresentNode() */)
                {
                    Node_t * node = newNode(); // Nouveau noeud !

                    // La suite n'est que l'application d'un tour classique

                    group = occurrences(intermediate.player[game.playerTurn].currentThrow,intermediate.player[game.playerTurn].dicesLeft,i);
                    intermediate.player[game.playerTurn].dicesLeft -= group;
                    intermediate.casino[i+1].dicesPlaced[game.playerTurn] += group;
                    intermediate.player[interestPlayer].dicesChosen = i+1;
                    intermediate.turn++;
                    game.playerTurn = (game.playerTurn+1)%NUMBER_PLAYERS;
                    if(intermediate.player[0].dicesLeft <= 0 && intermediate.player[1].dicesLeft <= 0)
                    {
                        intermediate.roundFinished = true;
                        intermediate = distributeMoney(game);
                    }

                    // fin du tour classique

                    node->currentGame = intermediate;
                    s = simulation(intermediate,0,interestPlayer);
                    N++;
                    node->attendance++;
                    node->sumGain += s;
                    node->averageGain = node->sumGain/node->attendance;
                    node->potential = C*sqrt(log(N/node->attendance));
                    node->interest = node->averageGain + node->potential;
                    // printf("HASHING : %d\n",node->value);
                    hash = addToHashTable(hash,node);
                    // printf("AXED : %p\n",hash->tab[node->value]);
                    list = addList(list,node);
                }
            }

            intermediate = game;
        }
    }
    return list;
}

Node_t * bestActualMove(List_Node * list)
{
    Node_t * currentNode = list->head;
    Node_t * bestNode = NULL;
    int bestInterest = 0;
    while(currentNode)
    {
        if(currentNode->interest > bestInterest)
        {
            bestNode = currentNode;
            bestInterest = currentNode->interest;
        }
        currentNode = currentNode->next;
    }
    return bestNode;
}

Node_t * MCTS(GameState game, hashTable * hash, int interestPlayer,int N)
{ //  Mettre une COPIE du game en argument
    List_Node * moves = listing_And_Simulating_Moves(game,hash,interestPlayer,N);
    Node_t * bestNode = NULL;
    GameState intermediate;
    int s;
    time_t timing = time(0);

    while(difftime(time(0),timing) < 2)
    {
        bestNode = bestActualMove(moves);
        intermediate = bestNode->currentGame;
        s = simulation(intermediate,0,intermediate.playerTurn);
        N++;
        bestNode->attendance++;
        bestNode->sumGain += s;
        bestNode->averageGain = bestNode->sumGain/bestNode->attendance;
        bestNode->potential = C*sqrt(log(N/bestNode->attendance));
        bestNode->interest = bestNode->averageGain + bestNode->potential;
    }
    return bestNode;
}

