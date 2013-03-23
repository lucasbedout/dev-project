/*
main.c
initialise les composants du jeu (fenêtres, variables etc...)

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "erreur.h"
#include "constantes.h"

int main ( int argc, char** argv )
{
	int continuer = 1;
    SDL_Surface* ecran;
    // initialisation de la SDL VIDEO
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
    {
        printf("Impossible d'initialiser la librairie vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // création de l'écran
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!ecran)
    {
        printf("Impossible de creer une fenetre 800x600 video: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //On nome la fenetre
    SDL_WM_SetCaption("Choplifter",NULL);

    //Blittage et chargement d'un image PNG pour l'écran de menu
    SDL_Surface *Menu=IMG_Load(NOM_FICHIER_MENU);
    if (Menu==NULL)
    {
        erreur_image(NOM_FICHIER_MENU);
    }

    SDL_Rect positionMenu;
    positionMenu.x=0;
    positionMenu.y=0;

    SDL_BlitSurface(Menu,NULL,ecran,&positionMenu);

    //Boucle pour ne pas fermer la fenetre
    while(continuer)
    {

        SDL_Event even;

	    SDL_WaitEvent(&even);

	    switch(even.type)
	    {
	        case SDL_QUIT:
	            continuer=0;
	            break;

	        case SDL_KEYDOWN:
	                switch(even.key.keysym.sym)
	                {
	                    case SDLK_ESCAPE:
	                        continuer=0;
	                        break;
	                    case SDLK_j:
	                        jeu(ecran);
	                        break;
	                    case SDLK_e:
	                        editeur(ecran);
	                        break;
	                }
	                break;
	    }

        SDL_BlitSurface(Menu,NULL,ecran,&positionMenu);

        SDL_Flip(ecran);
    }

    SDL_FreeSurface(Menu);
    SDL_Quit();

}
