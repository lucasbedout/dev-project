/*
ennemie.c
----------

Fait par : Mehdi

Rôle : Concerne les ennemies ( tank,avion, soucoupe volante )

Date de dernière modification : 12/03/2013

*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "constantes.h"
#include "ennemie.h"
#include "erreur.h"
#include "map.h"


//-----------------------------------------------PARTIE TANK---------------------------------------------------
void iniTank(SDL_Surface *ecran,sprite *tank)
{
    //initialise les surface
    tank->image4.image=NULL;
    tank->image3.image=NULL;
    tank->image2.image=NULL;
    tank->image1.image=NULL;

    //chargement des images du tank
    tank->image1.image=IMG_Load(NOM_FICHIER_HELICO);
    tank->image2.image=IMG_Load(NOM_FICHIER_HELICO2);
    tank->image3.image=IMG_Load(NOM_FICHIER_HELICO3);
    tank->image4.image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(tank->image1.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(tank->image2.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(tank->image3.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(tank->image4.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //initialisation des position du tank
    tank->image1.position.x=0;
    tank->image1.position.y=0;

    //chargement de l'image du tir associer au tank
    tank->imageUtilise.tir=NULL;
    tank->imageUtilise.tir=IMG_Load(NOM_FICHIER_TIR_HELICO);

    //verification que l'image soit bien charger
    if(tank->imageUtilise.tir==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_HELICO);
    }
}

void deplacementTank(sprite *helico,SDL_Event* even,int *positionMap,tilesets *tilesetsMap,int** map)
{
    //Code ici
}
//-------------------------------------------------------------------------------------------------------------

//-----------------------------------------------PARTIE AVION--------------------------------------------------
void iniAvion(SDL_Surface *ecran,sprite *avion)
{
    //initialise les surface
    avion->image4.image=NULL;
    avion->image3.image=NULL;
    avion->image2.image=NULL;
    avion->image1.image=NULL;

    //chargement des images de l'avion
    avion->image1.image=IMG_Load(NOM_FICHIER_HELICO);
    avion->image2.image=IMG_Load(NOM_FICHIER_HELICO2);
    avion->image3.image=IMG_Load(NOM_FICHIER_HELICO3);
    avion->image4.image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(avion->image1.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(avion->image2.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(avion->image3.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(avion->image4.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //initialisation des position de l'avion
    avion->image1.position.x=0;
    avion->image1.position.y=0;

    //chargement de l'image du tir associer a l'avion
    avion->imageUtilise.tir=NULL;
    avion->imageUtilise.tir=IMG_Load(NOM_FICHIER_TIR_HELICO);

    //verification que l'image soit bien charger
    if(avion->imageUtilise.tir==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_HELICO);
    }
}
//-------------------------------------------------------------------------------------------------------------

//-----------------------------------------------PARTIE SOUCOUPE-----------------------------------------------
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe)
{
    //initialise les surface
    soucoupe->image4.image=NULL;
    soucoupe->image3.image=NULL;
    soucoupe->image2.image=NULL;
    soucoupe->image1.image=NULL;

    //chargement des images de la soucoupe volante
    soucoupe->image1.image=IMG_Load(NOM_FICHIER_HELICO);
    soucoupe->image2.image=IMG_Load(NOM_FICHIER_HELICO2);
    soucoupe->image3.image=IMG_Load(NOM_FICHIER_HELICO3);
    soucoupe->image4.image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(soucoupe->image1.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(soucoupe->image2.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(soucoupe->image3.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(soucoupe->image4.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //initialisation de la position de la soucoupe volante
    soucoupe->image1.position.x=0;
    soucoupe->image1.position.y=0;

}
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------PARTIE GENERAL----------------------------------------------
int animationEnnemie(int image,int tempsActu,int tempsPrece,SDL_Surface *ecran,sprite *ennemie)
{
    if(ennemie->imageUtilise.direction==GAUCHE)
    {
        //si l'image 1 est blitter et que le temps est supérieur a 50ms on blitte l'image 2
        if( ( (image==IMAGE1) && ((tempsActu-tempsPrece)>50) ) || image==IMAGE3 || image==IMAGE4 )
        {
            SDL_BlitSurface(ennemie->image2.image,NULL,ecran,&(ennemie->image1.position));

            return IMAGE2;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE2) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(ennemie->image1.image,NULL,ecran,&(ennemie->image1.position));

            return IMAGE1;
        }
    }
    else if(ennemie->imageUtilise.direction==DROITE)
    {
        //si l'image 3 est blitter et que le temps est supérieur a 50ms on blitte l'image 4
        if( ( (image==IMAGE3) && ((tempsActu-tempsPrece)>50) ) || image==IMAGE1 || image==IMAGE2 )
        {
            SDL_BlitSurface(ennemie->image4.image,NULL,ecran,&(ennemie->image1.position));

            return IMAGE4;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE4) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(ennemie->image3.image,NULL,ecran,&(ennemie->image1.position));

            return IMAGE3;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
