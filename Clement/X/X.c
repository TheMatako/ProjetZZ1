#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

int main()
{   
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * G;
    SDL_Window * D;

    G = SDL_CreateWindow("Feur",0,0,100,100,SDL_WINDOW_RESIZABLE);
    G = SDL_CreateWindow("Feur",100,100,100,100,SDL_WINDOW_RESIZABLE);
    G = SDL_CreateWindow("Feur",200,200,100,100,SDL_WINDOW_RESIZABLE);
    G = SDL_CreateWindow("Feur",300,300,100,100,SDL_WINDOW_RESIZABLE);
    G = SDL_CreateWindow("Feur",400,400,100,100,SDL_WINDOW_RESIZABLE);
    G = SDL_CreateWindow("Feur",500,500,100,100,SDL_WINDOW_RESIZABLE);

    D = SDL_CreateWindow("Feur",0,500,100,100,SDL_WINDOW_RESIZABLE);
    D = SDL_CreateWindow("Feur",100,400,100,100,SDL_WINDOW_RESIZABLE);
    D = SDL_CreateWindow("Feur",200,300,100,100,SDL_WINDOW_RESIZABLE);
    D = SDL_CreateWindow("Feur",300,200,100,100,SDL_WINDOW_RESIZABLE);
    D = SDL_CreateWindow("Feur",400,100,100,100,SDL_WINDOW_RESIZABLE);
    D = SDL_CreateWindow("Feur",500,0,100,100,SDL_WINDOW_RESIZABLE);


    SDL_Delay(3000);
    SDL_DestroyWindow(G);
    SDL_DestroyWindow(D);
    SDL_Quit();
}