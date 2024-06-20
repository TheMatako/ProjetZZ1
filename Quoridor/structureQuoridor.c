#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

// Définition des constantes

// Pour les Cases
#define BOX_WIDTH 64
#define BOX_HEIGHT 64
#define BOX_NUMBER_LINE 9
#define BOX_NUMBER_COLUMN 9
#define TOTAL_BOX_NUMBER BOX_NUMBER_LINE * BOX_NUMBER_COLUMN 
// Pour le Plateau
#define SPACE_LENGHT 20
// Pour les Barrières
#define BARRIER_NUMBER 20
#define BARRIER_WIDTH 2*BOX_WIDTH + SPACE_LENGHT
#define BARRIER_HEIGHT 2*BOX_HEIGHT + SPACE_LENGHT
// Pour le Jeu
#define TOP_LENGHT 200
#define BOTTOM_LENGHT 400
// Pour la fenêtre
#define WINDOW_WIDTH BOX_NUMBER_LINE*(BOX_WIDTH+SPACE_LENGHT-1)
#define WINDOW_HEIGHT BOX_NUMBER_COLUMN*(BOX_WIDTH+SPACE_LENGHT-1) + TOP_LENGHT + BOTTOM_LENGHT

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
} Case;

typedef struct {
    Player players[2]; // Joueurs (0: humain, 1: IA)
    Barrier barriers[20]; // Toutes les barrières placées sur le plateau
    Case boxes[BOX_NUMBER_LINE][BOX_NUMBER_COLUMN]; // Toutes les (positions des) cases sur le plateau
    int barrierCount;     // Nombre actuel de barrières placées
    bool isGameRunning;
} GameState;

// Prototypes fonction en attendant le .h
void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawGame(SDL_Renderer *renderer, SDL_Texture *gridBoxes);
SDL_Texture *loadTextures(SDL_Renderer *renderer);
GameState initGame();

int main() 
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    initSDL(&window, &renderer);

    // On load toutes les images nécessaires au jeu
    // Actuellement de manière pas propre
    SDL_Texture *gridBoxes;
    gridBoxes = loadTextures(renderer);

    // On initialise le jeu et ses composantes
    //GameState Jeu = initGame();



    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGame(renderer, gridBoxes);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

GameState initGame()
{
    GameState jeu = {
        .players = {{.pos = {4, 0}, .barriersLeft = 10}, {.pos = {4, 8}, .barriersLeft = 10}},
        .barrierCount = 0,
        .isGameRunning = true
    };

    return jeu;
}

SDL_Texture *loadTextures(SDL_Renderer *renderer)
{
    /*
    // On load l'image des joueurs
    SDL_Surface * imagePlayer1 = IMG_Load("images/player1.png");
    SDL_Texture * ImgPlayer1 = SDL_CreateTextureFromSurface(renderer, imagePlayer1);
    SDL_Surface * imagePlayer2 = IMG_Load("images/player2.png");
    SDL_Texture * ImgPlayer2 = SDL_CreateTextureFromSurface(renderer, imagePlayer2);
    // On load les images du plateau*/
    SDL_Surface * imageGridBoxes = IMG_Load("images/grid1.png");
    SDL_Texture * gridBoxes = SDL_CreateTextureFromSurface(renderer, imageGridBoxes);
    /*SDL_Surface * imageGridSpaces = IMG_Load("images/grid2.png");
    SDL_Texture * gridSpaces = SDL_CreateTextureFromSurface(renderer, imageGridSpaces);
    // On load les images des barrières 
    SDL_Surface * imageBarrierHorizontal = IMG_Load("images/barrierHorizontal.png");
    SDL_Texture * barrierHorizontal = SDL_CreateTextureFromSurface(renderer, imageBarrierHorizontal);
    SDL_Surface * imaageBarrierVertical = IMG_Load("images/barrierVertical.png");
    SDL_Texture * barrierVertical = SDL_CreateTextureFromSurface(renderer, imaageBarrierVertical);
    // On load les images du nombre de barrières restantes
    SDL_Surface * image0 = IMG_Load("images/0.png");
    SDL_Texture * img0 = SDL_CreateTextureFromSurface(renderer, image0);
    SDL_Surface * image1 = IMG_Load("images/1.png");
    SDL_Texture * img1 = SDL_CreateTextureFromSurface(renderer, image1);
    SDL_Surface * image2 = IMG_Load("images/2.png");
    SDL_Texture * img2 = SDL_CreateTextureFromSurface(renderer, image2);
    SDL_Surface * image3 = IMG_Load("images/3.png");
    SDL_Texture * img3 = SDL_CreateTextureFromSurface(renderer, image3);
    SDL_Surface * image4 = IMG_Load("images/4.png");
    SDL_Texture * img4 = SDL_CreateTextureFromSurface(renderer, image4);
    SDL_Surface * image5 = IMG_Load("images/5.png");
    SDL_Texture * img5 = SDL_CreateTextureFromSurface(renderer, image5);
    SDL_Surface * image6 = IMG_Load("images/6.png");
    SDL_Texture * img6 = SDL_CreateTextureFromSurface(renderer, image6);
    SDL_Surface * image7 = IMG_Load("images/7.png");
    SDL_Texture * img7 = SDL_CreateTextureFromSurface(renderer, image7);
    SDL_Surface * image8 = IMG_Load("images/8.png");
    SDL_Texture * img8 = SDL_CreateTextureFromSurface(renderer, image8);
    SDL_Surface * image9 = IMG_Load("images/9.png");
    SDL_Texture * img9 = SDL_CreateTextureFromSurface(renderer, image9);
    // On load l'image du curseur
    SDL_Surface * imageCursor = IMG_Load("images/cursor.png");
    SDL_Texture * Cursor = SDL_CreateTextureFromSurface(renderer, imageCursor);*/

    return gridBoxes;
}

void drawGame(SDL_Renderer *renderer, SDL_Texture *gridBoxes) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //
    //SDL_Rect srcRect;
    // On dessine les cases
    for (int i = 0; i < BOX_NUMBER_LINE; i++) 
    {
        for (int j = 0; j < BOX_NUMBER_COLUMN; j++)
        {
            SDL_Rect dstRect = {SPACE_LENGHT + i*(BOX_WIDTH+SPACE_LENGHT), SPACE_LENGHT + j*(BOX_HEIGHT+SPACE_LENGHT), BOX_WIDTH, BOX_HEIGHT};
            SDL_RenderCopy(renderer, gridBoxes, NULL, &dstRect);
        }
    }
}

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    *window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
}