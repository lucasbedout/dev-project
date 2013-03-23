/*
jeu.c
----------

Fait par : Mehdi

Rôle : Permet de lancer le jeu et les fonction principal comme l'hélicoptère, les énnemies, la map, etc...

Date de dernière modification : 03/03/2013

*/
#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/jeu.h"
#include "headers/helicoptere.h"
#include "headers/erreur.h"
#include "headers/map.h"

void jeu (SDL_Surface *ecran)
{
    //création d'un evennement
    SDL_Event even;

    //--------------------CHARGEMENT DE LA MAP---------------------
    //map 2D contenant tout simplement des chiffres
    int **map=NULL;
    //positionMap permet de déplacé la map pour faire un scrolling
    int positionMap=0;
    tilesets tilesetsMap;
    declaration_tilesets(&tilesetsMap);
    map=chargement_map(ecran);
    //--------------------------------------------------------------

    //---------VARIABLE HELICO---------------
    //Initilisation position de l'hélico et chargement sprite
    sprite helico;
    iniPosHelicoptere(ecran,&helico);
    //Type AnimeHelico permet de savoir quel image est actuellement blitter
    int typeAnimeHelico=IMAGE1,actionEnCour=0;
    helico.imageUtilise.positionEcran=ecran;
    //on commence par l'image 1 et avec la position de l'hélico vers la gauche
    helico.imageUtilise.numeroImage=IMAGE1;
    helico.imageUtilise.direction=GAUCHE;
    //---------TIR DE L HELICO-------
    double coefA=0.0,coefB=0.0;
    int equation=0;
    //-------------------------------
    //---------------------------------------

    //---------VARIABLE SOUCOUPE VOLANTE--------
    //------------------------------------------

    //---------VARIABLE TANK--------------------
    //------------------------------------------

    //---------VARIABLE AVION-------------------
    //------------------------------------------

    //---------VARIABLE OTAGE-------------------
    //------------------------------------------

    //----------------DECLARATION TEMPS JEU-----------------------
    temps tempsJeu;
    initilisationTemps(tempsJeu.tempsPrecedent);
    //------------------------------------------------------------

    //---------------Blitage---------------------
    SDL_BlitSurface(helico.image1.image,NULL,ecran,&helico.image1.position);
    //--------------------------------------------

    SDL_Flip(ecran);

    //activation des touches répétter
    SDL_EnableKeyRepeat(10,10);

    //-------------VARIABLE DU JEU GLOBAL-----------------------------
    int continuer=verif_erreur();

    //créationdes position ( pour le tir de l'hélico )
    SDL_Rect cible,position;

    tempsJeu.tempsActuel=SDL_GetTicks(); //temps actuel du jeu
    //----------------------------------------------------------------

    while(continuer)
    {
        option(&continuer,&even);

        deplacementHelico(&helico,&even,&positionMap,&tilesetsMap,map);

        //On blitte toute les surfaces et on rafraichie l'image
        affiche_map(map,ecran,tilesetsMap,positionMap);
        //decallement_image_map(&helico,&tilesetsMap,20,40,positionMap,helico.image2.image);
        //blitte le tir que si le tir est en cour
        tirHelico(&even,&actionEnCour,&cible,&position,&helico,ecran,&coefA,&coefB,&equation);
        if(actionEnCour==1)
        {SDL_BlitSurface(helico.imageUtilise.tir,NULL,ecran,&position);}
        //Gestion des colisions
        GestionColision(&helico,map,&tilesetsMap,positionMap);
        //On blitte les animations de l'hélico en fonction si l'hélico est atérie ou pas
        if(0==atterrissageHelico(&helico,map,&tilesetsMap,positionMap))
        {
            typeAnimeHelico=animationHelico(typeAnimeHelico,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[1],ecran,&helico);
        }
        else
        {
            SDL_BlitSurface(helico.image1.image,NULL,ecran,&helico.image1.position);
        }

        SDL_Flip(ecran);

        //Mise en place des FPS
        tempsJeu.tempsActuel=SDL_GetTicks();
        if((tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0])<(TEMPS_POSE))
        {
            SDL_Delay(TEMPS_POSE-(tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0]));
        }
    }

    //on libere les images chargeren mémoire
    SDL_FreeSurface(helico.image1.image);
    SDL_FreeSurface(helico.image2.image);
    SDL_FreeSurface(helico.image3.image);
    SDL_FreeSurface(helico.image4.image);
    SDL_FreeSurface(helico.imageUtilise.tir);
    liberation_tilesets(&tilesetsMap);

}

void option(int* continuer,SDL_Event *even)
{
    SDL_PollEvent(even);

    switch(even->type)
    {
        case SDL_QUIT:
            //*continuer=0;
            break;

        case SDL_KEYDOWN:
                switch(even->key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        *continuer=0;
                        break;
                }
                break;
    }

}

void initilisationTemps(int tab[TAILLE_TABLEAU_TEMPS])
{
    int i=0;
    for(i=0;i<TAILLE_TABLEAU_TEMPS;i++)
    {
        tab[i]=0;
    }
}
