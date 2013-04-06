/*
ennemie.c
----------

Fait par : Mehdi

Rôle : Concerne les ennemies ( tank,avion, soucoupe volante )

Date de dernière modification : 05/04/2013

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

//--------------------------------DEFINE TANK-----------------------------
//Plus la vitesse est grande et plus le tank se déplacera rapidement
#define VITESSE_TANK 1
//Plus la latence est petite et plus le tir est relancé rapidement ( 1 latence = 1 seconde par temps de réponse )
#define LATENCE_TIR_TANK 1
//Plus la porter est grande, plus le tank poura tirré loin
#define PORTER_TIR_TANK 400
//------------------------------------------------------------------------

//-----------------------------------------------PARTIE TANK---------------------------------------------------
void iniTank(SDL_Surface *ecran,sprite *tank)
{
    int i=0;

    //initialise les surface
    for(i=0;i<=IMAGE4;i++)
    {
         tank->image[i].image=NULL;
    }

    //chargement des images du tank
    tank->image[IMAGE1].image=IMG_Load(NOM_FICHIER_TANK);
    tank->image[IMAGE2].image=IMG_Load(NOM_FICHIER_TANK2);
    tank->image[IMAGE3].image=IMG_Load(NOM_FICHIER_TANK3);
    tank->image[IMAGE4].image=IMG_Load(NOM_FICHIER_TANK4);

    //verification que l'image soit bien charger
    if(tank->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK);
    }
    if(tank->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK2);
    }
    if(tank->image[IMAGE3].image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK3);
    }
    if(tank->image[IMAGE4].image==NULL)
    {
        erreur_image(NOM_FICHIER_TANK4);
    }

    //initialisation des position du tank
    tank->image[IMAGE1].position.x=0;
    tank->image[IMAGE1].position.y=0;

    //chargement de l'image du tir associer au tank
    tank->imageUtilise.tir.image=NULL;
    tank->imageUtilise.tir.image=IMG_Load(NOM_FICHIER_TIR_TANK);

    //verification que l'image soit bien charger
    if(tank->imageUtilise.tir.image==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_TANK);
    }

    //vie du tank
    tank->vie=VIE_TANK;

    //on attribue les propriété de l'écran a la structure tank
    tank->imageUtilise.positionEcran=ecran;

    //on initialise la position de l'image et le numero de l'image a blitté
    tank->imageUtilise.numeroImage=IMAGE1;
    tank->imageUtilise.direction=GAUCHE;

    //---------TIR DU TANK------------
    tank->imageUtilise.tir.coefDirecteur=0.0;
    tank->imageUtilise.tir.coefIndice=0.0;
    tank->imageUtilise.tir.signeEquation=0;
    tank->imageUtilise.tir.actionEnCour=0;
    //-------------------------------
}

void deplacementTank(sprite *tank,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsTank)
{
    int limiteDeplacement=0;
    limiteDeplacement=tank->imageUtilise.positionEcran->w/tilesetsMap->image[IMAGE1]->w;

    if(tempsActu>((*tempsTank)+(200/VITESSE_TANK) ) &&
       (positionMap-tank->image[IMAGE1].position.x<limiteDeplacement) && (positionMap-tank->image[IMAGE1].position.x>(limiteDeplacement*-1) )
       && (tank->image[IMAGE1].position.x>saveZone(tank,tilesetsMap) ) )
    {
        //permet de positionner le tank par rapport au sol
        tank->image[IMAGE1].position.y=tank->imageUtilise.positionEcran->h-hauteur_sol_max(map,tilesetsMap,tank->imageUtilise.positionEcran,tank->image[IMAGE1].position.x,
                                                                                           (tank->image[IMAGE1].position.x+tank->image[IMAGE1].image->w/tilesetsMap->image[0]->w))-tank->image[IMAGE1].image->h;

        //permet au tank de suivre l'hélico ( tout en étant centrer ) avec un temps de latence a chaque déplacement
        if(positionMap>((tank->image[IMAGE1].position.x)+((tank->image[IMAGE1].image->w/2)/tilesetsMap->image[0]->w)) )
        {
            tank->image[IMAGE1].position.x++;
            tank->imageUtilise.direction=DROITE;
        }
        else if(positionMap<((tank->image[IMAGE1].position.x)+((tank->image[IMAGE1].image->w/2)/tilesetsMap->image[0]->w)) )
        {
            tank->image[IMAGE1].position.x--;
            tank->imageUtilise.direction=GAUCHE;
        }

        *tempsTank=tempsActu;
    }
}

void calculTrajectoireTank(sprite *tank,sprite *helico,int positionMap,tilesets *tilesetsMap,int tempsActu,int *tempsPrece)
{
    //On regarde si l'hélico est a la porté du tir du tank et si le tank a la possibilité de retirré
    if((tempsActu>(*tempsPrece+1000*LATENCE_TIR_TANK)) &&
       ( ((tank->image[IMAGE1].position.y-helico->image[IMAGE1].position.y)<PORTER_TIR_TANK) &&
        ( ((positionMap-tank->image[IMAGE1].position.x)<( PORTER_TIR_TANK/tilesetsMap->image[IMAGE1]->w )) &&
         ((positionMap-tank->image[IMAGE1].position.x)>( -1*PORTER_TIR_TANK/tilesetsMap->image[IMAGE1]->w ))  ) )
       )
    {
        //on prends la position de la cible en prenant la colonne ( unité de valeur : tileset ) et les y en pixel
        tank->imageUtilise.tir.cibleTir.x=positionMap;
        tank->imageUtilise.tir.cibleTir.y=helico->image[IMAGE1].position.y+helico->image[IMAGE1].image->h/2;
        //la position se fait a partir de la position de la map et on place le tir juste en dessosu de l'hélico
        tank->imageUtilise.tir.positionTir.x=tank->image[IMAGE1].position.x;
        tank->imageUtilise.tir.positionTir.y=tank->image[IMAGE1].position.y;
        //on déclare enfin que le tir est en cour
        tank->imageUtilise.tir.actionEnCour=1;

        *tempsPrece=tempsActu;
    }
}

//-------------------------------------------------------------------------------------------------------------

//-----------------------------------------------PARTIE AVION--------------------------------------------------
void iniAvion(SDL_Surface *ecran,sprite *avion)
{
    int i=0;

    //initialise les surface
    for(i=0;i<=IMAGE4;i++)
    {
        avion->image[i].image=NULL;
    }

    //chargement des images de l'avion
    avion->image[IMAGE1].image=IMG_Load(NOM_FICHIER_HELICO);
    avion->image[IMAGE2].image=IMG_Load(NOM_FICHIER_HELICO2);
    avion->image[IMAGE3].image=IMG_Load(NOM_FICHIER_HELICO3);
    avion->image[IMAGE4].image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(avion->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(avion->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(avion->image[IMAGE3].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(avion->image[IMAGE4].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //initialisation des position de l'avion
    avion->image[IMAGE1].position.x=0;
    avion->image[IMAGE1].position.y=0;

    //chargement de l'image du tir associer a l'avion
    avion->imageUtilise.tir.image=NULL;
    avion->imageUtilise.tir.image=IMG_Load(NOM_FICHIER_TIR_HELICO);

    //verification que l'image soit bien charger
    if(avion->imageUtilise.tir.image==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_HELICO);
    }

    //vie de l'avion
    avion->vie=VIE_AVION;

    //on attribue les propriété de l'écran a la structure avion
    avion->imageUtilise.positionEcran=ecran;

    //on initialise la position de l'image et le numero de l'image a blitté
    avion->imageUtilise.numeroImage=IMAGE1;
    avion->imageUtilise.direction=GAUCHE;

    //---------TIR DU AVION-----------
    avion->imageUtilise.tir.coefDirecteur=0.0;
    avion->imageUtilise.tir.coefIndice=0.0;
    avion->imageUtilise.tir.signeEquation=0;
    avion->imageUtilise.tir.actionEnCour=0;
    //-------------------------------
}
//-------------------------------------------------------------------------------------------------------------

//-----------------------------------------------PARTIE SOUCOUPE-----------------------------------------------
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe)
{
    int i=0;

    //initialise les surface
    for(i=0;i<=IMAGE4;i++)
    {
        soucoupe->image[i].image=NULL;
    }

    //chargement des images de la soucoupe volante
    soucoupe->image[IMAGE1].image=IMG_Load(NOM_FICHIER_HELICO);
    soucoupe->image[IMAGE2].image=IMG_Load(NOM_FICHIER_HELICO2);
    soucoupe->image[IMAGE3].image=IMG_Load(NOM_FICHIER_HELICO3);
    soucoupe->image[IMAGE4].image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(soucoupe->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(soucoupe->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(soucoupe->image[IMAGE3].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(soucoupe->image[IMAGE4].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //initialisation de la position de la soucoupe volante
    soucoupe->image[IMAGE1].position.x=0;
    soucoupe->image[IMAGE1].position.y=0;

    //vie de la soucoupe
    soucoupe->vie=VIE_SOUCOUPE;

    //on attribue les propriété de l'écran a la structure soucoupe
    soucoupe->imageUtilise.positionEcran=ecran;

    //on initialise la position de l'image et le numero de l'image a blitté
    soucoupe->imageUtilise.numeroImage=IMAGE1;
    soucoupe->imageUtilise.direction=GAUCHE;

    //---------TIR DU SOUCOUPE-----------
    soucoupe->imageUtilise.tir.coefDirecteur=0.0;
    soucoupe->imageUtilise.tir.coefIndice=0.0;
    soucoupe->imageUtilise.tir.signeEquation=0;
    soucoupe->imageUtilise.tir.actionEnCour=0;
    //-------------------------------

}
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------PARTIE GENERAL----------------------------------------------

//-------------------------------------------------------------------------------------------------------------
