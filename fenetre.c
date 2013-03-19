/*
Fenetre.c
----------

Fait par : Mehdi

Rôle : création de la fenetre du menu

Date de dernière modifiction : 16/01/2013

*/

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "constantes.h"
#include "fenetre.h"
#include "jeu.h"
#include "editeur.h"
#include "erreur.h"

#define LARGEUR 800
#define HAUTEUR 600

void fenetreJeu()
{
    // initialisation de la SDL VIDEO
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
    {
        printf( "impossible d initialise la SDL video SDL: %s\n", SDL_GetError() );
        exit(EXIT_FAILURE);
    }

    // création de l'écran
    SDL_Surface* ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !ecran )
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

    int continuer=verif_erreur();

    //Boucle pour ne pas fermer la fenetre
    while(continuer)
    {
        //vérifie si une erreur a été générrer pendant le jeu
        continuer=verif_erreur();

        sortir(&continuer,ecran);

        SDL_BlitSurface(Menu,NULL,ecran,&positionMenu);

        SDL_Flip(ecran);
    }

    SDL_FreeSurface(Menu);
    SDL_Quit();

}

void sortir(int* continuer,SDL_Surface *ecran)
{
    SDL_Event even;

    SDL_WaitEvent(&even);

    switch(even.type)
    {
        case SDL_QUIT:
            *continuer=0;
            break;

        case SDL_KEYDOWN:
                switch(even.key.keysym.sym)
                {
                    case SDLK_3:
                    case SDLK_KP3:
                    case SDLK_ESCAPE:
                        *continuer=0;
                        break;
                    case SDLK_KP1:
                    case SDLK_1:
                        jeu(ecran);
                        break;
                    case SDLK_KP2:
                    case SDLK_2:
                        editeur(ecran);
                        break;
                }
                break;
    }

}
