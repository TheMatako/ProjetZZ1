#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define N 20

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    // on initialise la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error: SDL initialization - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window *window[N];
    for (int n = 0; n < N; n++)
    {
        window[n] = SDL_CreateWindow("fenetre_mobile", 0, 0, 20*(n+1) , 20*(n+1) , SDL_WINDOW_RESIZABLE);

        // on vérifie la création de la fenetre
        if (window[n] == NULL)
        {
            SDL_Log("Error: SDL window creation - %s\n", SDL_GetError());
        
            for (int i = 0; i < n; i++)
            {
                SDL_DestroyWindow(window[i]);
            }
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
    }

    int w, h;
    SDL_GetWindowSize(window[N-1], &w, &h);

    SDL_DisplayMode D;
    SDL_GetCurrentDisplayMode(0, &D);
    int Ecran_h = D.h;
    

    //je fais bouger les fenetres une par une 
    for (int n = 0; n < N; n++)
    { 
         for (int i = 0; i < (Ecran_h - h) / 10; i++)
        {
        SDL_SetWindowPosition(window[n], 10 * i, 10 * i);
        }

        int x, y;
        SDL_GetWindowPosition(window[n], &x, &y);

        for (int j = 1; j < 30; j++)
        {
        x += 5 * j;
        y -= 5 * j;
        SDL_SetWindowPosition(window[n], x, y);
        }

    }
    
    SDL_Delay(4000);
    for (int n = 0; n < N; n++)
    {
        SDL_DestroyWindow(window[N-1-n]);
    }

    SDL_Quit();
    return 0;
}
 