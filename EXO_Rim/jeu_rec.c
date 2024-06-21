#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RECT_WIDTH 100
#define RECT_HEIGHT 30
#define FALLING_RECT_SIZE 30
#define FALLING_SPEED 7

typedef struct {
    int x, y;
    int w, h;
} Rectangle;

void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer) {
    char msg_formated[255];
    int l;

    if (!ok) {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if (!ok) {
        exit(EXIT_FAILURE);
    }
}

void draw(SDL_Renderer* renderer, Rectangle rect, Rectangle falling_rect, int score) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanc opaque
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge opaque
    SDL_Rect sdl_rect = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderFillRect(renderer, &sdl_rect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Vert opaque
    SDL_Rect sdl_falling_rect = {falling_rect.x, falling_rect.y, falling_rect.w, falling_rect.h};
    SDL_RenderFillRect(renderer, &sdl_falling_rect);

    // Affichage du score (simple pour cet exemple, pas de rendu de texte)
    char score_text[50];
    snprintf(score_text, 50, "Score: %d", score);
    SDL_Log("%s", score_text);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {

    (void)argc;
    (void)argv; 

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "Erreur d'initialisation de la SDL", window, renderer);
    }

    window = SDL_CreateWindow("Simple SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        end_sdl(0, "Erreur de création de la fenêtre", window, renderer);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        end_sdl(0, "Erreur de création du renderer", window, renderer);
    }

    Rectangle rect = {WINDOW_WIDTH / 2 - RECT_WIDTH / 2, WINDOW_HEIGHT - RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT};
    Rectangle falling_rect = {rand() % (WINDOW_WIDTH - FALLING_RECT_SIZE), 0, FALLING_RECT_SIZE, FALLING_RECT_SIZE};
    int score = 0;

    srand(time(NULL));

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        rect.x -= 20;
                        if (rect.x < 0) rect.x = 0;
                        break;
                    case SDLK_RIGHT:
                        rect.x += 20;
                        if (rect.x + RECT_WIDTH > WINDOW_WIDTH) rect.x = WINDOW_WIDTH - RECT_WIDTH;
                        break;
                }
            }
        }

        falling_rect.y += FALLING_SPEED;
        if (falling_rect.y > WINDOW_HEIGHT) {
            falling_rect.y = 0;
            falling_rect.x = rand() % (WINDOW_WIDTH - FALLING_RECT_SIZE);
        }

        // on regarde s'il y a collision entre le rectangale rouge et le vert --> si oui on incrément le score et le rectangle vert retombe
        if (falling_rect.y + FALLING_RECT_SIZE > rect.y && falling_rect.x + FALLING_RECT_SIZE > rect.x && falling_rect.x < rect.x + RECT_WIDTH) {
            score++;
            falling_rect.y = 0;
            falling_rect.x = rand() % (WINDOW_WIDTH - FALLING_RECT_SIZE);
        }

        draw(renderer, rect, falling_rect, score);
        SDL_Delay(16);  
    }

    end_sdl(1, "Fin normale", window, renderer);
    return 0;
}
