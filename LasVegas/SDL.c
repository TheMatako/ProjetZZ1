#include "SDL.h"
#include <SDL2/SDL_ttf.h>



void drawText(SDL_Renderer *renderer, GameState game, TTF_Font *font)
{
    SDL_Color textColor = {255, 255, 255, 0};
    for (int i = 0; i < NUMBER_PLAYERS; i++)
    {
        char text[100];
        snprintf(text, 100, "Money Joueur %d: %d0K", i, game.player[i].totalMoney);
        SDL_Surface *surface;
        surface = TTF_RenderText_Solid(font, text, textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect dstRect = {30, 30 + 60*i, 450, 40};
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_DestroyTexture(texture);
    }
    char text[100];
    snprintf(text, 100, "Tour numero %d, au joueur %d de jouer", game.turn, game.playerTurn);
    SDL_Surface *surface2;
    surface2 = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface2);
    SDL_Rect dstRect2 = {1170, 30, 480, 40};
    SDL_RenderCopy(renderer, texture2, NULL, &dstRect2);
    SDL_DestroyTexture(texture2);

    char text1[100];
    snprintf(text1, 100, "Round numero %d", game.round);
    SDL_Surface *surface3;
    surface3 = TTF_RenderText_Solid(font, text1, textColor);
    SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    SDL_FreeSurface(surface3);
    SDL_Rect dstRect3 = {1400, 100, 250, 40};
    SDL_RenderCopy(renderer, texture3, NULL, &dstRect3);
    SDL_DestroyTexture(texture3);

}



int checkHitbox(GameState game)
{
    // On r�cup�re la position en pixels de la souris
    int positionCursorX, positionCursorY;
    SDL_GetMouseState(&positionCursorX, &positionCursorY);

    for (int i = 0; i < 6; i++)
    {
        int posX = game.casino[i].rectDice[0];
        int posY = game.casino[i].rectDice[1];
        int width = game.casino[i].rectDice[2];
        int height = game.casino[i].rectDice[3];
        if ((positionCursorX > posX && positionCursorX < posX + width) && (positionCursorY > posY && positionCursorY < posY + height))
        {
            if (occurrences(game.player[game.playerTurn].currentThrow, game.player[game.playerTurn].dicesLeft, i+1) > 0)
                return i+1;
            else 
                return 0;
        }
    }
    return 0;
}

void drawDicesOverCasino(GameState game, SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int player)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < game.casino[i].dicesPlaced[player]; j++)
        {
            int posX = game.casino[i].rectCasino[0] + 43 *j;
            int posY = game.casino[i].rectCasino[1] + 43*player;


            SDL_Rect dstRectDice = {posX,posY,42,42};
            SDL_RenderCopy(renderer, dicesPlayersImages[i], NULL, &dstRectDice);
        }
    }
}



// Fonctions de dessin
void drawGame(GameState game, SDL_Renderer *renderer, SDL_Texture **allImages, SDL_Texture **dicesImages, SDL_Texture ***dicesPlayersImages, TTF_Font *font)
{
    drawBackground(game, renderer, allImages);
    drawLines(renderer);
    drawDiceImages(renderer, dicesImages);
    drawCasino(renderer, allImages);
    drawBanknotes(game, renderer, allImages);
    drawPlayersThrows(game, renderer, dicesPlayersImages[game.playerTurn]);
    for (int i = 0; i < NUMBER_PLAYERS; i++)
    {
        drawDicesOverCasino(game, renderer, dicesPlayersImages[i], i);
    }
    drawText(renderer, game, font);
}




void drawBackground(GameState game, SDL_Renderer *renderer, SDL_Texture **allImages)
{
    SDL_Rect dstRectDevanture = {0,0,1680,255};
    SDL_RenderCopy(renderer, allImages[16], NULL, &dstRectDevanture);
    for (int i = 0; i < 6; i++)
    {
        int posX = game.casino[i].rectCasino[0];
        int posY = game.casino[i].rectCasino[1];
        int width = game.casino[i].rectCasino[2];
        int height = game.casino[i].rectCasino[3]*2;
        SDL_Rect dstRectBackground = {posX,posY,width,height};
        SDL_RenderCopy(renderer, allImages[17], NULL, &dstRectBackground);
    }
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
    SDL_Rect dstRectCasino1 = {240,255,400,180};
    SDL_RenderCopy(renderer, allImages[10], NULL, &dstRectCasino1);

    // Casino 2 :
    SDL_Rect dstRectCasino2 = {640,255,400,180};
    SDL_RenderCopy(renderer, allImages[11], NULL, &dstRectCasino2);
    
    // Casino 3 :
    SDL_Rect dstRectCasino3 = {1040,255,400,180};
    SDL_RenderCopy(renderer, allImages[12], NULL, &dstRectCasino3);

    // Casino 4 :
    SDL_Rect dstRectCasino4 = {240,615,400,180};
    SDL_RenderCopy(renderer, allImages[13], NULL, &dstRectCasino4);

    // Casino 5 :
    SDL_Rect dstRectCasino5 = {640,615,400,180};
    SDL_RenderCopy(renderer, allImages[14], NULL, &dstRectCasino5);

    // Casino 6 :
    SDL_Rect dstRectCasino6 = {1040,615,400,180};
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
void drawBanknotes(GameState game, SDL_Renderer *renderer, SDL_Texture **allImages)
{

    for (int i = 0; i < 6; i ++) // On parcourt les casinos
    {
        bubbleTea(game.casino[i].associatedValues, MAX_BILLETS_PER_CASINO, 1);
        for (int j = 0; j < MAX_BILLETS_PER_CASINO; j++) // On va parcourir les billets associés au casino
        {
            int value = game.casino[i].associatedValues[j];
            
            if (value > 0)
            {
                SDL_Rect dstRectBanknote = {game.casino[i].rectCasino[0]+j*100,game.casino[i].rectCasino[1]+game.casino[i].rectCasino[3],100,game.casino[i].rectCasino[3]};
                SDL_RenderCopy(renderer, allImages[game.casino[i].associatedValues[j]], NULL, &dstRectBanknote);
            }
        }
    }
}
void drawPlayersThrows(GameState game, SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages)
{
    for (int i = 1; i < 7; i++)
    {
        int column = i/4;
        int line = (i-1)%3;
        //printf("Dé n°%d, column = %d, line = %d\n", i, column, line);
        int numberOf = occurrences(game.player[game.playerTurn].currentThrow, game.player[game.playerTurn].dicesLeft, i);
        if (numberOf == 1)
        {
            draw1Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
        else if (numberOf == 2)
        {
            draw2Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
        else if (numberOf == 3)
        {
            draw3Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
        else if (numberOf == 4)
        {
            draw4Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
        else if (numberOf == 5)
        {
            draw5Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);

        }
        else if (numberOf == 6)
        {
            draw6Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
        else if (numberOf == 7)
        {
            draw7Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
        else if (numberOf == 8)
        {
            draw8Dices(renderer, dicesPlayersImages, column*1440, 255+line*240, i-1);
        }
    }
}
void draw1Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePositionX = positionX + (240-42)/2;
        int dicePositionY = positionY + (240-42)/2;
        SDL_Rect dstRectDice = {dicePositionX, dicePositionY, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRectDice);
    }

}
void draw2Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + 5*(240-42)/8;
        int dicePosition1Y = positionY + 3*(240-42)/8;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 3*(240-42)/8;
        int dicePosition2Y = positionY + 5*(240-42)/8;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);
    }

}
void draw3Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + (240-42)/4;
        int dicePosition1Y = positionY + (240-42)/4;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 2*(240-42)/4;
        int dicePosition2Y = positionY + 2*(240-42)/4;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);
        
        int dicePosition3X = positionX + 3*(240-42)/4;
        int dicePosition3Y = positionY + 3*(240-42)/4;
        SDL_Rect dstRect3Dice = {dicePosition3X, dicePosition3Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect3Dice);
    }

}
void draw4Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + 3*(240-42)/10;
        int dicePosition1Y = positionY + 3*(240-42)/10;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 7*(240-42)/10;
        int dicePosition2Y = positionY + 7*(240-42)/10;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);

        int dicePosition3X = positionX + 7*(240-42)/10;
        int dicePosition3Y = positionY + 3*(240-42)/10;
        SDL_Rect dstRect3Dice = {dicePosition3X, dicePosition3Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect3Dice);

        int dicePosition4X = positionX + 3*(240-42)/10;
        int dicePosition4Y = positionY + 7*(240-42)/10;
        SDL_Rect dstRect4Dice = {dicePosition4X, dicePosition4Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect4Dice);
    }

}
void draw5Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + (240-42)/4;
        int dicePosition1Y = positionY + (240-42)/4;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 2*(240-42)/4;
        int dicePosition2Y = positionY + 2*(240-42)/4;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);
        
        int dicePosition3X = positionX + 3*(240-42)/4;
        int dicePosition3Y = positionY + 3*(240-42)/4;
        SDL_Rect dstRect3Dice = {dicePosition3X, dicePosition3Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect3Dice);


        int dicePosition4X = positionX + 3*(240-42)/4;
        int dicePosition4Y = positionY + (240-42)/4;
        SDL_Rect dstRect4Dice = {dicePosition4X, dicePosition4Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect4Dice);

        int dicePosition5X = positionX + (240-42)/4;
        int dicePosition5Y = positionY + 3*(240-42)/4;
        SDL_Rect dstRect5Dice = {dicePosition5X, dicePosition5Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect5Dice);
    }

}
void draw6Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + 4*(240-42)/12;
        int dicePosition1Y = positionY + 3*(240-42)/12;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 4*(240-42)/12;
        int dicePosition2Y = positionY + 6*(240-42)/12;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);

        int dicePosition3X = positionX + 4*(240-42)/12;
        int dicePosition3Y = positionY + 9*(240-42)/12;
        SDL_Rect dstRect3Dice = {dicePosition3X, dicePosition3Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect3Dice);

        int dicePosition4X = positionX + 8*(240-42)/12;
        int dicePosition4Y = positionY + 3*(240-42)/12;
        SDL_Rect dstRect4Dice = {dicePosition4X, dicePosition4Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect4Dice);

        int dicePosition5X = positionX + 8*(240-42)/12;
        int dicePosition5Y = positionY + 6*(240-42)/12;
        SDL_Rect dstRect5Dice = {dicePosition5X, dicePosition5Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect5Dice);

        int dicePosition6X = positionX + 8*(240-42)/12;
        int dicePosition6Y = positionY + 9*(240-42)/12;
        SDL_Rect dstRect6Dice = {dicePosition6X, dicePosition6Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect6Dice);
    }

}
void draw7Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + 6*(240-42)/24;
        int dicePosition1Y = positionY + 3*(240-42)/12;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 6*(240-42)/24;
        int dicePosition2Y = positionY + 6*(240-42)/12;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);

        int dicePosition3X = positionX + 6*(240-42)/24;
        int dicePosition3Y = positionY + 9*(240-42)/12;
        SDL_Rect dstRect3Dice = {dicePosition3X, dicePosition3Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect3Dice);

        int dicePosition4X = positionX + 18*(240-42)/24;
        int dicePosition4Y = positionY + 3*(240-42)/12;
        SDL_Rect dstRect4Dice = {dicePosition4X, dicePosition4Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect4Dice);

        int dicePosition5X = positionX + 18*(240-42)/24;
        int dicePosition5Y = positionY + 6*(240-42)/12;
        SDL_Rect dstRect5Dice = {dicePosition5X, dicePosition5Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect5Dice);

        int dicePosition6X = positionX + 18*(240-42)/24;
        int dicePosition6Y = positionY + 9*(240-42)/12;
        SDL_Rect dstRect6Dice = {dicePosition6X, dicePosition6Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect6Dice);

        int dicePosition7X = positionX + (240-42)/2;
        int dicePosition7Y = positionY + (240-42)/2;
        SDL_Rect dstRect7Dice = {dicePosition7X, dicePosition7Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect7Dice);


    }

}
void draw8Dices(SDL_Renderer *renderer, SDL_Texture **dicesPlayersImages, int positionX, int positionY, int dice)
{
    if (dicesPlayersImages[dice] == NULL)
    {
        fprintf(stderr, "Texture for dice %d is NULL.\n", dice);
        return;
    }
    else
    {
        int dicePosition1X = positionX + 6*(240-42)/24;
        int dicePosition1Y = positionY + 3*(240-42)/12;
        SDL_Rect dstRect1Dice = {dicePosition1X, dicePosition1Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect1Dice);

        int dicePosition2X = positionX + 6*(240-42)/24;
        int dicePosition2Y = positionY + 6*(240-42)/12;
        SDL_Rect dstRect2Dice = {dicePosition2X, dicePosition2Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect2Dice);

        int dicePosition3X = positionX + 6*(240-42)/24;
        int dicePosition3Y = positionY + 9*(240-42)/12;
        SDL_Rect dstRect3Dice = {dicePosition3X, dicePosition3Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect3Dice);

        int dicePosition4X = positionX + 18*(240-42)/24;
        int dicePosition4Y = positionY + 3*(240-42)/12;
        SDL_Rect dstRect4Dice = {dicePosition4X, dicePosition4Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect4Dice);

        int dicePosition5X = positionX + 18*(240-42)/24;
        int dicePosition5Y = positionY + 6*(240-42)/12;
        SDL_Rect dstRect5Dice = {dicePosition5X, dicePosition5Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect5Dice);

        int dicePosition6X = positionX + 18*(240-42)/24;
        int dicePosition6Y = positionY + 9*(240-42)/12;
        SDL_Rect dstRect6Dice = {dicePosition6X, dicePosition6Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect6Dice);

        int dicePosition7X = positionX + (240-42)/2;
        int dicePosition7Y = positionY + 5*(240-42)/12;
        SDL_Rect dstRect7Dice = {dicePosition7X, dicePosition7Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect7Dice);

        int dicePosition8X = positionX + (240-42)/2;
        int dicePosition8Y = positionY + 8*(240-42)/12;
        SDL_Rect dstRect8Dice = {dicePosition8X, dicePosition8Y, 42, 42};
        SDL_RenderCopy(renderer, dicesPlayersImages[dice], NULL, &dstRect8Dice);
    }


}
// Fonctions de chargement des images
void loadTextures(SDL_Renderer *renderer, SDL_Texture ***allImages)
{

    char *filesName[] = {"./images/billetFaceCache.png", "./images/billet10.png", "./images/billet20.png", "./images/billet30.png", "./images/billet40.png",
                             "./images/billet50.png", "./images/billet60.png", "./images/billet70.png", "./images/billet80.png", "./images/billet90.png",
                             "./images/casino1.png", "./images/casino2.png", "./images/casino3.png", "./images/casino4.png",
                             "./images/casino5.png", "./images/casino6.png", "./images/devanture2.png","./images/casinoBackground.png"};
    for (int i = 0; i < 18; i++)
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

void loadDicesPlayersTextures(SDL_Renderer *renderer, SDL_Texture ***dicesPlayersImages)
{
    if (NUMBER_PLAYERS > 0)
    {
        // Joueur 0 = bleu
        char *filesNameBleu[] = {"./images/de1bleu.png","./images/de2bleu.png","./images/de3bleu.png","./images/de4bleu.png","./images/de5bleu.png","./images/de6bleu.png",};
        for (int i = 0; i < 6; i++)
        {
            SDL_Surface *imageSurface = IMG_Load(filesNameBleu[i]);
            if (!imageSurface)
            {
                printf("Impossible to load image %s ! SDL_image Error: %s\n", filesNameBleu[i], IMG_GetError());
                dicesPlayersImages[0][i] = NULL;
            }
            else
            {
                dicesPlayersImages[0][i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
                if (!dicesPlayersImages[0][i])
                {
                    printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesNameBleu[i], SDL_GetError());
                }
                SDL_FreeSurface(imageSurface);
            }
        }
        if (NUMBER_PLAYERS > 1)
        {
            // Joueur 1 = jaune
            char *filesNameJaune[] = {"./images/de1jaune.png","./images/de2jaune.png","./images/de3jaune.png","./images/de4jaune.png","./images/de5jaune.png","./images/de6jaune.png",};
            for (int i = 0; i < 6; i++)
            {
                SDL_Surface *imageSurface = IMG_Load(filesNameJaune[i]);
                if (!imageSurface)
                {
                    printf("Impossible to load image %s ! SDL_image Error: %s\n", filesNameJaune[i], IMG_GetError());
                    dicesPlayersImages[1][i] = NULL;
                }
                else
                {
                    dicesPlayersImages[1][i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
                    if (!dicesPlayersImages[1][i])
                    {
                        printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesNameJaune[i], SDL_GetError());
                    }
                    SDL_FreeSurface(imageSurface);
                }
            }   
            if (NUMBER_PLAYERS > 2)
            {
                // Joueur 2 = rouge
                char *filesNameRouge[] = {"./images/de1rouge.png","./images/de2rouge.png","./images/de3rouge.png","./images/de4rouge.png","./images/de5rouge.png","./images/de6rouge.png",};
                for (int i = 0; i < 6; i++)
                {
                    SDL_Surface *imageSurface = IMG_Load(filesNameRouge[i]);
                    if (!imageSurface)
                    {
                        printf("Impossible to load image %s ! SDL_image Error: %s\n", filesNameRouge[i], IMG_GetError());
                        dicesPlayersImages[2][i] = NULL;
                    }
                    else
                    {
                        dicesPlayersImages[2][i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
                        if (!dicesPlayersImages[2][i])
                        {
                            printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesNameRouge[i], SDL_GetError());
                        }
                        SDL_FreeSurface(imageSurface);
                    }
                }  
                if (NUMBER_PLAYERS > 3) 
                {
                    // Joueur 3 = vert
                    char *filesNameVert[] = {"./images/de1vert.png","./images/de2vert.png","./images/de3vert.png","./images/de4vert.png","./images/de5vert.png","./images/de6vert.png",};
                    for (int i = 0; i < 6; i++)
                    {
                        SDL_Surface *imageSurface = IMG_Load(filesNameVert[i]);
                        if (!imageSurface)
                        {
                            printf("Impossible to load image %s ! SDL_image Error: %s\n", filesNameVert[i], IMG_GetError());
                            dicesPlayersImages[3][i] = NULL;
                        }
                        else
                        {
                            dicesPlayersImages[3][i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
                            if (!dicesPlayersImages[3][i])
                            {
                                printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesNameVert[i], SDL_GetError());
                            }
                            SDL_FreeSurface(imageSurface);
                        }
                    }
                    if (NUMBER_PLAYERS > 4)
                    {
                        // Joueur 4 = violet
                        char *filesNameViolet[] = {"./images/de1violet.png","./images/de2violet.png","./images/de3violet.png","./images/de4violet.png","./images/de5violet.png","./images/de6violet.png",};
                        for (int i = 0; i < 6; i++)
                        {
                            SDL_Surface *imageSurface = IMG_Load(filesNameViolet[i]);
                            if (!imageSurface)
                            {
                                printf("Impossible to load image %s ! SDL_image Error: %s\n", filesNameViolet[i], IMG_GetError());
                                dicesPlayersImages[4][i] = NULL;
                            }
                            else
                            {
                                dicesPlayersImages[4][i] = SDL_CreateTextureFromSurface(renderer, imageSurface);
                                if (!dicesPlayersImages[4][i])
                                {
                                    printf("Impossible to create texture from image %s ! SDL Error: %s\n", filesNameViolet[i], SDL_GetError());
                                }
                                SDL_FreeSurface(imageSurface);
                            }
                        }
                    }
                }
            }
        }
    }
}




// Fonction d'intiialisation de la SDL
void initSDL(SDL_Window **window, SDL_Renderer **renderer) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
       
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