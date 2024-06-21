#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

void Rectangle(SDL_Renderer * Rend)
{
    SDL_Rect rect;

    SDL_SetRenderDrawColor(Rend, 0, 0, 0, 255);
    SDL_RenderClear(Rend);

    SDL_SetRenderDrawColor(Rend, 128, 52, 145, 201);
    rect.x = rect.y = 12;
    rect.w = rect.h = 100;
    SDL_RenderFillRect(Rend, &rect);

    SDL_RenderPresent(Rend);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);


    /* SDL_Window * F1 = SDL_CreateWindow("SDL2 Programme 0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            1000, 1000, 
            SDL_WINDOW_RESIZABLE); */

    /* SDL_Window * C;

    for(int i = 0; i<= 25; i++)
    {
        C = SDL_CreateWindow("SDL2 Programme 0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        100*i, 100*i, SDL_WINDOW_RESIZABLE);
        SDL_Renderer * R1 = SDL_CreateRenderer(C,-1,SDL_RENDERER_ACCELERATED);
        SDL_RenderPresent(R1);
        SDL_DestroyWindow(C);
    }
    
    SDL_DestroyRenderer(R1); */

    /* SDL_Window * F1 = SDL_CreateWindow("SDL2 Programme 0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            1000, 1000, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer * R1 = SDL_CreateRenderer(F1,-1,SDL_RENDERER_ACCELERATED);
    SDL_bool Prog = SDL_TRUE;
    SDL_Event Event;

    Rectangle(R1);

    while(Prog)
    {
        if(SDL_PollEvent(&Event))
        {
            switch(Event.type)
            {
                case SDL_MOUSEBUTTONDOWN :
                    if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_RIGHT))
                    {
                        SDL_DestroyWindow(F1);
                        Prog = SDL_FALSE;
                    }
                    break;
            }
        }
    }

    SDL_Delay(2000);
    SDL_SetRenderDrawColor(R1, 0, 0, 0, 255);
    SDL_RenderClear(R1);
    SDL_DestroyRenderer(R1);
    SDL_DestroyWindow(F1); */

    /* SDL_Window * Cour;
    SDL_Renderer * Coul;
    Prog = SDL_TRUE;

    for(int i = 1; i<=1; i++)
    {
        Cour = SDL_CreateWindow("SDL2 Programme 0.1",50*i,50*i,500,500,SDL_WINDOW_RESIZABLE);
        Coul = SDL_CreateRenderer(Cour,-1,SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor(Coul, 10*i, 20*i, 30*i, 40*i);
        SDL_RenderClear(Coul);
        SDL_RenderPresent(Coul);
        SDL_Delay(100);
    }

    int x = 0 ; int y = 0 ;

   while(Prog)
    {       
        SDL_GetGlobalMouseState(&x,&y);
        printf("X = %d , Y = %d", x, y);
        system("clear");

        if(SDL_PollEvent(&Event))
        {
            switch(Event.type)
            {
                case SDL_MOUSEBUTTONDOWN :
                    if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_RIGHT))
                    {
                        SDL_DestroyWindow(Cour);
                        Prog = SDL_FALSE;
                    }
                    break;
            }
        }

        system("clear");
        SDL_Delay(500);
    }

    SDL_DestroyWindow(Cour);
    SDL_DestroyRenderer(Coul); */

    SDL_Window * W = SDL_CreateWindow("Clique sur le Carré !",SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 1000,1000, SDL_WINDOW_RESIZABLE);

    SDL_Renderer * R = SDL_CreateRenderer(W,-1,SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(R, 255, 0, 0, 0);

    int epaisseur = 5;

    SDL_Rect RECT;
    RECT.x = 100;
    RECT.y = 500;
    RECT.w = RECT.h = 250;

    SDL_Rect CONTOUR;
    CONTOUR.x = RECT.x - epaisseur;
    CONTOUR.y = RECT.y - epaisseur;
    CONTOUR.w = CONTOUR.h = RECT.w + 2*epaisseur;

    // printf(" %d %d %d %d", CONTOUR.x,CONTOUR.y,CONTOUR.w,CONTOUR.h);

    SDL_SetRenderDrawColor(R, 255, 255, 255, 0);
    SDL_RenderFillRect(R,&CONTOUR);
    SDL_RenderPresent(R);
    SDL_SetRenderDrawColor(R, 255, 0, 0, 0);
    SDL_RenderFillRect(R,&RECT);
    SDL_RenderPresent(R);

    SDL_Event EVENT;
    SDL_Cursor * Cursor = SDL_CreateSystemCursor(11);

    // (typeof(Cursor->SDL_SYSTEM_CURSOR_HAND)) * Hand;

    SDL_bool Prog = SDL_TRUE;

    int x = 0;
    int y = 0;

    SDL_SetCursor(Cursor);

    while(Prog)
        {
            if(SDL_PollEvent(&EVENT))
            {
                switch(EVENT.type)
                {
                    case SDL_QUIT :
                        SDL_DestroyWindow(W);
                        SDL_DestroyRenderer(R);
                        SDL_FreeCursor(Cursor);
                        Prog = SDL_FALSE;
                        break;

                    case SDL_MOUSEBUTTONDOWN :

                        SDL_GetMouseState(&x,&y);
                        printf("|%d %d| ",x,y);

                        if(SDL_BUTTON(SDL_BUTTON_LEFT) 
                        && (CONTOUR.x <= x)
                        && (CONTOUR.x + CONTOUR.w >= x)
                        && (CONTOUR.y <= y)
                        && (CONTOUR.y + CONTOUR.h >= y))
                        {
                            SDL_DestroyWindow(W);
                            SDL_DestroyRenderer(R);
                            SDL_FreeCursor(Cursor);
                            Prog = SDL_FALSE;
                        }
                        break;

                    default:
                        break;
                }
            }
        }

    SDL_Quit();
}