#include "SDL.h"


int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(16 * sizeof(SDL_Texture *));
    SDL_Texture **dicesImages = malloc(16 * sizeof(SDL_Texture *));
    if (!allImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 1;
    }
    if (!dicesImages) 
    {
        fprintf(stderr, "Failed to allocate memory for textures.\n");
        return 1;
    }
    for (int i = 0; i < 16; i++) 
    {
        if (i < 6) dicesImages[i] = NULL;
        allImages[i] = NULL;
    }
    initSDL(&window, &renderer);
    // Chargez toutes les images nécessaires au jeu
    loadTextures(renderer, &allImages);
    loadTexturesDices(renderer, &dicesImages);
    int running = 1;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_RenderClear(renderer);
        drawGame(renderer, allImages, dicesImages);
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
        if (i < 6)
        {
            if (dicesImages[i] != NULL) SDL_DestroyTexture(dicesImages[i]);
        }
        if (allImages[i] != NULL) SDL_DestroyTexture(allImages[i]);
    }
    free(dicesImages);
    free(allImages);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, SDL_Texture **dicesImages)
{
    drawLines(renderer);
    drawDiceImages(renderer, dicesImages);
    drawCasino(renderer, allImages);
}

void drawDiceImages(SDL_Renderer *renderer, SDL_Texture **dicesImages)
{
    // Dé 1 :
    SDL_Rect dstRectdice1 = {0,255,42,42};
    SDL_RenderCopy(renderer, dicesImages[0], NULL, &dstRectdice1);

    // Dé 2 :
    SDL_Rect dstRectdice2 = {0,495,42,42};
    SDL_RenderCopy(renderer, dicesImages[1], NULL, &dstRectdice2);

    // Dé 3 :
    SDL_Rect dstRectdice3 = {0,735,42,42};
    SDL_RenderCopy(renderer, dicesImages[2], NULL, &dstRectdice3);

    // Dé 4 :
    SDL_Rect dstRectdice4 = {1680-42,255,42,42};
    SDL_RenderCopy(renderer, dicesImages[3], NULL, &dstRectdice4);

    // Dé 5 :
    SDL_Rect dstRectdice5 = {1680-42,495,42,42};
    SDL_RenderCopy(renderer, dicesImages[4], NULL, &dstRectdice5);

    // Dé 6 :
    SDL_Rect dstRectdice6 = {1680-42,735,42,42};
    SDL_RenderCopy(renderer, dicesImages[5], NULL, &dstRectdice6);
}

void drawCasino(SDL_Renderer *renderer, SDL_Texture **allImages)
{
    // Casino 1 :
    SDL_Rect dstRectCasino1 = {240,255,400,360};
    SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectCasino1);

    // Casino 2 :
    SDL_Rect dstRectCasino2 = {640,255,400,360};
    SDL_RenderCopy(renderer, allImages[11], NULL, &dstRectCasino2);
    
    // Casino 3 :
    SDL_Rect dstRectCasino3 = {1040,255,400,360};
    SDL_RenderCopy(renderer, allImages[12], NULL, &dstRectCasino3);

    // Casino 4 :
    SDL_Rect dstRectCasino4 = {240,615,400,360};
    SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectCasino4);

    // Casino 5 :
    SDL_Rect dstRectCasino5 = {640,615,400,360};
    SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectCasino5);

    // Casino 6 :
    SDL_Rect dstRectCasino6 = {1040,615,400,360};
    SDL_RenderCopy(renderer, allImages[15], NULL, &dstRectCasino6);
}
void drawLines(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
    // Dessiner les lignes de séparation du board
    SDL_Rect r1 = {0,255,1680,1};
    // Lignes verticales séparant les zones dé et les différents casinos
    SDL_RenderFillRect(renderer, &r1);
    SDL_Rect r2 = {240,255,1,900};
    SDL_RenderFillRect(renderer, &r2);
    SDL_Rect r3 = {640,255,1,900};
    SDL_RenderFillRect(renderer, &r3);
    SDL_Rect r4 = {1040,255,1,900};
    SDL_RenderFillRect(renderer, &r4);
    SDL_Rect r5 = {1440,255,1,900};
    SDL_RenderFillRect(renderer, &r5);
    // Lignes horizontales dés 1 à 3
    SDL_Rect r6 = {0,495,240,1};
    SDL_RenderFillRect(renderer, &r6);
    SDL_Rect r7 = {0, 735,240,1};
    SDL_RenderFillRect(renderer, &r7);
    // Ligne horizontale séparant les casino 1à3 et ceux 4à6
    SDL_Rect r8 = {240,615,1200,1};
    SDL_RenderFillRect(renderer, &r8);
    // Lignes horizontales séparant les dés 4 à 6
    SDL_Rect r9 = {1440,495,240,1};
    SDL_RenderFillRect(renderer, &r9);
    SDL_Rect r10 = {1440, 735,240,1};
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

void loadTexturesDices(SDL_Renderer *renderer, SDL_Texture ***dicesImages)
{

    char *filesName[] = {"./images/de1.png","./images/de2.png","./images/de3.png","./images/de4.png","./images/de5.png","./images/de6.png",};
    for (int i = 0; i < 6; i++)
    {
        SDL_Surface *imageSurface = IMG_Load(filesName[i]);
        if (!imageSurface)
        {
            printf("Impossible to load image %s ! SDL_image Error: %s\n", filesName[i], IMG_GetError());
            (*dicesImages)[i] = NULL;
        }
        else
        {
            (*dicesImages)[i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
            if (!(*dicesImages)[i])
            {
                printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesName[i], SDL_GetError());
            }
            SDL_FreeSurface(imageSurface);
        }
    }
}

