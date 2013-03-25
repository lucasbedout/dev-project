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

#include "headers/constantes.h"
#include "headers/ennemie.h"
#include "headers/erreur.h"
#include "headers/map.h"


//-----------------------------------------------PARTIE TANK---------------------------------------------------
void iniTank(SDL_Surface *ecran,sprite *tank)
{
    //initialise les surface
    tank->image4.image=NULL;
    tank->image3.image=NULL;
    tank->image2.image=NULL;
    tank->image1.image=NULL;

    //chargement des images du tank
    tank->image1.image=IMG_Load(NOM_FICHIER_TANK);
    tank->image2.image=IMG_Load(NOM_FICHIER_TANK2);
    tank->image3.image=IMG_Load(NOM_FICHIER_TANK3);
    tank->image4.image=IMG_Load(NOM_FICHIER_TANK4);

    //verification que l'image soit bien charger
    if(tank->image1.image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK);
    }
    if(tank->image2.image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK2);
    }
    if(tank->image3.image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK3);
    }
    if(tank->image4.image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK4);
    }

    //initialisation des position du tank
    tank->image1.position.x=0;
    tank->image1.position.y=0;

    //chargement de l'image du tir associer au tank
    tank->imageUtilise.tir=NULL;
    /*tank->imageUtilise.tir=IMG_Load(NOM_FICHIER_TIR_HELICO);

    //verification que l'image soit bien charger
    if(tank->imageUtilise.tir==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_HELICO);
    }*/
}

void deplacementTank(sprite *tank,int positionMap,tilesets *tilesetsMap,int** map)
{
    //tank->image1.position.y=((tank->imageUtilise.positionEcran->h-hauteur_sol(map,tilesetsMap,tank->imageUtilise.positionEcran,positionMap)-tank->image1.image->h)/tilesetsMap->image[0]->h);
    tank->image1.position.y=tank->imageUtilise.positionEcran->h-hauteur_sol(map,tilesetsMap,tank->imageUtilise.positionEcran,positionMap)-tank->image1.image->h;
    tank->image1.position.x=positionMap;
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

//-------------------------------------------------------------------------------------------------------------
