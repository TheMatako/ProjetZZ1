#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "structureQuoridor.h"




GameState initGame(SDL_Texture* image_barrier)
{
    GameState jeu = 
    {
        .players = {{.pos = {4, 8}, .barriersLeft = 10}, {.pos = {4, 0}, .barriersLeft = 10}},
        .barrierCount = 0,
        .isGameRunning = true,
        .playerTurn = 0,
        .isDragging = 0,
        .draggedBarrier = NULL
    };

    memset(jeu.barriers, 0, sizeof(jeu.barriers)); // Initialise toutes les barrières à 0
    for (int i = 0; i < 20; i++) 
    {
        if (i % 2 == 0) // Supposons une alternance pour l'exemple
        {  
            // Initialisation de la barrière verticale
            int barrierVerticalWidth, barrierVerticalHeight;
            SDL_QueryTexture(image_barrier, NULL, NULL, &barrierVerticalWidth, &barrierVerticalHeight);
            float barrierVerticalX = WINDOW_WIDTH - (3*SPACE_LENGTH+BOX_WIDTH)+ barrierVerticalWidth/2;
            int barrierVerticalY = WINDOW_HEIGHT - 125;
            jeu.barriers[i].rect.x = barrierVerticalX;
            jeu.barriers[i].rect.y = barrierVerticalY;
            jeu.barriers[i].rect.w = BARRIER_HEIGHT;  
            jeu.barriers[i].rect.h = BARRIER_WIDTH;          
            jeu.barriers[i].isHorizontal=false;
        } 
        else 
        {
            // Initialisation de la barrière horizontale
            float barrierHorizontalX = SPACE_LENGTH + BOX_WIDTH/2;
            int barrierHorizontalY = WINDOW_HEIGHT - 85;
            jeu.barriers[i].rect.x = barrierHorizontalX;
            jeu.barriers[i].rect.y = barrierHorizontalY;
            jeu.barriers[i].rect.w = BARRIER_WIDTH ;  
            jeu.barriers[i].rect.h = BARRIER_HEIGHT;       
            jeu.barriers[i].isHorizontal=true;                      
        }
    }


    for (int i = 0; i < BOX_NUMBER_COLUMN; i++) 
    {
        for (int j = 0; j < BOX_NUMBER_LINE; j++)
        {
            // On initialise dans le tableau jeu.boxes les positions en pixel des cases
            jeu.boxes[i][j].posPixel.x = SPACE_LENGTH + i*(BOX_WIDTH+SPACE_LENGTH);
            jeu.boxes[i][j].posPixel.y = TOP_LENGTH + SPACE_LENGTH + j*(BOX_HEIGHT+SPACE_LENGTH);
        }
    }

    return jeu;
}

void getCursorIndex(GameState game, int *positionX, int *positionY, bool *mouvementEffectue, int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE])
{
    // On r�cup�re la position en pixels de la souris
    int positionCursorX, positionCursorY;
    SDL_GetMouseState(&positionCursorX, &positionCursorY);
    for (int i = 0; i < BOX_NUMBER_COLUMN; i++)
    {
        for (int j = 0; j < BOX_NUMBER_LINE; j++)
        {   
            if (boxesPlayable[i][j] == 1)
            {
                int caseX = game.boxes[i][j].posPixel.x;
                int caseY = game.boxes[i][j].posPixel.y;
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

    char *filesName[] = {"./images/0.png", "./images/1.png", "./images/2.png", "./images/3.png", "./images/4.png",
                             "./images/5.png", "./images/6.png", "./images/7.png", "./images/8.png", "./images/9.png",
                             "./images/grid1.png", "./images/player1.png", "./images/player2.png", "./images/barrierHorizontal.png",
                             "./images/barrierVertical.png", "./images/cursor.png"};
    for (int i = 0; i < 16; i++)
    {
        SDL_Surface *imageSurface = IMG_Load(filesName[i]);
        if (!imageSurface)
        {
            printf("Impossible to load image %s ! SDL_image Error: %s\n", filesName[i], IMG_GetError());
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

void drawGame(SDL_Renderer *renderer, SDL_Texture **allImages, GameState Jeu, int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE]) 
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // On dessine les cases
    for (int i = 0; i < BOX_NUMBER_COLUMN; i++) 
    {
        for (int j = 0; j < BOX_NUMBER_LINE; j++)
        {
            SDL_Rect dstRectGrid = {SPACE_LENGTH + i*(BOX_WIDTH+SPACE_LENGTH), TOP_LENGTH + SPACE_LENGTH + j*(BOX_HEIGHT+SPACE_LENGTH), BOX_WIDTH, BOX_HEIGHT};
            SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectGrid);
        }
    }

    // Dessin de toutes les barrières placées
    for (int k = 0; k < BARRIER_NUMBER; k++) 
    {
        if (Jeu.barriers[k].isPlaced) 
        {
            
            for (int i = 0; i < BOX_NUMBER_COLUMN; i++)
            {
                for (int j = 0; j < BOX_NUMBER_LINE; j++)
                {
                    if ((Jeu.barriers[k].rect.x > Jeu.boxes[i][j].posPixel.x - SPACE_LENGTH && Jeu.barriers[k].rect.x < Jeu.boxes[i][j].posPixel.x + BOX_WIDTH + SPACE_LENGTH) && (Jeu.barriers[k].rect.y > Jeu.boxes[i][j].posPixel.y - 2*SPACE_LENGTH && Jeu.barriers[k].rect.y < Jeu.boxes[i][j].posPixel.y + SPACE_LENGTH))
                    {
                        printf("ici\n");
                        if (Jeu.barriers[k].isHorizontal)
                        {
                            Jeu.barriers[k].pos1.x = i;
                            Jeu.barriers[k].pos1.y = j;
                            Jeu.barriers[k].pos2.x = i+1;
                            Jeu.barriers[k].pos2.y = j;
                            SDL_Rect dstRectBarrier = { Jeu.boxes[Jeu.barriers[k].pos1.x][Jeu.barriers[k].pos1.y].posPixel.x,Jeu.boxes[Jeu.barriers[k].pos1.x][Jeu.barriers[k].pos1.y].posPixel.y-Jeu.barriers[k].rect.h, Jeu.barriers[k].rect.w, Jeu.barriers[k].rect.h };

                            if (Jeu.barriers[k].isHorizontal) SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectBarrier);

                            else SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectBarrier);    
                        }
                        else
                        {
                            Jeu.barriers[k].pos1.x = i+1;
                            Jeu.barriers[k].pos1.y = j+1;
                            Jeu.barriers[k].pos2.x = i+1;
                            Jeu.barriers[k].pos2.y = j+2;
                            SDL_Rect dstRectBarrier = { Jeu.boxes[Jeu.barriers[k].pos1.x][Jeu.barriers[k].pos1.y].posPixel.x-Jeu.barriers[k].rect.w,Jeu.boxes[Jeu.barriers[k].pos1.x][Jeu.barriers[k].pos1.y].posPixel.y-Jeu.barriers[k].rect.h, Jeu.barriers[k].rect.w, Jeu.barriers[k].rect.h };

                            if (Jeu.barriers[k].isHorizontal) SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectBarrier);

                            else SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectBarrier);
                        }
                        
                    }
                }
            }
        }
    }
    
    // Dessiner les cases jouables en vert
    if (Jeu.playerTurn == 0)
    {
        getPositionPlayable(Jeu, &Jeu.players[Jeu.playerTurn].pos.x, &Jeu.players[Jeu.playerTurn].pos.y, boxesPlayable);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 125);
        for (int i = 0; i < BOX_NUMBER_COLUMN; i++) 
        {
            for (int j = 0; j < BOX_NUMBER_LINE; j++)
            {
                if (boxesPlayable[i][j] == 1)
                {
                    int posX = Jeu.boxes[i][j].posPixel.x;
                    int posY = Jeu.boxes[i][j].posPixel.y;
                    SDL_Rect dstRectPlayable = { posX, posY, BOX_WIDTH, BOX_HEIGHT};
                    SDL_RenderFillRect(renderer, &dstRectPlayable);
                }
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // On dessine le haut du plateau 
    SDL_Rect dstRectHaut = {0, 0, WINDOW_WIDTH, TOP_LENGTH};
    SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectHaut);
    // On dessine le bas du plateau 
    SDL_Rect dstRectBas = {0, TOP_LENGTH + SPACE_LENGTH + BOX_NUMBER_LINE*(BOX_HEIGHT+SPACE_LENGTH), WINDOW_WIDTH, BOTTOM_LENGTH};
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
    // On dessine les barri�res en bas du plateau 
    // Barri�re Verticale
    int barrierVerticalWidth, barrierVerticalHeight;
    SDL_QueryTexture(allImages[14], NULL, NULL, &barrierVerticalWidth, &barrierVerticalHeight); 
    float barrierVerticalX = WINDOW_WIDTH - (3*SPACE_LENGTH+BOX_WIDTH)+ barrierVerticalWidth/2;
    int barrierVerticalY = WINDOW_HEIGHT - 125;
    SDL_Rect dstRectbarrierVertical = {barrierVerticalX, barrierVerticalY, SPACE_LENGTH, 100};
    SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectbarrierVertical);
    // Barri�re Horizontale
    int barrierHorizontalWidth, barrierHorizontalHeight;
    SDL_QueryTexture(allImages[13], NULL, NULL, &barrierHorizontalWidth, &barrierHorizontalHeight); 
    float barrierHorizontalX = SPACE_LENGTH + BOX_WIDTH/2;
    int barrierHorizontalY = WINDOW_HEIGHT - 85;
    SDL_Rect dstRectbarrierHorizontal = {barrierHorizontalX, barrierHorizontalY, 2*(BOX_WIDTH+SPACE_LENGTH), 20};
    SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectbarrierHorizontal);
    // Affichage du nombre de barri�res restantes pour le joueur 1
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

    // Dessiner les cases jouables en vert
    getPositionPlayable(Jeu, &Jeu.players[Jeu.playerTurn].pos.x, &Jeu.players[Jeu.playerTurn].pos.y, boxesPlayable);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 125);
    for (int i = 0; i < BOX_NUMBER_COLUMN; i++) {
        for (int j = 0; j < BOX_NUMBER_LINE; j++) {
            if (boxesPlayable[i][j] == 1) {
                int posX = Jeu.boxes[i][j].posPixel.x;
                int posY = Jeu.boxes[i][j].posPixel.y;
                SDL_Rect dstRectPlayable = { posX, posY, BOX_WIDTH, BOX_HEIGHT};
                SDL_RenderFillRect(renderer, &dstRectPlayable);
            }
        }
    }
    
    if (Jeu.isDragging == 1)
    {
        int posCursorX; int posCursorY;
        SDL_GetMouseState(&posCursorX, &posCursorY);
        posCursorX -= SPACE_LENGTH/2 - BOX_WIDTH;
        posCursorX -= SPACE_LENGTH/2;
        SDL_Rect dstRectDragging = {posCursorX, posCursorY, 2*BOX_WIDTH+SPACE_LENGTH, SPACE_LENGTH};
        SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectDragging);
    }
    else if (Jeu.isDragging == 2)
    {
        int posCursorX; int posCursorY;
        SDL_GetMouseState(&posCursorX, &posCursorY);
        posCursorX -= SPACE_LENGTH/2;
        posCursorX -= SPACE_LENGTH/2 - BOX_HEIGHT;
        SDL_Rect dstRectDragging = {posCursorX, posCursorY, SPACE_LENGTH, 2*BOX_HEIGHT+SPACE_LENGTH};
        SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectDragging);
    }



}

void initSDL(SDL_Window **window, SDL_Renderer **renderer) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    *window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
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

// Retourne 0 si pas de barri�re 1 si y'a 
int isThereBarrier(GameState game, int otherPlayerPosX, int otherPlayerPosY, int direction)
{
    // On r�cup�re le nombre de barri�re pos�es 


    // Direction : left = 1, up = 2, right = 3, down = 4
    for (int i = 0; i < 20; i++)
    {
        if (game.barriers[i].isPlaced)
        {
            Barrier barrier = game.barriers[i];
            if (direction == 1) // left
            {
                if ( (barrier.pos1.x == otherPlayerPosX - 1 && barrier.pos1.y == otherPlayerPosY) || (barrier.pos2.x == otherPlayerPosX - 1 && barrier.pos2.y == otherPlayerPosY))
                {
                    return 1;
                }
            }
            else if (direction == 2) // up
            {
                if ( (barrier.pos1.x == otherPlayerPosX && barrier.pos1.y == otherPlayerPosY - 1) || (barrier.pos2.x == otherPlayerPosX && barrier.pos2.y == otherPlayerPosY - 1))
                {
                    return 1;
                }

            }
            else if (direction == 3) // right
            {
                if ( (barrier.pos1.x == otherPlayerPosX && barrier.pos1.y == otherPlayerPosY) || (barrier.pos2.x == otherPlayerPosX && barrier.pos2.y == otherPlayerPosY))
                {
                    return 1;
                }
            }
            else if(direction == 4) // down
            {
                if ( (barrier.pos1.x == otherPlayerPosX && barrier.pos1.y == otherPlayerPosY) || (barrier.pos2.x == otherPlayerPosX && barrier.pos2.y == otherPlayerPosY))
                {
                    return 1;
                }

            }
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

void getPositionPlayable(GameState game, int *positionX, int *positionY, int boxesPlayable[BOX_NUMBER_COLUMN][BOX_NUMBER_LINE])
{
    int up = 1, down = 1, right = 1, left = 1;
    int diagUpLeft = 0, diagUpRight = 0, diagDownLeft = 0, diagDownRight = 0;
    Player otherPlayer = game.players[(game.playerTurn + 1)%2];
    int otherPlayerPosX = otherPlayer.pos.x, otherPlayerPosY = otherPlayer.pos.y;
    // Gestion des positions en bord de tableau
    // Gestion des indices de lignes
    if (*positionX == 0) left = 0;
    else if (*positionX == BOX_NUMBER_COLUMN - 1) right = 0;
    // Gestion des indices de colonnes    
    if (*positionY == 0) up = 0;
    else if (*positionY == BOX_NUMBER_LINE - 1) down = 0;
    // Gestion des cas o� le joueur nous bloque et qu'on doive sauter par dessus
    
    // 4�me argument de isThereBarrier() = direction
    // Direction : left = 1, up = 2, right = 3, down = 4

    // D�placement � droite
    if (right && otherPlayerPosX == *positionX + 1 && otherPlayerPosY == *positionY)
    {
        if (isThereBarrier(game, otherPlayerPosX+1, otherPlayerPosY, 3) == 0) 
        {
            if (*positionX + 2 < BOX_NUMBER_COLUMN) right = 2;
            else right = 0;
        }
        // Gestion des d�placements en diagonale
        else
        {
            right = 0;
            if (up && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 2) == 0) diagUpRight = 1;
            else if (down && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 4) == 0) diagDownRight = 1;
        }
    }
    // D�placement � gauche
    if (left && otherPlayerPosX == *positionX - 1 && otherPlayerPosY == *positionY)
    {
        if (isThereBarrier(game, otherPlayerPosX-1, otherPlayerPosY, 1) == 0) 
        {
            if (*positionX - 2 >= 0) left = 2;
            else left = 0;
        }
        // Gestion des d�placements en diagonale
        else
        {
            left = 0;
            if (up && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, up) == 0) diagUpLeft = 1;
            else if (down && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 4) == 0) diagDownLeft = 1;
        }
    }
    // D�placement en haut
    if (up && otherPlayerPosY == *positionY - 1 && otherPlayerPosX == *positionX )
    {
        if (isThereBarrier(game, otherPlayerPosX, otherPlayerPosY-1, 2) == 0)
        {
            if (*positionY - 2 >= 0) up = 2;
            else up = 0;
        }
        // Gestion des d�placements en diagonale
        else
        {
            up = 0;
            if (right && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 3) == 0) diagUpRight = 1;
            else if (left && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 1) == 0) diagUpLeft = 1;
        }
    }
    // D�placement en bas
    if (down && otherPlayerPosY == *positionY + 1 && otherPlayerPosX == *positionX)
    {
        if (isThereBarrier(game, otherPlayerPosX, otherPlayerPosY+1, 4) == 0)
        {
            if (*positionY + 2 < BOX_NUMBER_COLUMN) down = 2;
            else down = 0;
        }
        // Gestion des d�placements en diagonale
        else
        {
            down = 0;
            if (left && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 1) == 0) diagDownLeft = 1;
            else if (right && isThereBarrier(game, otherPlayerPosX, otherPlayerPosY, 3) == 0) diagDownRight = 1;
        }
    }
    // Remplissage de boxesPlayable
    if (diagUpLeft)
    {
        if (*positionX > 0 && *positionY > 0) boxesPlayable[*positionX-1][*positionY-1] = 1;
    } 
    if (diagUpRight)
    {
        if (*positionX < BOX_NUMBER_LINE -1  && *positionY > 0) boxesPlayable[*positionX+1][*positionY-1] = 1;        
    }
    if (diagDownLeft)
    {
        if (*positionX > 0 && *positionY < BOX_NUMBER_COLUMN -1) boxesPlayable[*positionX-1][*positionY+1] = 1;
    }
    if (diagDownRight)
    {
        if (*positionX < BOX_NUMBER_LINE - 1 && *positionY < BOX_NUMBER_LINE - 1) boxesPlayable[*positionX+1][*positionY+1] = 1;
    }
    if (up)
    {
        if (*positionY > 0) boxesPlayable[*positionX][*positionY-up] = 1;
    }
    if (down)
    {
        if (*positionY < BOX_NUMBER_COLUMN - 1) boxesPlayable[*positionX][*positionY+down] = 1;
    }     
    if (left)
    {
        if (*positionX > 0) boxesPlayable[*positionX-left][*positionY] = 1;
    }     
    if (right)
    {
        if (*positionX < BOX_NUMBER_LINE - 1) boxesPlayable[*positionX+right][*positionY] = 1;
    }     
}