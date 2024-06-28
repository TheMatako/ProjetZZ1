#include "SDL.h"
#include "SDL_ttf.h"  // je l'ai deja introduit dans le .h
#include "LasVegas.h" 

//int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Jeu Las Vegas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("path/to/font.ttf", 24);  // Ensure the font path is correct

    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        // handle error
    }

    GameState game = initGame();
    SDL_Color textColor = {255, 255, 255, 255};  // White color for the text

    // Main game loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Handle other events like key presses
        }

        // Game logic updates
        // For example, update the game state based on input
        // game = updateGameState(game);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // Black background // là jsp comment on clrear 
                                                                        //screen puisque nous on a pas qu une seule couleur 
        SDL_RenderClear(renderer);

        // Display player money
            int playerTurn = game.player[game.PlayerTurn];            char moneyText[64];
            sprintf(moneyText, "PlayerTurn %d: %dk", i + 1, game.player[PlayerTurn].totalMoney);
            int x = 50;  // X position of the text
            int y = 30 + i * 30;  // Y position of the text, adjusted per player
            renderText(renderer, font, moneyText, textColor, x, y);
    

        // Present the renderer
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Approximately 60 frames per second
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
