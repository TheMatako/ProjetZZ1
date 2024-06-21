#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * Window = SDL_CreateWindow("UwU",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                                            1000,1000,SDL_WINDOW_RESIZABLE);

    SDL_Renderer * Renderer = SDL_CreateRenderer(Window,-1,SDL_RENDERER_ACCELERATED);

    SDL_Surface * Surface = IMG_Load("pngegg.png");

    SDL_Texture * Texture = SDL_CreateTextureFromSurface(Renderer,Surface);

    // SDL_BlendMode * BlendMode;

    SDL_Rect source; source.x = source.y = 0;

    SDL_Rect destination;
    destination.x = destination.y = 500 - 125;
    destination.w = destination.h = 250;

    // SDL_GetWindowSize(Window,&window_dimensions.w,&window_dimensions.h);
    // destination = window_dimensions;

    SDL_QueryTexture(Texture,NULL,NULL,&source.w,&source.h);
    // SDL_SetRenderDrawBlendMode(&Texture, SDL_BLENDMODE_NONE);
    // SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(Renderer,Texture,&source,&destination);

    // SDL_RenderPresent(Renderer);
    // SDL_Delay(1000);

    SDL_Event EVENT;
    SDL_bool prog = SDL_TRUE;
    SDL_Cursor * Cursor = SDL_CreateSystemCursor(3);
    SDL_SetCursor(Cursor);

    int i = 0;

    while(prog)
    {
        while(SDL_PollEvent(&EVENT))
        {
            switch(EVENT.type)
            {
                case SDL_QUIT :
                        prog = SDL_FALSE;
                        break;
                
                case SDL_KEYDOWN :
                    // SDL_BUTTON(SDL_BUTTON_RIGHT)
                    switch(EVENT.key.keysym.sym)
                    {      
                        case SDLK_z:

                            if(i == 0)
                            {
                                source.x = 180; source.y = 610;
                                source.w = 190; source.h = 800 - 610;
                                destination.y = destination.y - 5;
                                i = 1;
                            }
                            else
                            {
                                source.x = 380 ; source.y = 610;
                                source.w = 190; source.h = 800 - 610;
                                destination.y = destination.y - 5;
                                i = 0;
                            }

                            break;

                        case SDLK_s :

                            if(i == 0)
                            {
                                source.x = 180; source.y = 0;
                                source.w = 190; source.h = 800 - 610;
                                destination.y = destination.y + 5;
                                i = 1;
                            }
                            else
                            {
                                source.x = 380 ; source.y = 0;
                                source.w = 190; source.h = 800 - 610;
                                destination.y = destination.y + 5;
                                i = 0;
                            }

                            break;  

                        case SDLK_q :

                            if(i == 0)
                            {
                                source.x = 180; source.y = 200;
                                source.w = 190; source.h = 800 - 610;
                                destination.x = destination.x - 5;
                                i = 1;
                            }
                            else
                            {
                                source.x = 380 ; source.y = 200;
                                source.w = 190; source.h = 800 - 610;
                                destination.x = destination.x - 5;
                                i = 0;
                            }

                            break;

                        case SDLK_d :

                            if(i == 0)
                            {
                                source.x = 180; source.y = 400;
                                source.w = 190; source.h = 800 - 610;
                                destination.x = destination.x + 5;
                                i = 1;
                            }
                            else
                            {
                                source.x = 380 ; source.y = 400;
                                source.w = 190; source.h = 800 - 610;
                                destination.x = destination.x + 5;
                                i = 0;
                            }

                            break;
                        
                        case SDLK_SPACE :
                            prog = SDL_FALSE;
                            break;

                        default :
                            break;
                    }
                    break;

                default :
                    break;
            }
        }

        SDL_RenderClear(Renderer);
        SDL_RenderCopy(Renderer,Texture,&source,&destination);
        SDL_RenderPresent(Renderer);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyTexture(Texture);
    SDL_FreeCursor(Cursor);
    SDL_FreeSurface(Surface);
    IMG_Quit();
    SDL_Quit();
}