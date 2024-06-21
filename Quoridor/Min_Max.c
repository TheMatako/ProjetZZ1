# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

typedef struct Coup
{   
    Coup * Prec;
    Coup * Suiv;

    Position * NewPos;
    Barrier * NewBar;

} Coup_t;

typedef struct Liste_Coups
{   
    Coup_t * Tete;
    Coup_t * Queue;

    int Longueur;

} Liste_Coups_t;

int Compare_Place(Pos1,Pos2)
{
    if(Pos1->x = Pos2->x && Pos1->y = Pos2->y)
        return 1;
    return 0;
}

int Is_Diagonal_or_Simple_Moove(GameState * jeu, Position * Previous, Position * Next)
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

bool * Is_There_An_Obstacle(GameState * jeu, Position * Previous, Position * Next)
{ // True : Il y a un obstacle ,False : Aucun Obstacle
    int i; int j; bool O = false;

    if(Next->y <= BOX_NUMBER_LINE && Next->x <= BOX_NUMBER_COLUMN && !Compare_Place(jeu->Player[0]->pos,jeu->Player[1]->pos))
    {
        int Direction = Is_Diagonal_or_Simple_Moove(Previous,Next);

        switch(Direction)
        {
            case 0 :

                for(i=0;i<=19;i++)
                {
                    if(jeu->Barrier[i]->isHorizontal == 1)
                    {
                        if(Previous->y + 1 = Next->y && (jeu->Barrier[i]->pos->x = Previous->x || jeu->Barrier[i]->pos->x = Previous->x-1) && jeu->Barrier[i]->pos->y = Next->y)
                            O = true;
                        else if(Previous->y - 1 = Next->y && (jeu->Barrier[i]->pos->x = Previous->x || jeu->Barrier[i]->pos->x = Previous->x-1) && jeu->Barrier[i]->pos->y = Previous->y)
                            O = true;
                    }
                }
                break;

            case 1 :

                for(i=0;i<=19;i++)
                {
                    if(jeu->Barrier[i]->isHorizontal == 0)
                    {
                        if(Previous->x + 1 = Next->x && jeu->Barrier[i]->pos->x = Previous->x && (jeu->Barrier[i]->pos->y = Previous->y || jeu->Barrier[i]->pos->y = Previous->y+1))
                            O = true;
                        else if(Previous->x - 1 = Next->x && jeu->Barrier[i]->pos->x = Next->x && (jeu->Barrier[i]->pos->y = Previous->y || jeu->Barrier[i]->pos->y = Previous->y+1))
                            O = true;
                    }
                }
                break;

            case -1 :

                if(Previous->y + 1 == Next->y && Previous->x + 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->Barrier[i]->isHorizontal)
                        {
                            if((jeu->Barrier[i]->pos->x = Previous->x || jeu->Barrier[i]->pos->x = Previous->x-1) && jeu->Barrier[i]->pos->y = Next->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->Barrier[i]->pos->y = Previous->y|| jeu->Barrier[i]->pos->x = Previous->y+1) && jeu->Barrier[i]->pos->x = Next->x)
                                O = true;
                        }
                    }
                }
                else if(Previous->y - 1 == Next->y && Previous->x + 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->Barrier[i]->isHorizontal)
                        {
                            if((jeu->Barrier[i]->pos->x = Previous->x || jeu->Barrier[i]->pos->x = Previous->x+1) && jeu->Barrier[i]->pos->y = Previous->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->Barrier[i]->pos->y = Previous->y|| jeu->Barrier[i]->pos->y = Previous->y-1) && jeu->Barrier[i]->pos->x = Previous->x)
                                O = true;
                        }
                    }
                }
                else if(Previous->y + 1 == Next->y && Previous->x - 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->Barrier[i]->isHorizontal)
                        {
                            if((jeu->Barrier[i]->pos->x = Next->x || jeu->Barrier[i]->pos->x = Next->x-1) && jeu->Barrier[i]->pos->y = Next->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->Barrier[i]->pos->y = Previous->y|| jeu->Barrier[i]->pos->y = Previous->y+1) && jeu->Barrier[i]->pos->x = Next->x)
                                O = true;
                        }
                    }
                }
                else if(Previous->y - 1 == Next->y && Previous->x - 1 == Next->x)
                {
                    for(i=0;i<=19;i++)
                    {
                        if(jeu->Barrier[i]->isHorizontal)
                        {
                            if((jeu->Barrier[i]->pos->x = Previous->x || jeu->Barrier[i]->pos->x = Previous->x-1) && jeu->Barrier[i]->pos->y = Previous->y)
                                O = true;
                        }
                        else
                        {
                            if((jeu->Barrier[i]->pos->y = Previous->y|| jeu->Barrier[i]->pos->y = Previous->y-1) && jeu->Barrier[i]->pos->x = Next->x)
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

Liste_Coups_t * Ajouter_Coup_Liste(Liste_Coups_t * L,int xp, int yp ,int xb ,int yb ,int H);
{
    Coup_t * New = malloc(sizeof(Coup_t));

    if(New)
    {
        if(x != NULL)
            New->NewPos->x = xp; New->NewPos->y = yp;
        else
            New->NewBar->x = xb; New->NewBar->y = yb;

        if(L->tete)
        {   
            New->Prec = L->Tete->Prec;
            New->Suiv = L->Tete
            L->Tete->Prec = New;
            New->Prec->Suiv = New;
            L->Tete = New;
        }
    }

    return L;
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

bool Present(int T[8][8],int x,int y)
{   
    if(T[x][y] && T[y][x]) {return true}
    return false;
}

void Add(int T[8][8],x,y) { T[x][y] = T[y][x] = 1;}

Liste_Coups_t * Generer_Coup(GameState * jeu, int Joueur, Liste_Coups_t * L) 
{ // Il vaut mieux que L soit vide
    int i; int j;
    Liste_Coups_t * L = Creer_Liste_Coups();

    // On change de position

    Position * Place = jeu->Player[Joueur]->pos;
    Position * Nouvelle_Position = Place;

    Nouvelle_Position->y = Nouvelle_Position->y + 1;

    if(Is_There_An_Obstacle(jeu,Place,Nouvelle_Position)) /* Si il est possible d'aller en avant */
        L = Ajouter_Coup_Liste(L,Place->x,Place->y+1,NULL,NULL,NULL);
    
    Nouvelle_Position->y = Nouvelle_Position->y - 2;

    if (Is_There_An_Obstacle(jeu,Place,Nouvelle_Position)) /* Si il est possible d'aller en arrière*/
        L = Ajouter_Coup_Liste(L,Place->x,Place->y-1,NULL,NULL,NULL);

    Nouvelle_Position->y = Nouvelle_Position->y + 1;
    Nouvelle_Position->x = Nouvelle_Position->x - 1;
    
    if (Is_There_An_Obstacle(jeu,Place,Nouvelle_Position)) /* Si il est possible d'aller à Gauche */
        L = Ajouter_Coup_Liste(L,Place->x-1,Place->y,NULL,NULL,NULL);
    
    Nouvelle_Position->x = Nouvelle_Position->x + 2;

    if (Is_There_An_Obstacle(jeu,Place,Nouvelle_Position)) /* Si il est possible d'aller à droite*/
        L = Ajouter_Coup_Liste(L,Place->x+1,Place->y,NULL,NULL,NULL);
        
    // On préfère poser une barrière

    if(jeu->Player[Joueur]->barriersLeft)
    {    
        int Occupees[8][8];
        Initialiser_Liste(Occupees);

        for(i=0;i<=19;i++)
        {
            if(jeu->Barrier[i]->isHorizontal)
            {
                Add(Occupees,jeu->Barrier[i]->pos->x,jeu->Barrier[i]->pos->y);
                Add(Occupees,jeu->Barrier[i]->pos->x+1,jeu->Barrier[i]->pos->y);
            }
            else
            {
                Add(Occupees,jeu->Barrier[i]->pos->x,jeu->Barrier[i]->pos->y);
                Add(Occupees,jeu->Barrier[i]->pos->x+1,jeu->Barrier[i]->pos->y-1);
            }

            for(i=0;i<=80;i++)
            {
                for(j=0;j<=80;j++)
                {
                    if(!Present(Occupees,x,y) && !Present(Occupees,x+1,y))
                        L = Ajouter_Coup_Liste(L,NULL,NULL,x,y,1);
                    else if(!Present(Occupees,x,y) && !Present(Occupees,x,y-1))
                        L = Ajouter_Coup_Liste(L,NULL,NULL,x,y,0);
                }
            }
        }    
    }

    return L;
}

// int * Poser_Barriere(Plateau_t * P,int X,int Y)
// {
    
// }

// int Fin(Plateau_t * P,int /* Quel Joueur ?*/)
// {
//     if(P->PosX)
//     // Si l'un des deux joueurs est au bout du plateau
//     // renvoit 1,0 ou -1
// }

// int Score(Plateau_t * P,int S, int T)
// {
//     if(Fin(P) == 0)
//     {
//         // On commence par les changements de position
//         // (Il faudra Rajouter les diagonales)

//         if(/* Si il est possible d'aller en avant*/)
//         {
//             P->PosY = P->PosY + 1;
//             S = S + Generer_Coup_Ordinateur(Plateau_t,S);
//         }
//         else if (/* Si il est possible d'aller en arrière*/)
//         {
//             P->PosY = P->PosY - 1;
//             S = S + Generer_Coup_Ordinateur(Plateau_t,S);
//         }
//         else if (/* Si il est possible d'aller à gauche*/)
//         {
//             P->PosX = P->PosX - 1;
//             S = S + Generer_Coup_Ordinateur(Plateau_t,S);
//         }
//         else if (/* Si il est possible d'aller à droite*/)
//         {
//             P->PosX = P->PosX + 1;
//             S = S + Generer_Coup_Ordinateur(Plateau_t,S);
//         }

//         // On préfère poser une barrière

//     }
//     else // on a trouvé une fin de jeu
//     {
//         if(Fin() == 1)
//         {
//             return 1;
//         }
//     }
// }

// int main()
// {

// }