#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// Définition des constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SNAKE_HEAD_SIZE 20
#define SNAKE_BODY_SIZE 14
#define SNAKE_SPEED 150
#define SNAKE_INITIAL_LENGTH 8

// Définition des directions
#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT 2
#define DIRECTION_UP 3
#define DIRECTION_DOWN 4

// Déclaration des prototypes de fonctions
void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);
void drawSnake(SDL_Renderer *renderer, int snakeX[], int snakeY[], int length);
void moveSnake(int snakeX[], int snakeY[], int length, int direction);
void handleEvents(int *running, int *direction);

int main() 
{
    // Initialisation de SDL
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    initSDL(&window, &renderer);

    // Définition des variables du serpent
    int snakeX[SNAKE_INITIAL_LENGTH + 1]; // Tableau de position des différentes parties du serpent sur la coordonnée x
    int snakeY[SNAKE_INITIAL_LENGTH + 1]; // Tableau de position des différentes parties du serpent sur la coordonnée y
    snakeX[0] = WINDOW_WIDTH / 2 - SNAKE_HEAD_SIZE / 2;
    snakeY[0] = WINDOW_HEIGHT / 2 - SNAKE_HEAD_SIZE / 2;
    for (int i = 1; i <= SNAKE_INITIAL_LENGTH; ++i) 
    {
        snakeX[i] = snakeX[0] - i * SNAKE_BODY_SIZE;
        snakeY[i] = snakeY[0] + SNAKE_HEAD_SIZE / 2;
    }
    int snakeDirection = DIRECTION_RIGHT;

    // Boucle principale du jeu
    int running = 1;
    while (running) 
    {
        // Gestion des événements
        handleEvents(&running, &snakeDirection);

        // Déplacement du serpent
        moveSnake(snakeX, snakeY, SNAKE_INITIAL_LENGTH + 1, snakeDirection);

        // Affichage du serpent
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
        SDL_RenderClear(renderer);
        drawSnake(renderer, snakeX, snakeY, SNAKE_INITIAL_LENGTH + 1);
        SDL_RenderPresent(renderer);

        // Délai d'attente pour contrôler la vitesse du jeu
        SDL_Delay(SNAKE_SPEED);
    }

    // Quitter SDL
    quitSDL(window, renderer);

    return 0;
}

void initSDL(SDL_Window **window, SDL_Renderer **renderer) 
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Impossible d'initialiser SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Création de la fenêtre
    *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) 
    {
        printf("Impossible de créer la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création du renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) 
    {
        SDL_DestroyWindow(*window);
        printf("Impossible de créer le renderer: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer) 
{
    // Destruction du renderer et de la fenêtre
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quitter SDL
    SDL_Quit();
}

void drawSnake(SDL_Renderer *renderer, int snakeX[], int snakeY[], int length) 
{
    // Dessin de la tête du serpent
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_Rect headRect = {snakeX[0], snakeY[0], SNAKE_HEAD_SIZE, SNAKE_HEAD_SIZE};
    SDL_RenderFillRect(renderer, &headRect);

    // Dessin du corps du serpent
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 255);
    for (int i = 1; i < length; ++i) 
    {
        SDL_Rect bodyRect = {snakeX[i], snakeY[i], SNAKE_BODY_SIZE, SNAKE_BODY_SIZE};
        SDL_RenderFillRect(renderer, &bodyRect);
    }
}

void moveSnake(int snakeX[], int snakeY[], int length, int direction) 
{
    // Déplacement des parties du corps
    for (int i = length - 1; i > 0; --i) 
    {
      if (direction == DIRECTION_DOWN || direction == DIRECTION_UP)
      {
        if (i!=1)
        {
          snakeX[i] = snakeX[i - 1];
        }
        else
        {
          snakeX[i] = snakeX[i - 1] + ( SNAKE_HEAD_SIZE - SNAKE_BODY_SIZE )/ 2;
        }
        snakeY[i] = snakeY[i - 1];
      }
      else
      {
        if (i!=1)
        {
          snakeY[i] = snakeY[i - 1];
        }
        else
        {
          snakeY[i] = snakeY[i - 1] + ( SNAKE_HEAD_SIZE - SNAKE_BODY_SIZE )/ 2;
        }
        snakeX[i] = snakeX[i - 1];
      }
    }

    // Déplacement de la tête du serpent
    switch (direction) 
    {
        case DIRECTION_LEFT:
            snakeX[0] -= SNAKE_HEAD_SIZE;
            break;
        case DIRECTION_RIGHT:
            snakeX[0] += SNAKE_HEAD_SIZE;
            break;
        case DIRECTION_UP:
            snakeY[0] -= SNAKE_HEAD_SIZE;
            break;
        case DIRECTION_DOWN:
            snakeY[0] += SNAKE_HEAD_SIZE;
            break;
    }

    // Gestion du passage d'un bord à l'autre
    if (snakeX[0] < 0) 
    {
        snakeX[0] = WINDOW_WIDTH - SNAKE_HEAD_SIZE;
    } 
    else if (snakeX[0] + SNAKE_HEAD_SIZE > WINDOW_WIDTH) 
    {
        snakeX[0] = 0;
    }

    if (snakeY[0] < 0) 
    {
        snakeY[0] = WINDOW_HEIGHT - SNAKE_HEAD_SIZE;
    } 
    else if (snakeY[0] + SNAKE_HEAD_SIZE > WINDOW_HEIGHT) 
    {
        snakeY[0] = 0;
    }
}

void handleEvents(int *running, int *direction) 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
            case SDL_QUIT:
                *running = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) 
                {
                    case SDL_SCANCODE_LEFT:
                        if (*direction != DIRECTION_RIGHT) 
                        {
                            *direction = DIRECTION_LEFT;
                        }
                        break;
                    case SDL_SCANCODE_RIGHT:
                        if (*direction != DIRECTION_LEFT) 
                        {
                            *direction = DIRECTION_RIGHT;
                        }
                        break;
                    case SDL_SCANCODE_UP:
                        if (*direction != DIRECTION_DOWN) 
                        {
                            *direction = DIRECTION_UP;
                        }
                        break;
                    case SDL_SCANCODE_DOWN:
                        if (*direction != DIRECTION_UP) 
                        {
                            *direction = DIRECTION_DOWN;
                        }
                        break;
                    case SDL_SCANCODE_ESCAPE:
                        *running = 0;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}
  