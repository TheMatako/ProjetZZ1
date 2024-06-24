#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structureQuoridor.h"



Liste_Coups_t * Creer_Liste_Coups()
{ // Initialise une Liste de Coups (Liste Chaînée)
    Liste_Coups_t * NewL = malloc(sizeof(Liste_Coups_t));

    if(NewL)
    {
        NewL->Tete = NULL; NewL->Queue = NULL; 
        NewL->Longueur =  0; 
        return NewL;
    }

    return NULL;
}

Liste_Coups_t * Ajouter_Coup_Liste(Liste_Coups_t * L, int xp, int yp ,int xb ,int yb ,int H)
{ // Ajoute un coup à une liste de Coups, il faut définir 
  // la nouvelle position (xp;yp) et la nouvelle barrière (xb;yb)
    Coup_t * NewC = malloc(sizeof(Coup_t));
    Position * NewP = malloc(sizeof(Position));
    Barrier * NewB = malloc(sizeof(Barrier));
    Position * NewPB1 = malloc(sizeof(Position));
    Position * NewPB2 = malloc(sizeof(Position));

    if(NewC && NewP && NewB && NewPB1 && NewPB2)
    {
        NewP->x = xp; NewP->y = yp;
        NewC->NewPos = NewP;

        NewPB1->x = xb; NewPB1->y = yb;

        if(H) 
        { 
            NewPB2->x = xb+1; 
            NewPB2->y = yb;
        }
        else 
        {
            NewPB2->x = xb; 
            NewPB2->y = yb-1;
        }
            
        NewB->pos1 = *NewPB1;
        NewB->pos2 = *NewPB2;
        NewB->isHorizontal = H;

        NewC->NewBar = NewB;

        if (L->Tete == NULL)
        {  // Cas où la liste est vide
            L->Tete = NewC;
            L->Queue = NewC;
        }
        else 
        {  // Cas où la Le n'est pas vide
            L->Queue->Suiv = NewC;
            NewC->Prec = L->Queue;
            L->Queue = NewC;
        }
    }

    return L;
}

void Affichage_Liste_Coups(Liste_Coups_t * L) 
{ // Affiche une liste de Coups
    if(L->Tete)
    {
        Coup_t * Courant = L->Tete;
        Coup_t * Dernier = Courant;

        printf("| %d %d %d %d %d | -> "/* Courant->Prec*/,Courant->NewPos->x,Courant->NewPos->y,
                                        Courant->NewBar->pos1.x,Courant->NewBar->pos1.y,
                                        Courant->NewBar->isHorizontal/*Courant->Suiv*/);

        Dernier = Courant;
        Courant = Courant->Suiv;

        while (Courant && Courant != Dernier)
        {
            printf("| %d %d %d %d %d | -> "/* Courant->Prec*/,Courant->NewPos->x,Courant->NewPos->y,
                                        Courant->NewBar->pos1.x,Courant->NewBar->pos1.y,
                                        Courant->NewBar->isHorizontal/*Courant->Suiv*/);
            Courant = Courant->Suiv;
        } 
    }

    printf("FIN\n");
}

Liste_Coups_t * Suppression_Tete(Liste_Coups_t * L)
{ // Supprime le premier élément d'une Liste de Coups
    if (L->Tete) 
    {
        Coup_t * Temp = L->Tete;
        L->Tete = L->Tete->Suiv;

        if (L->Tete != NULL)
            L->Tete->Prec = NULL;
        else
            L->Queue = NULL;

        free(Temp);
    }

    return L;
}

void Free_Liste(Liste_Coups_t * L) 
{ // Libère entièrement une Liste de Coups
    Coup_t * Courant = L->Tete;
    Coup_t * NextCoup ;

    while(Courant) 
    {
        NextCoup = Courant->Suiv;
        free(Courant);
        Courant = NextCoup ;
    }

    L->Tete= NULL;
}

int Compare_Place(Position * Pos1,Position * Pos2)
{ // Observe simplement si deux positions sont les mêmes. 
  // 0 : Non , 1 : Oui
    if(Pos1->x == Pos2->x && Pos1->y == Pos2->y)
        return 1;
    return 0;
}

int Is_Diagonal_or_Simple_Moove(Position * Previous, Position * Next)
{ // 0 : Trajet Vertical , 1 : Trajet Horizontal , -1 : Trajet Diagonal
    if(Previous->y + 1 == Next->y && Previous->x + 1 == Next->x)
        return -1;
    else if(Previous->y + 1 == Next->y && Previous->x - 1 == Next->x)
        return -1;
    else if(Previous->y - 1 == Next->y && Previous->x + 1 == Next->x)
        return -1;
    else if(Previous->y - 1 == Next->y && Previous->x - 1 == Next->x)
        return -1;

    if(Previous->y + 1 == Next->y || Previous->y - 1 == Next->y)
        return 0;
    else if(Previous->x + 1 == Next->x || Previous->x - 1 == Next->x)
        return 1;
    return 2;
}

// Fonction d'évaluation
int evaluate(GameState game)
{
    // Distance pion joueur à la ligne du haut 
    int player1Distance = game.players[0].pos.y;
    // Distance pion ordi à la ligne du bas
    int player2Distance = BOX_NUMBER_LINE - game.players[1].pos.y;

    if (game.playerTurn == 0)
    {
        if (player1Distance == 0) return -1000; // On retourne une valeur très petite car le joueur à gagné
        else return player1Distance - player2Distance; // On retourne la différence de proximité entre les deux joueurs à leur case victorieuse
    }
    else 
    {
        if (player2Distance == 0) return 1000; // Là c'est l'ordi qui gagne
        else return player2Distance - player1Distance;
    }
}

bool Is_There_An_Obstacle(GameState * jeu, Position * Previous, Position * Next)
{ // Observe si un déplacement entre la position Previous et la position Next est possible
  // True : Il y a un obstacle , False : Aucun Obstacle
    int i; bool O = false;

    if((Next->y >= 0) && ((Next->x >= 0)) && (Next->y < BOX_NUMBER_LINE) && (Next->x < BOX_NUMBER_COLUMN) 
    && !Compare_Place(Next,&jeu->players[0].pos) && !Compare_Place(Next,&jeu->players[1].pos))
    { // Nous ne sortons pas du plateau, et ne rentrons pas en colision avec l'autre joueur
        int Direction = Is_Diagonal_or_Simple_Moove(Previous,Next);

        switch(Direction)
        {
            case 0 :

                for(i=0;i<=19;i++)
                {   
                    if(jeu->barriers[i].isHorizontal == 1)
                    {
                        if(Previous->y + 1 == Next->y && (jeu->barriers[i].pos1.x == Next->x || jeu->barriers[i].pos1.x == Next->x - 1) && jeu->barriers[i].pos1.y == Next->y)
                            O = true;
                        else if(Previous->y - 1 == Next->y && (jeu->barriers[i].pos1.x == Next->x || jeu->barriers[i].pos1.x == Next->x - 1) && jeu->barriers[i].pos1.y == Previous->y)
                            O = true;
                    }
                }
                break;

            case 1 :

                for(i=0;i<=19;i++)
                {
                    if(jeu->barriers[i].isHorizontal == 0)
                    {
                        if(Previous->x + 1 == Next->x && (jeu->barriers[i].pos1.y == Previous->y || jeu->barriers[i].pos1.y == Previous->y + 1) && jeu->barriers[i].pos1.x == Previous->x)
                            O = true;
                        else if(Previous->x - 1 == Next->x && (jeu->barriers[i].pos1.y == Next->y || jeu->barriers[i].pos1.y == Next->y + 1) && jeu->barriers[i].pos1.x == Next->x)
                            O = true;
                    }
                }
                break;

            /* case -1 :

                if(Previous->y + 1 == Next->y && Previous->x + 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->barriers[i].isHorizontal)
                        {
                            if((jeu->barriers[i].pos1.x == Previous->x || jeu->barriers[i].pos1.x == Previous->x-1) && jeu->barriers[i].pos1.y == Next->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos1.y == Previous->y|| jeu->barriers[i].pos1.x == Previous->y+1) && jeu->barriers[i].pos1.x == Next->x)
                                O = true;
                        }
                    }
                }
                else if(Previous->y - 1 == Next->y && Previous->x + 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->barriers[i].isHorizontal)
                        {
                            if((jeu->barriers[i].pos1.x == Previous->x || jeu->barriers[i].pos1.x == Previous->x+1) && jeu->barriers[i].pos1.y == Previous->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos1.y == Previous->y|| jeu->barriers[i].pos1.y == Previous->y-1) && jeu->barriers[i].pos1.x == Previous->x)
                                O = true;
                        }
                    }
                }
                else if(Previous->y + 1 == Next->y && Previous->x - 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->barriers[i].isHorizontal)
                        {
                            if((jeu->barriers[i].pos1.x == Next->x || jeu->barriers[i].pos1.x == Next->x-1) && jeu->barriers[i].pos1.y == Next->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos1.y == Previous->y|| jeu->barriers[i].pos1.y == Previous->y+1) && jeu->barriers[i].pos1.x == Next->x)
                                O = true;
                        }
                    }
                }
                else if(Previous->y - 1 == Next->y && Previous->x - 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->barriers[i].isHorizontal)
                        {
                            if((jeu->barriers[i].pos1.x == Previous->x || jeu->barriers[i].pos1.x == Previous->x-1) && jeu->barriers[i].pos1.y == Previous->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos1.y == Previous->y|| jeu->barriers[i].pos1.y == Previous->y-1) && jeu->barriers[i].pos1.x == Next->x)
                                O = true;
                        }
                    }
                }
                break;*/

                default :
                    break;
        }
    }
    else
        O = true;

    return O;
}





Liste_Coups_t * Generer_Coup(GameState jeu, int Joueur)
{
    
    // Il vaut mieux que L soit vide
    int i; int j;

    Liste_Coups_t * L = Creer_Liste_Coups();

    int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE];
    getPositionPlayable(jeu, jeu.players[Joueur].pos.x, jeu.player[Joueur].pos.y, boxesPlayable);
    for (int i = 0; i < BOX_NUMBER_COLUMN; i++)
    {
        for (int j = 0; j < BOX_NUMBER_LINE; j++)
        {
            
            if (boxesPlayable[i][j] == 1)
            {
                L = Ajouter_Coup_Liste(L, i, j, 0, 0, 0);
            }
        }
    }
    
    return L;
}

/*
Deuxième version avec déplacement non concluant mais idées poru les barrières
Liste_Coups_t * Generer_Coup(GameState * jeu, int Joueur)
{   // Renvoit la liste des coups possibles à partir d'une position de jeu, en fonction du joueur
    //  La fameuse fonction, ma plus grande fierté.
    if(!EndGame(jeu,Joueur))
    {   
        int i; int j;

        Liste_Coups_t * L = Creer_Liste_Coups();

        // On change de position

        Position Place = jeu->players[Joueur].pos;
        Position Nouvelle_Position = Place;

        Nouvelle_Position.y = Nouvelle_Position.y + 1;

        if(!Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller en avant */
            L = Ajouter_Coup_Liste(L,Place.x,Place.y + 1,0,0,0);
        
        Nouvelle_Position.y = Nouvelle_Position.y - 2;

        if (!Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller en arrière*/
            L = Ajouter_Coup_Liste(L,Place.x,Place.y - 1,0,0,0);

        Nouvelle_Position.y = Nouvelle_Position.y + 1;
        Nouvelle_Position.x = Nouvelle_Position.x - 1;
        
        if (!Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller à Gauche */
            L = Ajouter_Coup_Liste(L,Place.x - 1,Place.y,0,0,0);
        
        Nouvelle_Position.x = Nouvelle_Position.x + 2;

        if (!Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller à droite*/
            L = Ajouter_Coup_Liste(L,Place.x + 1,Place.y,0,0,0);
        
        // On préfère poser une barrière

        if(jeu->players[Joueur].barriersLeft)
            {
                int Occupees[8][8];

                for(i=0;i<=7;i++)
                {
                    for(j=0;i<=7;i++)
                        Occupees[i][j] = 0;
                }

                for(i=0;i<=19;i++)
                {
                    if(jeu->barriers[i].isPlaced)
                    {
                        Occupees[jeu->barriers[i].pos1.x][jeu->barriers[i].pos1.y] = 1;
                        Occupees[jeu->barriers[i].pos2.x][jeu->barriers[i].pos2.y] = 1;
                    }
                }

                for(i=0;i<=7;i++)
                {
                    for(j=0;j<=7;j++)
                    {
                        if((i+1) < 8 && (j-1) >= 0)
                        {
                            if(Occupees[i][j] != 1 && Occupees[i][j-1] != 1)
                                L = Ajouter_Coup_Liste(L,0,0,i,j,0);
                            if(Occupees[i][j] != 1 && Occupees[i+1][j] != 1)
                                L = Ajouter_Coup_Liste(L,0,0,i,j,1);
                        }
                    }
                    
                }
            }

        return L;
    }
    else
        return NULL;  

}
*/

// int minOrMax : min = 0 max = 1
int minMax(GameState game, int depth, int minOrMax)
{
    if (depth == 0|| game.isGameRunning == 0)
    {
        return evaluate(game);
    }
    Liste_Coups_t L = Generer_Coup(game, minOrMax);
    int moveCount = L.longueur;
    GameState moves[moveCount];

    if (minOrMax == 1)
    {  
        maxEval = -10000;
        for (int i = 0; i < moveCount; i++)
        {
            int eval = minimax(moves[i], depth - 1, 1 - minOrMax);
            if (eval > maxEval)
            {
                maxEval = eval;
            }
        }
        return maxEval;
    }
    else if (minOrMax == 0)
    {
        minEval = 10000;
        for (int i = 0; i < moveCount; i++)
        {
            int eval = minimax(moves[i], depth - 1, 1 - minOrMax);
            if (eval < minEval)
            {
                minEval = eval;   
            }
        }
        return minEval
    }

}

GameState generateBestMove(GameState game)
{
    GameState bestMove;
    int bestValue = -10000;

    Liste_Coups_t L = Generer_Coup(game, game.players[game.playerTurn]);
    int moveCount = L.longueur;
    GaameState moves[moveCount];

    for (int i = 0; i < moveCount; i++)
    {
        int moreValue = minMax(moves[i], 5, game.playerTurn);
        if (moreValue > bestValue)
        {
            bestValue = moreValue;
            bestMove = moves[i];
        }
    }
}

