#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structureQuoridor.h"

typedef struct Coup
{   
    struct Coup * Prec;
    struct Coup * Suiv;

    Position * NewPos;
    Barrier * NewBar;

} Coup_t;

typedef struct Liste_Coups
{   
    Coup_t * Tete;
    Coup_t * Queue;

    int Longueur;

} Liste_Coups_t;

Liste_Coups_t * Creer_Liste_Coups()
{
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
{
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

void Affichage (Liste_Coups_t * L) 
{
    Coup_t * Courant = L->Tete;
    Coup_t * Dernier = Courant;

    printf("XP YP XB YB Sens \n\n");

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

    printf("FIN\n");
}

void FreeListe(Liste_Coups_t * L) 
{
    Coup_t * Courant = L->Tete;
    Coup_t * NextCoup ;

    while (Courant != NULL) 
    {
        NextCoup  = Courant->Suiv;
        free(Courant);
        Courant = NextCoup ;
    }

    L->Tete= NULL;
}


void Initialiser_Liste(int T[8][8])
{
    int i; int j;

    for(i=0;i<=8;i++)
    {
        for(j=0;j<=8;j++)
        {
            T[i][j] = 0;
            printf("%d",T[i][j]);
        }
    }
}

void Add(int T[8][8],int x,int y) { T[x][y] = T[y][x] = 1;}

bool Present(int T[8][8],int x,int y)
{   
    if(T[x][y] && T[y][x]) {return true;}
    return false;
}

int Compare_Place(Position * Pos1,Position * Pos2)
{
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

bool Is_There_An_Obstacle(GameState * jeu, Position * Previous, Position * Next)
{ // True : Il y a un obstacle , False : Aucun Obstacle
    int i; bool O = false;

    if((Next->y >= 0) && ((Next->x >= 0)) && (Next->y < BOX_NUMBER_LINE) && (Next->x < BOX_NUMBER_COLUMN) 
    && !Compare_Place(Next,&jeu->players[0].pos) && !Compare_Place(Next,&jeu->players[1].pos))
    {
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

Liste_Coups_t * Generer_Coup(GameState * jeu, int Joueur)
{ // Il vaut mieux que L soit vide
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
        Initialiser_Liste(Occupees);

        for(i=0;i<=19;i++)
        {
            if(jeu->barriers[i].isHorizontal)
            {
                Add(Occupees,jeu->barriers[i].pos1.x,jeu->barriers[i].pos1.y);
                Add(Occupees,jeu->barriers[i].pos1.x,jeu->barriers[i].pos1.y);
            }
            else
            {
                Add(Occupees,jeu->barriers[i].pos1.x,jeu->barriers[i].pos1.y);
                Add(Occupees,jeu->barriers[i].pos1.x,jeu->barriers[i].pos1.y);
            }
        }

        for(i=0;i<=7;i++)
        {
            for(j=0;j<=7;j++)
            {
                if(!Present(Occupees,i,j) && !Present(Occupees,i+1,j))
                    L = Ajouter_Coup_Liste(L,0,0,i,j,1);
                else if(!Present(Occupees,i,j) && !Present(Occupees,i,j-1))
                    L = Ajouter_Coup_Liste(L,0,0,i,j,0);
            }
        }
    }

    return L;
}

GameState initGame()
{
    GameState jeu = 
    {
        .players = {{.pos = {4, 8}, .barriersLeft = 10}, {.pos = {4, 0}, .barriersLeft = 10}},
        .barrierCount = 0,
        .isGameRunning = true,
        .playerTurn = 0
    };

    for (int i = 0; i < BOX_NUMBER_LINE; i++) 
    {
        for (int j = 0; j < BOX_NUMBER_COLUMN; j++)
        {
            // On initialise dans le tableau jeu.boxes les positions en pixel des cases
            jeu.boxes[i][j].posPixel.x = SPACE_LENGTH + i*(BOX_WIDTH+SPACE_LENGTH);
            jeu.boxes[i][j].posPixel.y = TOP_LENGTH + SPACE_LENGTH + j*(BOX_HEIGHT+SPACE_LENGTH);
        }
    }

    return jeu;
}

int main()
{
    GameState jeu = initGame();
    /* Player * J0 = malloc(sizeof(Player));
    Player * J1 = malloc(sizeof(Player));
    Position * P0 = malloc(sizeof(Position));
    Position * P1 = malloc(sizeof(Position)); */

    /* P0->x = 4; P0->y = 9;
    P1->x = 4; P1->y = 0;

    J0->pos = *P0; J1->pos = *P1;
    J0->barriersLeft = 10; J1->barriersLeft = 10;

    jeu->players[0] = *J0;
    jeu->players[1] = *J1; */

    // Liste_Coups_t * L = Creer_Liste_Coups();
    /* L = Ajouter_Coup_Liste(L,4,8,0,0,0);
    L = Ajouter_Coup_Liste(L,5,8,0,0,0);
    L = Ajouter_Coup_Liste(L,5,7,0,0,0);
    L = Ajouter_Coup_Liste(L,4,8,8,8,1);
    L = Ajouter_Coup_Liste(L,4,8,4,8,1); */

    /* int T[8][8];

    Initialiser_Liste(T);
    Add(T,4,7);
    printf("%d %d",T[4][7],T[7][4]);*/

    // Position * P0 = malloc(sizeof(Position));
    // Position * P1 = malloc(sizeof(Position)); 

    // P0->x = jeu.players[0].pos.x; 
    // P0->y = jeu.players[0].pos.y;

    // P0->x = 4;
    // P0->y = 8;
    // P1->x = P0->x;
    // P1->y = P0->y;

    // jeu.barriers[0].pos1.x = P0->x-1;
    // jeu.barriers[0].pos1.y = P0->y;
    // jeu.barriers[0].isHorizontal = 0;

    // printf("%d",Is_Diagonal_or_Simple_Moove(P0,P1));

    // printf("%d",Is_There_An_Obstacle(&jeu,P0,P1));

    Liste_Coups_t * L = Generer_Coup(&jeu,0);

    Affichage(L);

    FreeListe(L);
}