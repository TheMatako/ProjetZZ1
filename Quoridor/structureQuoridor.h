#ifndef QUORIDOR_H
#define QUORIDOR_H

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
#define BARRIER_HEIGHT 2*BOX_HEIGHT + SPACE_LENGTH
// Pour le Jeu
#define TOP_LENGTH 100
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
} Case;

typedef struct {
    Player players[2]; // Joueurs (0: humain, 1: IA)
    Barrier barriers[20]; // Toutes les barrières placées sur le plateau
    Case boxes[BOX_NUMBER_LINE][BOX_NUMBER_COLUMN]; // Toutes les (positions des) cases sur le plateau
    int barrierCount;     // Nombre actuel de barrières placées
    bool isGameRunning;
    int playerTurn;
} GameState;


void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, GameState Jeu);
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages);
GameState initGame();
void getCursorIndex(GameState game, int *positionX, int *positionY, bool *mouvementEffectue);

#endif