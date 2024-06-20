#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


// Définition des constantes

// Pour les Cases
#define BOX_WIDTH 64
#define BOX_HEIGHT 64
#define BOX_NUMBER 81
// Pour le Plateau
#define SPACE_LENGHT 20
// Pour les Barrières
#define BARRIER_NUMBER 20
#define BARRIER_WIDTH 2*BOX_WIDTH + SPACE_LENGHT
#define BARRIER_HEIGHT 2*BOX_HEIGHT + SPACE_LENGHT
// Pour le Jeu
#define TOP_LENGHT 200
#define BOTTOM_LENGHT 400
#define GAME_WIDTH BOX_NUMBER*(BOX_WIDTH+SPACE_LENGHT-1)
#define GAME_HEIGHT BOX_NUMBER*(BOX_WIDTH+SPACE_LENGHT-1) + TOP_LENGHT + BOTTOM_LENGHT

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
} Player;

typedef struct {
    Position pos; // Position de la case
    
} Case

typedef struct {
    Player players[2]; // Joueurs (0: humain, 1: IA)
    Barrier barriers[20]; // Toutes les barrières placées sur le plateau
    Case case[81]; // Toutes les cases sur le plateau
    int barrierCount;     // Nombre actuel de barrières placées
} GameState;

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void drawGrid(SDL_Renderer *renderer);
void drawPlayer(SDL_Renderer *renderer, Position pos, SDL_Color color);
void drawBarrier(SDL_Renderer *renderer, Barrier barrier);

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    initSDL(&window, &renderer);

    GameState gameState = {
        .players = {{.pos = {4, 0}, .barriersLeft = 10}, {.pos = {4, 8}, .barriersLeft = 10}},
        .barrierCount = 0
    };

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer);
        drawPlayer(renderer, gameState.players[0].pos, (SDL_Color){0, 0, 255, 255});
        drawPlayer(renderer, gameState.players[1].pos, (SDL_Color){255, 0, 0, 255});

        for (int i = 0; i < gameState.barrierCount; ++i) {
            drawBarrier(renderer, gameState.barriers[i]);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void drawBarrier(SDL_Renderer *renderer, Barrier barrier) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if (barrier.isHorizontal) {
        SDL_Rect rect = {barrier.pos.x * 80, barrier.pos.y * 80 + 60, 160, 20};
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_Rect rect = {barrier.pos.x * 80 + 60, barrier.pos.y * 80, 20, 160};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void drawPlayer(SDL_Renderer *renderer, Position pos, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {pos.x * 80 + 20, pos.y * 80 + 20, 40, 40};
    SDL_RenderFillRect(renderer, &rect);
}

void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= 9; ++i) {
        SDL_RenderDrawLine(renderer, i * 80, 0, i * 80, 720);
        SDL_RenderDrawLine(renderer, 0, i * 80, 720, i * 80);
    }
}

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    *window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
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