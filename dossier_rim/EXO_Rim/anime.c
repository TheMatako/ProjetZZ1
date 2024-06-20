#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>

 

// 
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
 
int main(int argc, char ** argv)
{
    (void)argc;
    (void)argv; 

    bool quit = false;
    SDL_Event event;
    


    


    IMG_Init(IMG_INIT_PNG);
 
    SDL_Window * window = SDL_CreateWindow("SDL2 Sprite Sheets",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 800, 0);
    SDL_Surface* arriereplan = IMG_Load("arierePlan.png"); 
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * image = IMG_Load("PikPng.com_video-game-character-png_1877811.png");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image); 
    int Coord[10][4] = { {0,0,160,250}, {300,0,430,250}, {530,0,690,250}, {790,0,957,250}, {0,270,170,520}, {270,270,430,520}, {560,270,690,520}, {810,270,957,520}, {0,530,150,785}, {270,530,430,785} };
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "Erreur d'initialisation de la SDL", window, renderer);
    }
    int personnage_x=150,personnage_y=150,speed=10;
    int direction=1;
    while (!quit)
    {   
        SDL_SetRenderDrawColor(renderer, 168, 230, 255, 255);
        SDL_RenderClear(renderer);
        Uint32 ticks= SDL_GetTicks();
        Uint32 seconds = ticks/100;
        Uint32 sprite = seconds% 10;




        while (SDL_PollEvent(&event))
        { 
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_RIGHT:
                        direction=1;
                        break;
                    case SDL_SCANCODE_LEFT:
                        direction =-1;
                        break;
                    case SDL_SCANCODE_UP:
                        direction =2;
                        break;
                    case SDL_SCANCODE_DOWN:
                        direction=0;
                        break;
                    default:
                        break;
                }            
            }
        }


        if (direction == 0)
        {
            personnage_y += speed;
        }
        else if (direction == 2)
        {
            personnage_y -= speed;
        }
        else if (direction == 1)
        {
            personnage_x = (personnage_x + speed)%1200;
        }
        else if (direction == -1)
        {
            personnage_x -= speed;
        }
        
        SDL_Rect srcrect = { Coord[sprite][0], Coord[sprite][1], Coord[sprite][2]-Coord[sprite][0], Coord[sprite][3]-Coord[sprite][1] };

        SDL_Rect dstrect = { personnage_x, personnage_y, 85, 130 };


        SDL_RenderCopy(renderer, texture, &srcrect,&dstrect);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }
 
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_FreeSurface(arriereplan);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
 
    return 0;
}