#include <SDL2/SDL.h>
#include<stdio.h>
#define N 10

int main(int argc,char **argv)
{
    (void)argc;
    (void)argv;

    //initialisation de la SDL + gestion d'echec
    if (SDL_Init(SDL_INIT_VIDEO)!= 0)
    {
        SDL_Log("Error: SDL_initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }  


    int n;     
    SDL_Window * window [N]; 
    for (n=0;n<N;n++) 
    {  
      window[n]= SDL_CreateWindow("fenetre_mobile",0,0,20*n,20*n,SDL_WINDOW_RESIZABLE);  }
    
     //gestion d'echec 
      if (window[n]== NULL) 
      {
        SDL_Log("Error : SDL window  creation - %s\n", 
                 SDL_GetError());                 // échec de la création de la fenêtre
        SDL_Quit();                              // On referme la SDL       
        exit(EXIT_FAILURE);
    
         }
    int a;
    for (a=0;a<N;a++) { 
        int w,h;

        SDL_GetWindowSize(window[a], &w, &h);

        SDL_DisplayMode D;
        SDL_GetCurrentDisplayMode(0, &D);
        

        int Ecran_h=D.h;
        for (int i=0; i<(Ecran_h-h)/10; i++)
        {
    
          SDL_SetWindowPosition(window[a], 10*i,10*i);
        } 
    
        int i=1;
        int x,y;
        SDL_GetWindowPosition(window[a],&x,&y);

        for (int j=1; j<30; j++)
        { 
          x +=5*j;
          y -=5*i;
          SDL_SetWindowPosition(window[a],x,y);
          i+=1;
        
        } }
      
    int b=0;
    SDL_Delay(2000);
    for (b=0;b<N;n++)
    { 
      SDL_DestroyWindow(window[N-1-b]);
    }
    SDL_Quit();  
    exit(EXIT_FAILURE);
    return 0;
 
} 