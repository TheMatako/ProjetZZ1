#include <SDL2/SDL.h>
#include <stdbool.h>

// Structure pour stocker la position et la taille d'un objet
typedef struct {
    SDL_Rect rect;
    bool dragging;
} DraggableObject;


    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Drag and Drop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* image = SDL_LoadBMP("image.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    float barrierHorizontalX = SPACE_LENGTH + BOX_WIDTH/2;
    int barrierHorizontalY = WINDOW_HEIGHT - 85;

    float barrierVerticalX = WINDOW_WIDTH - (3*SPACE_LENGTH+BOX_WIDTH)+ barrierVerticalWidth/2;
    int barrierVerticalY = WINDOW_HEIGHT - 125;

    void Dragdrop(Barrière draggable_bar){
    bool quit = false;
    SDL_Event event;
    
    int indice_si_une_barrière_a_ete_ajoutée=GameState->barrierCount+1;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &draggable_bar.pos)) {
                            object.dragging = true;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        object.dragging = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (object.dragging) {
                        // Mettre à jour la position de l'objet pour suivre le curseur
                        object.rect.x = event.motion.x - object.rect.w / 2;
                        object.rect.y = event.motion.y - object.rect.h / 2;
                    }
                    break;
            }
        }
  }
    bool quit = false;
    SDL_Event event;

    
            
        

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &object.rect);
        SDL_RenderPresent(renderer);
    

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
