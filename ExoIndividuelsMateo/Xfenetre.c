#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// Définition des constantes
#define MAIN_WINDOW_BASE_WIDTH 400
#define MAIN_WINDOW_BASE_HEIGHT 300
#define SMALL_WINDOW_WIDTH 100
#define SMALL_WINDOW_HEIGHT 75
#define SMALL_WINDOW_STEP 30
#define WINDOW_SIZE_INCREMENT 20

// Structure représentant une petite fenêtre
typedef struct SmallWindow {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int moving_right;
    struct SmallWindow* next;
} SmallWindow;

// Déclaration des prototypes de fonctions
void initSDL(SDL_Window **main_window, SDL_Renderer **main_renderer);
SDL_Window* createWindow(const char* title, int x, int y, int w, int h);
void updateMainWindowSize(SDL_Window** main_window, int num_windows);
void handleEvents(int *running, SDL_Window **main_window, int *num_windows, SmallWindow **head);

int main() {
    SDL_Window *main_window = NULL;
    SDL_Renderer *main_renderer = NULL;
    initSDL(&main_window, &main_renderer);

    int running = 1;

    // On récupère les dimensions de l'écran
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    int screen_width = display_mode.w;

    SmallWindow* head = NULL;
    int num_windows = 0; // Nombre de petites fenêtres

    while (running) 
    {
        handleEvents(&running, &main_window, &num_windows, &head);

        SmallWindow* current = head;
        SmallWindow* prev = NULL;

        while (current) 
        {
            if (current->moving_right) 
            {
                current->rect.x += SMALL_WINDOW_STEP;
                if (current->rect.x >= screen_width - SMALL_WINDOW_WIDTH) 
                {
                    current->moving_right = 0;
                }
            } 
            else 
            {
                current->rect.x -= SMALL_WINDOW_STEP;
                int main_x;
                SDL_GetWindowPosition(main_window, &main_x, NULL);
                if (current->rect.x <= main_x) 
                {
                    // On supprime et détruit la petite fenêtre lorsqu'elle revient à la fenêtre principale
                    SDL_DestroyRenderer(current->renderer);
                    SDL_DestroyWindow(current->window);
                    if (prev) 
                    {
                        prev->next = current->next;
                    } 
                    else 
                    {
                        head = current->next;
                    }
                    SmallWindow* to_free = current;
                    current = current->next;
                    free(to_free);

                    num_windows--;
                    updateMainWindowSize(&main_window, num_windows);
                    continue;
                }
            }
            SDL_SetWindowPosition(current->window, current->rect.x, current->rect.y);
            prev = current;
            current = current->next;
        }

        // Rendu de la fenêtre principale
        SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
        SDL_RenderClear(main_renderer);
        SDL_RenderPresent(main_renderer);

        SDL_Delay(50); // Contrôle la vitesse des petites fenêtres
    }

    // Nettoyage
    SmallWindow* current = head;
    while (current) 
    {
        SDL_DestroyRenderer(current->renderer);
        SDL_DestroyWindow(current->window);
        SmallWindow* to_free = current;
        current = current->next;
        free(to_free);
    }

    SDL_DestroyRenderer(main_renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}

// Initialisation de SDL et création de la fenêtre principale et du renderer
void initSDL(SDL_Window **main_window, SDL_Renderer **main_renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    *main_window = createWindow("Main Window", 0, SDL_WINDOWPOS_CENTERED, MAIN_WINDOW_BASE_WIDTH, MAIN_WINDOW_BASE_HEIGHT);
    if (*main_window == NULL) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    *main_renderer = SDL_CreateRenderer(*main_window, -1, SDL_RENDERER_ACCELERATED);
    if (*main_renderer == NULL) 
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*main_window);
        SDL_Quit();
        exit(1);
    }
}

// Création d'une fenêtre SDL
SDL_Window* createWindow(const char* title, int x, int y, int w, int h) {
    SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_RESIZABLE);
    if (!window) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
    }
    return window;
}

// Mise à jour de la taille de la fenêtre principale en fonction du nombre de petites fenêtres
void updateMainWindowSize(SDL_Window** main_window, int num_windows) {
    int new_width = MAIN_WINDOW_BASE_WIDTH + num_windows * WINDOW_SIZE_INCREMENT;
    int new_height = MAIN_WINDOW_BASE_HEIGHT + num_windows * WINDOW_SIZE_INCREMENT;
    SDL_SetWindowSize(*main_window, new_width, new_height);
}

// Gestion des événements SDL, création et destruction des petites fenêtres
void handleEvents(int *running, SDL_Window **main_window, int *num_windows, SmallWindow **head) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) 
        {
            *running = 0;
        } else if (event.type == SDL_KEYDOWN) 
        {
            if (event.key.keysym.sym == SDLK_SPACE) 
            {
                // Créer une petite fenêtre
                SmallWindow* new_small_window = (SmallWindow*)malloc(sizeof(SmallWindow));
                if (!new_small_window) 
                {
                    printf("Could not allocate memory for small window\n");
                    *running = 0;
                    break;
                }
                new_small_window->window = createWindow("Small Window", SDL_WINDOWPOS_CENTERED + MAIN_WINDOW_BASE_WIDTH / 2, SDL_WINDOWPOS_CENTERED, SMALL_WINDOW_WIDTH, SMALL_WINDOW_HEIGHT);
                if (!new_small_window->window) 
                {
                    free(new_small_window);
                    continue;
                }
                new_small_window->renderer = SDL_CreateRenderer(new_small_window->window, -1, SDL_RENDERER_ACCELERATED);
                if (!new_small_window->renderer) 
                {
                    printf("Could not create renderer: %s\n", SDL_GetError());
                    SDL_DestroyWindow(new_small_window->window);
                    free(new_small_window);
                    continue;
                }

                int main_x, main_y;
                SDL_GetWindowPosition(*main_window, &main_x, &main_y);
                new_small_window->rect.x = main_x + (MAIN_WINDOW_BASE_WIDTH - SMALL_WINDOW_WIDTH) / 2;
                new_small_window->rect.y = main_y + (MAIN_WINDOW_BASE_HEIGHT - SMALL_WINDOW_HEIGHT) / 2;
                new_small_window->rect.w = SMALL_WINDOW_WIDTH;
                new_small_window->rect.h = SMALL_WINDOW_HEIGHT;
                SDL_SetWindowPosition(new_small_window->window, new_small_window->rect.x, new_small_window->rect.y);
                new_small_window->moving_right = 1;
                new_small_window->next = *head;
                *head = new_small_window;

                (*num_windows)++;
                updateMainWindowSize(main_window, *num_windows);
            }
        }
    }
}