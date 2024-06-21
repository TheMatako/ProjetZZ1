#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * Jeu = SDL_CreateWindow("Snake",1000,1000,1000,1000,SDL_WINDOW_FULLSCREEN);
    SDL_Renderer * Couleur = SDL_CreateRenderer(Jeu,-1,SDL_RENDERER_ACCELERATED);
    SDL_Rect RECT;
    RECT.x = RECT.y = 0;
    RECT.w = RECT.h = 1000;
    SDL_SetRenderDrawColor(Couleur, 255, 255, 255, 0);
    SDL_RenderFillRect(Couleur,&RECT);
    SDL_RenderPresent(Couleur);

    SDL_Event EVENT;
    SDL_bool prog = SDL_TRUE;

    SDL_RenderClear(Couleur);
    SDL_SetRenderDrawColor(Couleur,255,0,0,0);

    int Pos[2] = {0,0};

    while(prog)
    {   
        if(SDL_PollEvent(&EVENT))
        {
            switch(EVENT.type)
            {   
                case SDL_QUIT :
                        prog = SDL_FALSE;
                        break;
                
                case SDL_KEYDOWN :

                    switch(EVENT.key.keysym.sym)
                    {      
                        case SDLK_z:
                            SDL_RenderDrawLine(Couleur,Pos[0],Pos[1],Pos[0],Pos[1]-10);
                            Pos[1] = Pos[1]-10;
                            break;

                        case SDLK_s :
                            SDL_RenderDrawLine(Couleur,Pos[0],Pos[1],Pos[0],Pos[1]+10);
                            Pos[1] = Pos[1]+10;
                            break;  

                        case SDLK_q :
                            SDL_RenderDrawLine(Couleur,Pos[0],Pos[1],Pos[0]-10,Pos[1]);
                            Pos[0] = Pos[0]-10;
                            break;

                        case SDLK_d :
                            SDL_RenderDrawLine(Couleur,Pos[0],Pos[1],Pos[0]+10,Pos[1]);
                            Pos[0] = Pos[0]+10;
                            break;
                        
                        case SDLK_SPACE :
                            prog = SDL_FALSE;
                            break;

                        default :
                            break;
                    }

                    SDL_RenderPresent(Couleur);

                default :
                    break;
            }
        }
    }

    SDL_DestroyRenderer(Couleur);
    SDL_DestroyWindow(Jeu);
    SDL_Quit();
}