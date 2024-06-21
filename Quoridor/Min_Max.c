# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Pour les Cases
#define BOX_WIDTH 48
#define BOX_HEIGHT 48
#define BOX_NUMBER_LINE 9
#define BOX_NUMBER_COLUMN 9
#define TOTAL_BOX_NUMBER BOX_NUMBER_LINE * BOX_NUMBER_COLUMN 

typedef struct {
    int x;  // Coordonnée x
    int y;  // Coordonnée y
} Position;

typedef struct {
    Position pos;    // Position de la barrière
    int isHorizontal; // 1 pour horizontal, 0 pour vertical
} Barrier;

typedef struct {
    Position pos;      // Position actuelle du pion
    int barriersLeft;  // Nombre de barrières restantes
    SDL_Texture *sprite;
} Player;

typedef struct {
    Position pos; // Position de la case
    Position posPixel; // Position de la case en pixels
} Box;

typedef struct {
    Player * players; // Joueurs (0: humain, 1: IA)
    Barrier * barriers; // Toutes les barrières placées sur le plateau
    Position matrixBarriersPosition[8][8];
    Box boxes[BOX_NUMBER_LINE][BOX_NUMBER_COLUMN]; // Toutes les (positions des) cases sur le plateau
    int barriersCount;     // Nombre actuel de barrières placées
    bool isGameRunning;
    int playerTurn;
    
} GameState;

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
        NewL->Tete =  NULL; NewL->Queue =  NULL; 
        NewL->Longueur =  0; 
        return NewL;
    }
}

Liste_Coups_t * Ajouter_Coup_Liste(Liste_Coups_t * L, int xp, int yp ,int xb ,int yb ,int H)
{
    Coup_t * NewC = malloc(sizeof(Coup_t));

    if(NewC)
    {

        NewC->NewPos->x = xp; NewC->NewPos->y = yp;
        NewC->NewBar->pos.x = xb; NewC->NewBar->pos.y = yb;
        NewC->NewBar->isHorizontal = H;

        // Cas 1: La liste est vide
        if (L->Tete == NULL) 
        {
            L->Tete = NewC;
            L->Queue = NewC;
        }
        // Cas 2: La liste contient une seule cellule ou plus
        else 
        {
            // Insérer la nouvelle cellule à la fin de la liste
            L->Queue->Suiv = NewC;
            NewC->Prec = L->Queue;
            L->Queue = NewC;
        }
    }
}

void Affichage (Coup_t * Tete) 
{
    Coup_t * Courant = Tete;
    Coup_t * Dernier = NULL;

    while (Courant != NULL) 
    {
        printf("| %p %d %d %p | -> ", Courant->Prec,Courant->NewPos->x,Courant->NewPos->y,Courant->Suiv);

        // Mise à jour du dernier noeud visité
        Dernier = Courant;
        Courant = Courant->Suiv;

        // Vérifier si le noeud courant boucle vers un noeud précédent
        if (Courant != NULL && Courant->Prec != Dernier) 
        {
            printf("\nLoop detected. Stopping the display to prevent infinite loop.\n");
            break;
        }
    }

    printf("FIN\n");
}

void Initialiser_Liste(int T[8][8])
{
    int i; int j;

    for(i=0;i<=7;i++)
    {
        for(j=0;j<=7;j++)
        {
            T[i][j] = 0;
        }
    }
}

void Add(int T[8][8],int x,int y)  { T[x][y] = T[y][x] = 1;}

bool Present(int T[8][8],int x,int y)
{   
    if(T[x][y] && T[y][x]) {return true;}
    return false;
}

int Compare_Place(Position * Pos1,Position * Pos2)
{
    if(Pos1->x == Pos2->x && Pos1->y== Pos2->y)
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
}

bool Is_There_An_Obstacle(GameState * jeu, Position * Previous, Position * Next)
{ // True : Il y a un obstacle ,False : Aucun Obstacle
    int i; bool O = false;

    if((Next->y <= BOX_NUMBER_LINE) && (Next->x <= BOX_NUMBER_COLUMN) && (!Compare_Place(&jeu->players[0].pos,&jeu->players[1].pos)))
    {
        int Direction = Is_Diagonal_or_Simple_Moove(Previous,Next);

        switch(Direction)
        {
            case 0 :

                for(i=0;i<=19;i++)
                {   
                    if(jeu->barriers[i].isHorizontal == 1)
                    {
                        if(Previous->y + 1 == Next->y && (jeu->barriers[i].pos.x == Previous->x || jeu->barriers[i].pos.x == Previous->x-1) && jeu->barriers[i].pos.y == Next->y)
                            O = true;
                        else if(Previous->y - 1 == Next->y && (jeu->barriers[i].pos.x == Previous->x || jeu->barriers[i].pos.x == Previous->x-1) && jeu->barriers[i].pos.y == Previous->y)
                            O = true;
                    }
                }
                break;

            case 1 :

                for(i=0;i<=19;i++)
                {
                    if(jeu->barriers[i].isHorizontal == 0)
                    {
                        if(Previous->x + 1 == Next->x && jeu->barriers[i].pos.x == Previous->x && (jeu->barriers[i].pos.y == Previous->y || jeu->barriers[i].pos.y == Previous->y+1))
                            O = true;
                        else if(Previous->x - 1 == Next->x && jeu->barriers[i].pos.x == Next->x && (jeu->barriers[i].pos.y == Previous->y || jeu->barriers[i].pos.y == Previous->y+1))
                            O = true;
                    }
                }
                break;

            case -1 :

                if(Previous->y + 1 == Next->y && Previous->x + 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->barriers[i].isHorizontal)
                        {
                            if((jeu->barriers[i].pos.x == Previous->x || jeu->barriers[i].pos.x == Previous->x-1) && jeu->barriers[i].pos.y == Next->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos.y == Previous->y|| jeu->barriers[i].pos.x == Previous->y+1) && jeu->barriers[i].pos.x == Next->x)
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
                            if((jeu->barriers[i].pos.x == Previous->x || jeu->barriers[i].pos.x == Previous->x+1) && jeu->barriers[i].pos.y == Previous->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos.y == Previous->y|| jeu->barriers[i].pos.y == Previous->y-1) && jeu->barriers[i].pos.x == Previous->x)
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
                            if((jeu->barriers[i].pos.x == Next->x || jeu->barriers[i].pos.x == Next->x-1) && jeu->barriers[i].pos.y == Next->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos.y == Previous->y|| jeu->barriers[i].pos.y == Previous->y+1) && jeu->barriers[i].pos.x == Next->x)
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
                            if((jeu->barriers[i].pos.x == Previous->x || jeu->barriers[i].pos.x == Previous->x-1) && jeu->barriers[i].pos.y == Previous->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->barriers[i].pos.y == Previous->y|| jeu->barriers[i].pos.y == Previous->y-1) && jeu->barriers[i].pos.x == Next->x)
                                O = true;
                        }
                    }
                }
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

    if(Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller en avant */
        L = Ajouter_Coup_Liste(L,Place.x,Place.y+1,0,0,0);
    
    Nouvelle_Position.y = Nouvelle_Position.y - 2;

    if (Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller en arrière*/
        L = Ajouter_Coup_Liste(L,Place.x,Place.y-1,0,0,0);

    Nouvelle_Position.y= Nouvelle_Position.y+ 1;
    Nouvelle_Position.x = Nouvelle_Position.x - 1;
    
    if (Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller à Gauche */
        L = Ajouter_Coup_Liste(L,Place.x-1,Place.y,0,0,0);
    
    Nouvelle_Position.x = Nouvelle_Position.x + 2;

    if (Is_There_An_Obstacle(jeu,&Place,&Nouvelle_Position)) /* Si il est possible d'aller à droite*/
        L = Ajouter_Coup_Liste(L,Place.x+1,Place.y,0,0,0);
        
    // On préfère poser une barrière

    if(jeu->players[Joueur].barriersLeft)
    {    
        int Occupees[8][8];
        Initialiser_Liste(Occupees);

        for(i=0;i<=19;i++)
        {
            if(jeu->barriers[i].isHorizontal)
            {
                Add(Occupees,jeu->barriers[i].pos.x,jeu->barriers[i].pos.y);
                Add(Occupees,jeu->barriers[i].pos.x+1,jeu->barriers[i].pos.y);
            }
            else
            {
                Add(Occupees,jeu->barriers[i].pos.x,jeu->barriers[i].pos.y);
                Add(Occupees,jeu->barriers[i].pos.x+1,jeu->barriers[i].pos.y-1);
            }

            for(i=0;i<=80;i++)
            {
                for(j=0;j<=80;j++)
                {
                    if(!Present(Occupees,i,j) && !Present(Occupees,i+1,j))
                        L = Ajouter_Coup_Liste(L,0,0,i,j,1);
                    else if(!Present(Occupees,i,j) && !Present(Occupees,i,j-1))
                        L = Ajouter_Coup_Liste(L,0,0,i,j,0);
                }
            }
        }    
    }

    return L;
}

int main()
{
    GameState * jeu = malloc(sizeof(GameState));
    Player * J0 = malloc(sizeof(Player));
    Player * J1 = malloc(sizeof(Player));
    Position * P0 = malloc(sizeof(Position));
    Position * P1 = malloc(sizeof(Position));

    P0->x = 4; P0->y = 9;
    P1->x = 4; P1->y = 0;

    J0->pos = *P0; J1->pos = *P1;
    J0->barriersLeft = 10; J1->barriersLeft = 10;

    jeu->players[0] = *J0;
    jeu->players[1] = *J1;

    Liste_Coups_t * L = Generer_Coup(jeu,1);

    Affichage(L->Tete);
}