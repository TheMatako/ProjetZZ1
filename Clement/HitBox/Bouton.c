#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>   

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * W = SDL_CreateWindow("Clique sur le Carré !",SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 1000,1000, SDL_WINDOW_RESIZABLE);

    SDL_Renderer * R = SDL_CreateRenderer(W,-1,SDL_RENDERER_ACCELERATED);

    int epaisseur = 6;

    SDL_Rect RECT;
    RECT.x = 250;
    RECT.y = 500;
    RECT.w = 500;
    RECT.h = 400;

    SDL_Rect CONTOUR;
    CONTOUR.x = RECT.x - epaisseur;
    CONTOUR.y = RECT.y - epaisseur;
    CONTOUR.w = RECT.w + 2*epaisseur;
    CONTOUR.h = RECT.h + 2*epaisseur;

    // printf(" %d %d %d %d", CONTOUR.x,CONTOUR.y,CONTOUR.w,CONTOUR.h);

    SDL_SetRenderDrawColor(R, 255, 255, 255, 0);
    SDL_RenderFillRect(R,&CONTOUR);
    SDL_RenderPresent(R);
    SDL_SetRenderDrawColor(R, 255, 0, 0, 0);
    SDL_RenderFillRect(R,&RECT);
    SDL_RenderPresent(R);

    SDL_Event EVENT;
    SDL_bool Prog = SDL_TRUE;
    SDL_Cursor * Cursor = SDL_CreateSystemCursor(0);
    SDL_SetCursor(Cursor);

    SDL_Delay(1000);

    int x = 0; int y = 0;

    while(Prog)
    {
       SDL_GetMouseState(&x,&y);

       if (    (CONTOUR.x <= x)
            && (CONTOUR.x + CONTOUR.w >= x)
            && (CONTOUR.y <= y)
            && (CONTOUR.y + CONTOUR.h >= y)
          )
        {
            Cursor = SDL_CreateSystemCursor(11);
            SDL_SetCursor(Cursor);
        }
        else
        {
            Cursor = SDL_CreateSystemCursor(0);
            SDL_SetCursor(Cursor);
        }

        while(SDL_PollEvent(&EVENT))
        {
            switch(EVENT.type)
            {
                case SDL_QUIT :

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
                        Prog = SDL_FALSE;
            
                    break;

                    default:
                        break;
            }
        }

        SDL_Delay(10);
    }

    SDL_FreeCursor(Cursor);
    SDL_DestroyWindow(W);
    SDL_DestroyRenderer(R);
    SDL_Quit();
}