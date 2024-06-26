#ifndef QUORIDOR_H
#define QUORIDOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>


// Définition des constantes

// Pour les Cases
#define BOX_WIDTH 48
#define BOX_HEIGHT 48
#define BOX_NUMBER_LINE 9
#define BOX_NUMBER_COLUMN 9
#define TOTAL_BOX_NUMBER BOX_NUMBER_LINE * BOX_NUMBER_COLUMN 
// Pour le Plateau
#define SPACE_LENGTH 20
// Pour les Barrières
#define BARRIER_NUMBER 20
#define BARRIER_WIDTH 2*BOX_WIDTH + SPACE_LENGTH
#define BARRIER_HEIGHT SPACE_LENGTH
// Pour le Jeu
#define TOP_LENGTH 75
#define BOTTOM_LENGTH 150
// Pour la fenêtre
#define WINDOW_WIDTH BOX_NUMBER_LINE*BOX_WIDTH+(BOX_NUMBER_LINE+1)*SPACE_LENGTH
#define WINDOW_HEIGHT BOX_NUMBER_COLUMN*BOX_WIDTH+(BOX_NUMBER_COLUMN+1)*SPACE_LENGTH + TOP_LENGTH + BOTTOM_LENGTH

// Définition des structures du jeu
typedef struct {
    int x;  // Coordonnée x
    int y;  // Coordonnée y
} Position;

typedef struct {
    Position pos1;    // Position de la barrière (Ne pas oublier que la barrière fait 2 case)
    Position pos2;
    int isHorizontal; // 1 pour horizontal, 0 pour vertical
    bool isPlaced;
    SDL_Rect rect;  //

} Barrier;

typedef struct {
    Position pos;      // Position actuelle du pion
    int barriersLeft;  // Nombre de barrières restantes
} Player;

typedef struct {
    Position pos; // Position de la case
    Position posPixel; // Position de la case en pixels
} Box;

typedef struct {
    Player players[2]; // Joueurs (0: humain, 1: IA)
    Barrier barriers[20]; // Toutes les barrières placées sur le plateau
    Box boxes[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE]; // Toutes les (positions des) cases sur le plateau
    int barrierCount;     // Nombre actuel de barrières placées
    bool isGameRunning;
    int playerTurn;
    int isDragging;   // Indique si un drag est en cours 0: pas de drag, 1: drag barrière horizontale, 2: drag barrière verticale
    Barrier *draggedBarrier; // Pointeur vers la barrière actuellement déplacée
    SDL_Rect dragRect; // Rectangle de rendu pour la barrière en cours de drag
    
} GameState;

typedef struct Coup
{   
    struct Coup * Prec;
    struct Coup * Suiv;

    Position * NewPos; // Nouvelle Position engendré par le coup
    Barrier * NewBar; // Nouvelle Barrière

} Coup_t;

typedef struct Liste_Coups
{   
    Coup_t * Tete;
    Coup_t * Queue;

    int Longueur;

} Liste_Coups_t;
/* typedef struct Coup
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

} Liste_Coups_t; */

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, GameState Jeu, int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE]);
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages);
GameState initGame(SDL_Texture* image_barreir);
void getCursorIndex(GameState game, int *positionX, int *positionY, bool *mouvementEffectue, int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE]);
void getPositionPlayable(GameState game, int *positionX, int *positionY, int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE]);
int isThereBarrier(GameState game, int otherPlayerPosX, int otherPlayerPosY, int direction);


Liste_Coups_t * Creer_Liste_Coups();
// Fonction d'évaluation
int evaluate(GameState game);
Liste_Coups_t * Generer_Coup(GameState * jeu, int Joueur);
// int minOrMax : min = 0 max = 1
int minMax(GameState game, int depth, int minOrMax);

GameState generateBestMove(GameState game);








/*
int Compare_Place(Pos1,Pos2)
int Is_Diagonal_or_Simple_Moove(GameState * jeu, Position * Previous, Position * Next)
bool * Is_There_An_Obstacle(GameState * jeu, Position * Previous, Position * Next)
Liste_Coups_t * Ajouter_Coup_Liste(Liste_Coups_t * L,int xp, int yp ,int xb ,int yb ,int H);
Liste_Coups_t * Generer_Coup(GameState * jeu, int Joueur, Liste_Coups_t * L) 
*/


#endif