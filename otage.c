/*
otage.c
----------

Fait par : Mehdi

Rôle : Concerne les otages, les casernes et la base

Date de dernière modification : 05/04/2013

*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/otage.h"
#include "headers/erreur.h"
#include "headers/map.h"

#define NOMBRE_CASERNE 5
//Plus la vitesse est grande et plus l'otage se déplacera rapidement
#define VITESSE_OTAGE 2

//-------------------------PARTIE CASERNE---------------------------------
void iniCaserne(SDL_Surface *ecran,sprite *caserne,int** map,tilesets* tilesetsMap,int positionX)
{
    int i=0;

    //initialise les surface ( image 1 = état normal | image 2 = état détruit )
    for(i=0;i<=IMAGE2;i++)
    {
        caserne->image[i].image=NULL;
    }

    //chargement des images du caserne
    caserne->image[IMAGE1].image=IMG_Load(NOM_FICHIER_CASERNE);
    caserne->image[IMAGE2].image=IMG_Load(NOM_FICHIER_CASERNE2);

    //verification que l'image soit bien charger
    if(caserne->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_CASERNE);
    }
    if(caserne->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_CASERNE2);
    }

    //initialisation des position du caserne
    caserne->image[IMAGE1].position.x=positionX;
    caserne->image[IMAGE1].position.y=ecran->h-hauteur_sol_max(map,tilesetsMap,ecran,positionX,(positionX+caserne->image[IMAGE1].image->w/tilesetsMap->image[0]->w) )-caserne->image[IMAGE1].image->h;

    //vie du caserne
    caserne->vie=VIE_CASERNE;

    //on attribue les propriété de l'écran a la structure caserne
    caserne->imageUtilise.positionEcran=ecran;

    //on initialise le numero de l'image a blitté
    caserne->imageUtilise.numeroImage=IMAGE1;
}
//------------------------------------------------------------------------

//-------------------------PARTIE OTAGE---------------------------------
void iniOtage(SDL_Surface *ecran,otage *Otage,int** map,tilesets* tilesetsMap,int positionX)
{
    int i=0;
    //initialise les surface ( image 1 = état normal | image 2 = état détruit )
    for(i=0;i<=IMAGE5;i++)
    {
        Otage->strucSprite.image[i].image=NULL;
    }

    //chargement des images du tank
    Otage->strucSprite.image[IMAGE1].image=IMG_Load(NOM_FICHIER_OTAGE);
    Otage->strucSprite.image[IMAGE2].image=IMG_Load(NOM_FICHIER_OTAGE2);
    Otage->strucSprite.image[IMAGE3].image=IMG_Load(NOM_FICHIER_OTAGE3);
    Otage->strucSprite.image[IMAGE4].image=IMG_Load(NOM_FICHIER_OTAGE4);
    Otage->strucSprite.image[IMAGE5].image=IMG_Load(NOM_FICHIER_OTAGE5);

    //verification que l'image soit bien charger
    if(Otage->strucSprite.image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_OTAGE);
    }
    if(Otage->strucSprite.image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_OTAGE2);
    }
    if(Otage->strucSprite.image[IMAGE3].image==NULL)
    {
        erreur_image(NOM_FICHIER_OTAGE3);
    }
    if(Otage->strucSprite.image[IMAGE4].image==NULL)
    {
        erreur_image(NOM_FICHIER_OTAGE4);
    }
    if(Otage->strucSprite.image[IMAGE5].image==NULL)
    {
        erreur_image(NOM_FICHIER_OTAGE5);
    }

    //initialisation des position du tank
    Otage->strucSprite.image[IMAGE1].position.x=0;
    Otage->strucSprite.image[IMAGE1].position.y=0;

    Otage->strucSprite.image[IMAGE1].position.x=positionX;
    Otage->strucSprite.image[IMAGE1].position.y=ecran->h-hauteur_sol_max(map,tilesetsMap,ecran,positionX,(positionX+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->image[0]->w) )-Otage->strucSprite.image[IMAGE1].image->h;

    //vie du tank
    Otage->strucSprite.vie=VIE_OTAGE;

    //Nombre d'otage par caserne
    Otage->nbOtage=NB_OTAGE_PAR_CASERNE;

    //on attribue les propriété de l'écran a la structure caserne
    Otage->strucSprite.imageUtilise.positionEcran=ecran;

    //on initialise le numero de l'image a blitté
    Otage->strucSprite.imageUtilise.numeroImage=IMAGE5;

    //Par défaut, on active la file car lorsque la caserne sera détruite, un otage sortira directement, donc la file sera active
    Otage->file=1;
}

void deplacementOtageVersHelico(otage *Otage,sprite *helico,int** map,tilesets *tilesetsMap,int positionMap,int tempsActu,int *tempsOtage)
{
    //création de variable pour simplifier la lecture et réduire la longueur de la condition
    int moitierMap=0,positionOtageX=0;

    moitierMap=helico->imageUtilise.positionEcran->w/tilesetsMap->image[IMAGE1]->w/2;
    positionOtageX=Otage->strucSprite.image[IMAGE1].position.x;

    if( tempsActu>((*tempsOtage)+(200/VITESSE_OTAGE))  )
    {
        //On vérifie si l'hélico est attérie près de l'otage
        if( ( (moitierMap*-1)<(positionMap-positionOtageX) && (positionMap-positionOtageX)<moitierMap ) && (atterrissageHelico(helico,map,tilesetsMap,positionMap)==1) )
        {
            if( (positionMap>Otage->strucSprite.image[IMAGE1].position.x) )
            {
                Otage->strucSprite.image[IMAGE1].position.x++;
                Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-Otage->strucSprite.image[IMAGE1].image->h-
                                                            hauteur_sol_max(map,tilesetsMap,
                                                            Otage->strucSprite.imageUtilise.positionEcran,Otage->strucSprite.image[IMAGE1].position.x,Otage->strucSprite.image[IMAGE1].position.x+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->image[0]->w);
                Otage->strucSprite.imageUtilise.direction=DROITE;
            }
            else if( (positionMap<Otage->strucSprite.image[IMAGE1].position.x) )
            {
                Otage->strucSprite.image[IMAGE1].position.x--;
                Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-Otage->strucSprite.image[IMAGE1].image->h-
                                                            hauteur_sol_max(map,tilesetsMap,
                                                            Otage->strucSprite.imageUtilise.positionEcran,Otage->strucSprite.image[IMAGE1].position.x,Otage->strucSprite.image[IMAGE1].position.x+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->image[0]->w);
                Otage->strucSprite.imageUtilise.direction=GAUCHE;
            }
        }
        else
        {
            Otage->strucSprite.imageUtilise.direction=FACE;
        }

        *tempsOtage=tempsActu;
    }

}

void gestionFileOtage(otage *Otage,sprite *helico,int positionCaserneX,int positionCaserneY,int **map,tilesets *tilesetsMap,int positionActu,int *nbOtageBord)
{
    //On regarde si un otage est monter a bord
    if(Otage->file==1 && 1==hotage_monte_helico(helico,Otage,map,tilesetsMap,positionActu) )
    {
        Otage->nbOtage--;
        *nbOtageBord+=1;
        Otage->file=0;
    }
    //On regarde si l'otage a été tuer
    else if(Otage->file==1 && Otage->strucSprite.vie==0)
    {
        Otage->nbOtage--;
        Otage->file=0;
        Otage->strucSprite.vie=VIE_OTAGE;
    }
    else if(Otage->file==0)
    {
        Otage->strucSprite.image[IMAGE1].position.x=positionCaserneX;
        Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-
                                                    hauteur_sol_max(map,tilesetsMap,Otage->strucSprite.imageUtilise.positionEcran,positionCaserneX,(positionCaserneX+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->image[0]->w) )-Otage->strucSprite.image[IMAGE1].image->h;
        Otage->file=1;
    }
}

void deplacementOtageVersBase(otage *Otage,sprite *helico,sprite *base,int** map,tilesets *tilesetsMap,int positionMap,int tempsActu,int *tempsOtage)
{
    if(tempsActu>((*tempsOtage)+(200/VITESSE_OTAGE) ))
    {
        if( (base->image[IMAGE1].position.x>Otage->strucSprite.image[IMAGE1].position.x) )
        {
            Otage->strucSprite.image[IMAGE1].position.x++;
            Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-Otage->strucSprite.image[IMAGE1].image->h-
                                                        hauteur_sol_max(map,tilesetsMap,
                                                        Otage->strucSprite.imageUtilise.positionEcran,Otage->strucSprite.image[IMAGE1].position.x,Otage->strucSprite.image[IMAGE1].position.x+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->image[0]->w);
            Otage->strucSprite.imageUtilise.direction=DROITE;
        }
        else if( (base->image[IMAGE1].position.x<Otage->strucSprite.image[IMAGE1].position.x) )
        {
            Otage->strucSprite.image[IMAGE1].position.x--;
            Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-Otage->strucSprite.image[IMAGE1].image->h-
                                                        hauteur_sol_max(map,tilesetsMap,
                                                        Otage->strucSprite.imageUtilise.positionEcran,Otage->strucSprite.image[IMAGE1].position.x,Otage->strucSprite.image[IMAGE1].position.x+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->image[0]->w);
            Otage->strucSprite.imageUtilise.direction=GAUCHE;
        }
        else
        {
            Otage->strucSprite.imageUtilise.direction=FACE;
        }

        *tempsOtage=tempsActu;
    }

}
//------------------------------------------------------------------------

//-------------------------PARTIE BASE------------------------------------
void iniBase(SDL_Surface *ecran,sprite *base,int** map,tilesets* tilesetsMap)
{
    base->image[IMAGE1].image=NULL;

    //chargement des images du base
    base->image[IMAGE1].image=IMG_Load(NOM_FICHIER_BASE);

    //verification que l'image soit bien charger
    if(base->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_BASE);
    }

    //initialisation des position du base
    //On place la base vers le debus de la map sans possibilité a l'hélicoptère d'être coller ou que la base soit coller au bors de la fenetre
    int positionX=( (ecran->w-base->image[IMAGE1].image->w)/tilesetsMap->image[IMAGE1]->w)/2*-1;
    base->image[IMAGE1].position.x=positionX;
    base->image[IMAGE1].position.y=ecran->h-hauteur_sol_max(map,tilesetsMap,ecran,positionX,(positionX+base->image[IMAGE1].image->w/tilesetsMap->image[0]->w) )-base->image[IMAGE1].image->h;

    //on attribue les propriété de l'écran a la structure base
    base->imageUtilise.positionEcran=ecran;

    //on initialise le numero de l'image a blitté
    base->imageUtilise.numeroImage=IMAGE1;
}

void iniBariere(SDL_Surface *ecran,sprite *bariere,int** map,tilesets* tilesetsMap)
{
    bariere->image[IMAGE1].image=NULL;

    //chargement des images de la bariere
    bariere->image[IMAGE1].image=IMG_Load(NOM_FICHIER_BARIERE);

    //verification que l'image soit bien charger
    if(bariere->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_BARIERE);
    }

    //on attribue les propriété de l'écran a la structure bariere
    bariere->imageUtilise.positionEcran=ecran;

    //initialisation des position de la bariere
    int positionX=saveZone(bariere,tilesetsMap)-bariere->image[IMAGE1].image->w/tilesetsMap->image[IMAGE1]->w-1;
    bariere->image[IMAGE1].position.x=positionX;
    bariere->image[IMAGE1].position.y=ecran->h-hauteur_sol_max(map,tilesetsMap,ecran,positionX,saveZone(bariere,tilesetsMap)-1  )-bariere->image[IMAGE1].image->h;

    //on initialise le numero de l'image a blitté
    bariere->imageUtilise.numeroImage=IMAGE1;
}

int colisionBariere()
{
    //code ici
}
//------------------------------------------------------------------------
