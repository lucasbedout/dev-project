/*
main.c
initialise les composants du jeu (fen�tres, variables etc...)

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "headers/erreur.h"
#include "headers/constantes.h"

int main (int argc, char** argv)
{
    supr_fichier_erreur();

	int continuer = 1;
    SDL_Surface *ecran, *Menu;
    SDL_Rect positionMenu;

    // initialisation de la SDL VIDEO
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        printf("Impossible d'initialiser la librairie vid�o : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // cr�ation de l'�cran
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!ecran)
    {
        printf("Erreur lors de la cr�ation de la fen�tre: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Choplifter",NULL);

    //affichage du menu
    Menu = IMG_Load(NOM_FICHIER_MENU);

    positionMenu.x = 0;
    positionMenu.y = 0;

    //on v�rifie si une erreur c'est produite
    continuer=verif_erreur();

    //choix du menu
    while(continuer)
    {
        //On v�rifie si une erreur c'est produit dans le jeu, dans l'�diteur ou dans le menu
        continuer=verif_erreur();

        SDL_Event event;

	    SDL_WaitEvent(&event);

	    switch(event.type)
	    {
	        case SDL_QUIT:
	            continuer = 0;
	       break;

	        case SDL_KEYDOWN:
	                switch(event.key.keysym.sym)
	                {
	                    case SDLK_ESCAPE:
	                    case SDLK_3:
	                    case SDLK_KP3:
	                        continuer=0;
	                    break;

	                    case SDLK_j:
	                    case SDLK_1:
	                    case SDLK_KP1:
	                        jeu(ecran);
	                    break;

	                    case SDLK_e:
                        case SDLK_2:
	                    case SDLK_KP2:
	                        editeur(ecran);
	                   break;
	                }
	        break;
	    }

        SDL_BlitSurface(Menu, NULL, ecran, &positionMenu);

        SDL_Flip(ecran);

    }

    SDL_FreeSurface(Menu);
    SDL_Quit();

}
