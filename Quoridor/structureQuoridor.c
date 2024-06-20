#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
<<<<<<< HEAD
#include <stdbool.h>
<<<<<<< HEAD
=======
>>>>>>> a6b55a8 (Le point H qui a été supprimé)
=======
#includes
>>>>>>> 46c480f (Implémentation Grille + Joueurs + Barrières + Déplacement joueurs)
#include "structureQuoridor.h"
// Définition des constantes

// Pour les Cases
#define BOX_WIDTH 48
#define BOX_HEIGHT 48
#define BOX_NUMBER_LINE 9
#define BOX_NUMBER_COLUMN 9
#define TOTAL_BOX_NUMBER BOX_NUMBER_LINE * BOX_NUMBER_COLUMN 
// Pour le Plateau
#define SPACE_LENGTH 20
// Pour les Barrières
#define BARRIER_NUMBER 20
#define BARRIER_WIDTH 2*BOX_WIDTH + SPACE_LENGTH
#define BARRIER_HEIGHT 2*BOX_HEIGHT + SPACE_LENGTH
// Pour le Jeu
#define TOP_LENGTH 100
#define BOTTOM_LENGTH 150
// Pour la fenêtre
#define WINDOW_WIDTH BOX_NUMBER_LINE*BOX_WIDTH+(BOX_NUMBER_LINE+1)*SPACE_LENGTH
#define WINDOW_HEIGHT BOX_NUMBER_COLUMN*BOX_WIDTH+(BOX_NUMBER_COLUMN+1)*SPACE_LENGTH + TOP_LENGTH + BOTTOM_LENGTH

// Définition des structures du jeu
typedef struct {
    int x;  // Coordonnée x
    int y;  // Coordonnée y
} Position;

typedef struct {
    Position pos;    // Position de la barrière
    int isHorizontal; // 1 pour horizontal, 0 pour vertical
} Barrier;

typedef struct {
    Position pos;      // Position actuelle du pion
    int barriersLeft;  // Nombre de barrières restantes
    SDL_Texture *sprite;
} Player;

typedef struct {
    Position pos; // Position de la case
    Position posPixel; // Position de la case en pixels
} Case;

typedef struct {
    Player players[2]; // Joueurs (0: humain, 1: IA)
    Barrier barriers[20]; // Toutes les barrières placées sur le plateau
    Case boxes[BOX_NUMBER_LINE][BOX_NUMBER_COLUMN]; // Toutes les (positions des) cases sur le plateau
    int barrierCount;     // Nombre actuel de barrières placées
    bool isGameRunning;
    int playerTurn;
} GameState;



int main() 
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture **allImages = malloc(16 * sizeof(SDL_Texture *));
    initSDL(&window, &renderer);

    // On load toutes les images nécessaires au jeu
    // Actuellement de manière pas propre
    // Images 0 à 9 de 0 à 9, Box = 10, Player 1 = 11, Player 2 = 12, BarrièreHorizon = 13, BarrièreVerti = 14, Curseur = 15
    loadTextures(renderer, &allImages);

    // On initialise le jeu et ses composantes
    GameState Jeu = initGame();
    bool mouvementEffectue = false;
    while (Jeu.isGameRunning) 
    {
        SDL_Event event;
        printf("Tour au joueur %d\n", Jeu.playerTurn);
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                Jeu.isGameRunning = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)  // On gère les déplacements en fonction des flèches directionnelles pressées
                {                                   // On évite de permettre au serpent de faire un demi tour d'un coup
                    case SDL_SCANCODE_ESCAPE:
                        Jeu.isGameRunning = false;
                        break;
                    case SDL_SCANCODE_SPACE:
                        Jeu.players[0].barriersLeft -= 1;
                        break;
                    default:
                        break;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    getCursorIndex(Jeu, &Jeu.players[Jeu.playerTurn].pos.x, &Jeu.players[Jeu.playerTurn].pos.y, &mouvementEffectue);
                    if (mouvementEffectue)
                    {
                        Jeu.playerTurn = (Jeu.playerTurn + 1) % 2;
                        mouvementEffectue = false;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
        SDL_RenderClear(renderer);

        drawGame(renderer, allImages, Jeu);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

GameState initGame()
{
    GameState jeu = 
    {
        .players = {{.pos = {4, 8}, .barriersLeft = 10}, {.pos = {4, 0}, .barriersLeft = 10}},
        .barrierCount = 0,
        .isGameRunning = true,
        .playerTurn = 0
    };

    for (int i = 0; i < BOX_NUMBER_LINE; i++) 
    {
        for (int j = 0; j < BOX_NUMBER_COLUMN; j++)
        {
            // On initialise dans le tableau jeu.boxes les positions en pixel des cases
            jeu.boxes[i][j].posPixel.x = SPACE_LENGTH + i*(BOX_WIDTH+SPACE_LENGTH);
            jeu.boxes[i][j].posPixel.y = TOP_LENGTH + SPACE_LENGTH + j*(BOX_HEIGHT+SPACE_LENGTH);
        }
    }

    return jeu;
}

void getCursorIndex(GameState game, int *positionX, int *positionY, bool *mouvementEffectue)
{
    // On récupère la position en pixels de la souris
    int positionCursorX, positionCursorY;
    SDL_GetMouseState(&positionCursorX, &positionCursorY);

    int caseX, caseY;
    int startIndexLine, endIndexLine, startIndexColumn, endIndexColumn;
    // Gestion des indices de lignes
    if (*positionX == 0)
    {
        startIndexLine = 0;
        endIndexLine = 1;
    }
    else if (*positionX == BOX_NUMBER_LINE)
    {
        startIndexLine = 0;
        endIndexLine = BOX_NUMBER_LINE;
    }
    else
    {
        startIndexLine = *positionX - 1;
        endIndexLine = *positionX + 1;
    }
    // Gestion des indices de colonnes    
    if (*positionY == 0)
    {
        startIndexColumn = 0;
        endIndexColumn = 1;
    }
    else if (*positionY == BOX_NUMBER_COLUMN)
    {
        startIndexColumn = 0;
        endIndexColumn = BOX_NUMBER_COLUMN;
    }
    else
    {
        startIndexColumn = *positionY - 1;
        endIndexColumn = *positionY + 1;
    }
    for (int i = startIndexLine - 1; i < endIndexLine + 1; i++)
    {
        for (int j = startIndexColumn - 1; j < endIndexColumn + 1; j++)
        {
            // On zappe le cas où on cliquerait sur la même case que là où le joueur se situe déjà
            // On zappe aussi les diagonales
            if ((i == *positionX -1 && j == *positionY) || (i == *positionX && j == *positionY - 1) || (i == *positionX + 1 && j == *positionY) || (i == *positionX && j == *positionY + 1))
            {
                // On transforme la position en pixel de la souris en position de case 
                caseX = game.boxes[i][j].posPixel.x;
                caseY = game.boxes[i][j].posPixel.y;
                if ((caseX - SPACE_LENGTH/2 <= positionCursorX && positionCursorX < caseX + BOX_WIDTH + SPACE_LENGTH/2) && (caseY - SPACE_LENGTH/2 <= positionCursorY && positionCursorY < caseY + BOX_HEIGHT + SPACE_LENGTH/2))
                {
                    *positionX = i;
                    *positionY = j;
                    *mouvementEffectue = true;
                    return;
                }
            }
        }
    }
}
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages)
{
    // On load l'image des joueurs
    SDL_Surface * imagePlayer1 = IMG_Load("images/player1.png");
    SDL_Texture * imgPlayer1 = SDL_CreateTextureFromSurface(renderer, imagePlayer1);
    (*allImages)[11] = imgPlayer1;
    SDL_FreeSurface(imagePlayer1);
    SDL_Surface * imagePlayer2 = IMG_Load("images/player2.png");
    SDL_Texture * imgPlayer2 = SDL_CreateTextureFromSurface(renderer, imagePlayer2);
    (*allImages)[12] = imgPlayer2;
    SDL_FreeSurface(imagePlayer2);
    // On load les images du plateau
    SDL_Surface * imageGridBoxes = IMG_Load("images/grid1.png");
    SDL_Texture * gridBoxes = SDL_CreateTextureFromSurface(renderer, imageGridBoxes);
    (*allImages)[10] = gridBoxes;
    SDL_FreeSurface(imageGridBoxes);
    // On load les images des barrières 
    SDL_Surface * imageBarrierHorizontal = IMG_Load("images/barrierHorizontal.png");
    SDL_Texture * barrierHorizontal = SDL_CreateTextureFromSurface(renderer, imageBarrierHorizontal);
    (*allImages)[13] = barrierHorizontal;
    SDL_FreeSurface(imageBarrierHorizontal);
    SDL_Surface * imageBarrierVertical = IMG_Load("images/barrierVertical.png");
    SDL_Texture * barrierVertical = SDL_CreateTextureFromSurface(renderer, imageBarrierVertical);
    (*allImages)[14] = barrierVertical;
    SDL_FreeSurface(imageBarrierVertical);
    // On load les images du nombre de barrières restantes
    SDL_Surface * image0 = IMG_Load("images/0.png");
    SDL_Texture * img0 = SDL_CreateTextureFromSurface(renderer, image0);
    (*allImages)[0] = img0;
    SDL_FreeSurface(image0);
    SDL_Surface * image1 = IMG_Load("images/1.png");
    SDL_Texture * img1 = SDL_CreateTextureFromSurface(renderer, image1);
    (*allImages)[1] = img1;
    SDL_FreeSurface(image1);
    SDL_Surface * image2 = IMG_Load("images/2.png");
    SDL_Texture * img2 = SDL_CreateTextureFromSurface(renderer, image2);
    (*allImages)[2] = img2;
    SDL_FreeSurface(image2);
    SDL_Surface * image3 = IMG_Load("images/3.png");
    SDL_Texture * img3 = SDL_CreateTextureFromSurface(renderer, image3);
    (*allImages)[3] = img3;
    SDL_FreeSurface(image3);
    SDL_Surface * image4 = IMG_Load("images/4.png");
    SDL_Texture * img4 = SDL_CreateTextureFromSurface(renderer, image4);
    (*allImages)[4] = img4;
    SDL_FreeSurface(image4);
    SDL_Surface * image5 = IMG_Load("images/5.png");
    SDL_Texture * img5 = SDL_CreateTextureFromSurface(renderer, image5);
    (*allImages)[5] = img5;
    SDL_FreeSurface(image5);
    SDL_Surface * image6 = IMG_Load("images/6.png");
    SDL_Texture * img6 = SDL_CreateTextureFromSurface(renderer, image6);
    (*allImages)[6] = img6;
    SDL_FreeSurface(image6);
    SDL_Surface * image7 = IMG_Load("images/7.png");
    SDL_Texture * img7 = SDL_CreateTextureFromSurface(renderer, image7);
    (*allImages)[7] = img7;
    SDL_FreeSurface(image7);
    SDL_Surface * image8 = IMG_Load("images/8.png");
    SDL_Texture * img8 = SDL_CreateTextureFromSurface(renderer, image8);
    (*allImages)[8] = img8;
    SDL_FreeSurface(image8);
    SDL_Surface * image9 = IMG_Load("images/9.png");
    SDL_Texture * img9 = SDL_CreateTextureFromSurface(renderer, image9);
    (*allImages)[9] = img9;
    SDL_FreeSurface(image9);
    // On load l'image du curseur
    SDL_Surface * imageCursor = IMG_Load("images/cursor.png");
    SDL_Texture * Cursor = SDL_CreateTextureFromSurface(renderer, imageCursor);
    (*allImages)[15] = Cursor;
    SDL_FreeSurface(imageCursor);
}

void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, GameState Jeu) 
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // On dessine les cases
    for (int i = 0; i < BOX_NUMBER_LINE; i++) 
    {
        for (int j = 0; j < BOX_NUMBER_COLUMN; j++)
        {
            SDL_Rect dstRectGrid = {SPACE_LENGTH + i*(BOX_WIDTH+SPACE_LENGTH), TOP_LENGTH + SPACE_LENGTH + j*(BOX_HEIGHT+SPACE_LENGTH), BOX_WIDTH, BOX_HEIGHT};
            SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectGrid);
        }
    }
    // On dessine le haut du plateau 
    SDL_Rect dstRectHaut = {0, 0, WINDOW_WIDTH, TOP_LENGTH};
    SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectHaut);
    // On dessine le bas du plateau 
    SDL_Rect dstRectBas = {0, TOP_LENGTH + SPACE_LENGTH + BOX_NUMBER_COLUMN*(BOX_HEIGHT+SPACE_LENGTH), WINDOW_WIDTH, BOTTOM_LENGTH};
    SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectBas);
    // On dessine les joueurs
    // Joueur 1 - Humain
    int player1Width, player1Height;
    SDL_QueryTexture(allImages[11], NULL, NULL, &player1Width, &player1Height);
    int player1positionXpixels = Jeu.boxes[Jeu.players[0].pos.x][Jeu.players[0].pos.y].posPixel.x;
    int player1positionYpixels = Jeu.boxes[Jeu.players[0].pos.x][Jeu.players[0].pos.y].posPixel.y;
    SDL_Rect dstRectPlayer1 = {player1positionXpixels, player1positionYpixels, BOX_WIDTH, BOX_HEIGHT};
    SDL_RenderCopy(renderer, allImages[11], NULL, &dstRectPlayer1);
    // Joueur 2 - Ordi
    int player2Width, player2Height;
    SDL_QueryTexture(allImages[12], NULL, NULL, &player2Width, &player2Height);
    int player2positionXpixels = Jeu.boxes[Jeu.players[1].pos.x][Jeu.players[1].pos.y].posPixel.x;
    int player2positionYpixels = Jeu.boxes[Jeu.players[1].pos.x][Jeu.players[1].pos.y].posPixel.y;
    SDL_Rect dstRectPlayer2 = {player2positionXpixels, player2positionYpixels, BOX_WIDTH, BOX_HEIGHT};
    SDL_RenderCopy(renderer, allImages[12], NULL, &dstRectPlayer2);
    // On dessine les barrières en bas du plateau 
    // Barrière Verticale
    int barrierVerticalWidth, barrierVerticalHeight;
    SDL_QueryTexture(allImages[14], NULL, NULL, &barrierVerticalWidth, &barrierVerticalHeight); 
    float barrierVerticalX = WINDOW_WIDTH - (3*SPACE_LENGTH+BOX_WIDTH)+ barrierVerticalWidth/2;
    int barrierVerticalY = WINDOW_HEIGHT - 125;
    SDL_Rect dstRectbarrierVertical = {barrierVerticalX, barrierVerticalY, SPACE_LENGTH, 100};
    SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectbarrierVertical);
    // Barrière Horizontale
    int barrierHorizontalWidth, barrierHorizontalHeight;
    SDL_QueryTexture(allImages[13], NULL, NULL, &barrierHorizontalWidth, &barrierHorizontalHeight); 
    float barrierHorizontalX = SPACE_LENGTH + BOX_WIDTH/2;
    int barrierHorizontalY = WINDOW_HEIGHT - 85;
    SDL_Rect dstRectbarrierHorizontal = {barrierHorizontalX, barrierHorizontalY, 2*(BOX_WIDTH+SPACE_LENGTH), 20};
    SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectbarrierHorizontal);
    // Affichage du nombre de barrières restantes pour le joueur 1
    if (Jeu.players[0].barriersLeft == 10)
    {
        // Le 1 de 10
        int number1Width, number1Height;
        SDL_QueryTexture(allImages[1], NULL, NULL, &number1Width, &number1Height); 
        float number1X = 316 - 4*number1Width;
        int number1Y = WINDOW_HEIGHT - 125;
        SDL_Rect dstRectNumber1 = {number1X, number1Y, 4*number1Width, 100};
        SDL_RenderCopy(renderer, allImages[1], NULL, &dstRectNumber1);
        // Le 0 de 10
        int number0Width, number0Height;
        SDL_QueryTexture(allImages[0], NULL, NULL, &number0Width, &number0Height); 
        float number0X = 316;
        int number0Y = WINDOW_HEIGHT - 125;
        SDL_Rect dstRectNumber0 = {number0X, number0Y, 4*number0Width, 100};
        SDL_RenderCopy(renderer, allImages[0], NULL, &dstRectNumber0);
    }
    else
    {
        // On dessine le nombre
        int numberWidth, numberHeight;
        SDL_QueryTexture(allImages[Jeu.players[0].barriersLeft], NULL, NULL, &numberWidth, &numberHeight); 
        float numberX = 316 - 2*numberWidth;
        int numberY = WINDOW_HEIGHT - 125;
        SDL_Rect dstRectNumber = {numberX, numberY, 4*numberWidth, 100};
        SDL_RenderCopy(renderer, allImages[Jeu.players[0].barriersLeft], NULL, &dstRectNumber);
    }

}

void initSDL(SDL_Window **window, SDL_Renderer **renderer) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    *window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) 
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
}