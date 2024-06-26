#include "SDL.h"


int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(16 * sizeof(SDL_Texture *));
    if (!allImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 1;
    }
    for (int i = 0; i < 16; i++) 
    {
        allImages[i] = NULL;
    }
    initSDL(&window, &renderer);
    // Chargez toutes les images nécessaires au jeu
    loadTextures(renderer, &allImages);
    int running = 1;
    int game = 2;
    while (running)
    {
        printf("%d\n",game/50);
        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_RenderClear(renderer);
        if (game == 1)
        {
            drawGame1(renderer);
        }
        else
        {
            drawGame2(renderer);
        }
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) 
                    {
                        case SDL_SCANCODE_ESCAPE:
                            running = 0;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100); // Délai pour limiter la fréquence de mise à jour
    }

    // Libération des ressources allouées
    for (int i = 0; i < 16; i++) 
    {
        if (allImages[i] != NULL) SDL_DestroyTexture(allImages[i]);
    }
    free(allImages);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawGame1(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
    // Dessiner les lignes de séparation du board
    SDL_Rect r1 = {240,0,1,1050};
    SDL_RenderFillRect(renderer, &r1);
    SDL_Rect r2 = {480,0,1,1050};
    SDL_RenderFillRect(renderer, &r2);
    SDL_Rect r3 = {720,0,1,1050};
    SDL_RenderFillRect(renderer, &r3);
    SDL_Rect r4 = {960,0,1,1050};
    SDL_RenderFillRect(renderer, &r4);
    SDL_Rect r5 = {1200,0,1,1050};
    SDL_RenderFillRect(renderer, &r5);
    //
    SDL_Rect r6 = {0,350,240,1};
    SDL_RenderFillRect(renderer, &r6);
    SDL_Rect r7 = {0,700,240,1};
    SDL_RenderFillRect(renderer, &r7);
    //
    SDL_Rect r8 = {240,525,720,1};
    SDL_RenderFillRect(renderer, &r8);
    //
    SDL_Rect r9 = {960,350,240,1};
    SDL_RenderFillRect(renderer, &r9);
    SDL_Rect r10 = {960,700,240,1};
    SDL_RenderFillRect(renderer, &r10);
}

void drawGame2(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
    // Dessiner les lignes de séparation du board
    SDL_Rect r1 = {0,240,1680,1};
    //
    SDL_RenderFillRect(renderer, &r1);
    SDL_Rect r2 = {336,240,1,900};
    SDL_RenderFillRect(renderer, &r2);
    SDL_Rect r3 = {672,240,1,900};
    SDL_RenderFillRect(renderer, &r3);
    SDL_Rect r4 = {1008,240,1,900};
    SDL_RenderFillRect(renderer, &r4);
    SDL_Rect r5 = {1344,240,1,900};
    SDL_RenderFillRect(renderer, &r5);
    //
    SDL_Rect r6 = {0,510,336,1};
    SDL_RenderFillRect(renderer, &r6);
    SDL_Rect r7 = {0, 780,336,1};
    SDL_RenderFillRect(renderer, &r7);
    //
    SDL_Rect r8 = {336,645,1008,1};
    SDL_RenderFillRect(renderer, &r8);
    //
    SDL_Rect r9 = {1344,510,336,1};
    SDL_RenderFillRect(renderer, &r9);
    SDL_Rect r10 = {1344, 780,336,1};
    SDL_RenderFillRect(renderer, &r10);
}




void initSDL(SDL_Window **window, SDL_Renderer **renderer) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    
    *window = SDL_CreateWindow("LasVegas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DM.w, DM.h, SDL_WINDOW_SHOWN);
    if (*window == NULL) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    printf("Width : %d, Height : %d\n", DM.w, DM.h);

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) 
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
}

void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages)
{

    char *filesName[] = {"./images/billetFaceCache.png", "./images/billet10.png", "./images/billet20.png", "./images/billet30.png", "./images/billet40.png",
                             "./images/billet50.png", "./images/billet60.png", "./images/billet70.png", "./images/billet80.png", "./images/billet90.png",
                             "./images/casino1.png", "./images/casino2.png", "./images/casino3.png", "./images/casino4.png",
                             "./images/casino5.png", "./images/casino6.png"};
    for (int i = 0; i < 16; i++)
    {
        SDL_Surface *imageSurface = IMG_Load(filesName[i]);
        if (!imageSurface)
        {
            printf("Impossible to load image %s ! SDL_image Error: %s\n", filesName[i], IMG_GetError());
            (*allImages)[i] = NULL;
        }
        else
        {
            (*allImages)[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
            if (!(*allImages)[i])
            {
                printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesName[i], SDL_GetError());
            }
            SDL_FreeSurface(imageSurface);
        }
    }
}