/*
helicoptere.c
----------

Fait par : Mehdi

Rôle : Mouvement, tir, atterrissage,etc... Tout ce qui concerne l'hélicoptère en lui même est dans ce fichier

Date de dernière modification : 26/03/2013

*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>
#include <math.h>

#include "headers/constantes.h"
#include "headers/helicoptere.h"
#include "headers/erreur.h"
#include "headers/map.h"

#define VITESSE_HELICO 3

void iniPosHelicoptere(SDL_Surface *ecran,sprite *helico)
{
    int i=0;

    //initialise les surface
    for(i=0;i<=IMAGE4;i++)
    {
        helico->image[i].image=NULL;
    }

    //chargement de l'hélicoptère au centre de l'écran
    helico->image[IMAGE1].image=IMG_Load(NOM_FICHIER_HELICO);
    helico->image[IMAGE2].image=IMG_Load(NOM_FICHIER_HELICO2);
    helico->image[IMAGE3].image=IMG_Load(NOM_FICHIER_HELICO3);
    helico->image[IMAGE4].image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(helico->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(helico->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(helico->image[IMAGE3].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(helico->image[IMAGE4].image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //position de l'hélico
    helico->image[IMAGE1].position.x=0;
    helico->image[IMAGE1].position.y=0;
    helico->image[IMAGE1].position.x=((*ecran).w/2)-helico->image[IMAGE1].image->w/2;
    helico->image[IMAGE1].position.y=((*ecran).h/2)-helico->image[IMAGE1].image->h/2;

    //chargement de l'image du tir associer a l'hélicoptère
    helico->imageUtilise.tir.image[IMAGE1]=NULL;
    helico->imageUtilise.tir.image[IMAGE1]=IMG_Load(NOM_FICHIER_TIR_HELICO);

    //verification que l'image soit bien charger
    if(helico->imageUtilise.tir.image[IMAGE1]==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_HELICO);
    }

    //vie de l'hélicoptère
    helico->vie=VIE_HELICO;

    //on donne les valeurs de l'écran dans la structure de l'hélico
    helico->imageUtilise.positionEcran=ecran;

    //on initialise la position de l'image et le numero de l'image a blitté
    helico->imageUtilise.numeroImage=IMAGE1;
    helico->imageUtilise.direction=GAUCHE;

    //---------TIR DE L HELICO-------
    helico->imageUtilise.tir.coefDirecteur=0.0;
    helico->imageUtilise.tir.coefIndice=0.0;
    helico->imageUtilise.tir.signeEquation=0;
    helico->imageUtilise.tir.actionEnCour=0;
    //-------------------------------
}


void deplacementHelico(sprite *helico,SDL_Event* even,int *positionMap,tilesets *tilesetsMap,int** map)
{
    switch(even->type)
    {

        case SDL_KEYDOWN:
                switch(even->key.keysym.sym)
                {
                    case SDLK_UP:
                    case SDLK_w:
                        helico->image[IMAGE1].position.y-=VITESSE_HELICO;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        helico->image[IMAGE1].position.y+=VITESSE_HELICO;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        //on test si il est pas au bous de la map ou attérie
                        if( (*positionMap>0) && (atterrissageHelico(helico,map,tilesetsMap,*positionMap)==0 ) )
                        {
                            *positionMap=*positionMap-1;
                            helico->imageUtilise.direction=GAUCHE;
                        }
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        //On test si il n'est pas a la limite du scrolling map ou attérie
                        if( (*positionMap<limite_map(helico,tilesetsMap)) && (atterrissageHelico(helico,map,tilesetsMap,*positionMap)==0 ) )
                        {
                            *positionMap=*positionMap+1;
                            helico->imageUtilise.direction=DROITE;
                        }
                        break;
                }
                break;
    }


}

int animationHelico(int image,int tempsActu,int tempsPrece,SDL_Surface *ecran,sprite *helico)
{
    if(helico->imageUtilise.direction==GAUCHE)
    {
        //si l'image 1 est blitter et que le temps est supérieur a 50ms on blitte l'image 2
        if( ( (image==IMAGE1) && ((tempsActu-tempsPrece)>50) ) || image==IMAGE3 || image==IMAGE4 )
        {
            SDL_BlitSurface(helico->image[IMAGE2].image,NULL,ecran,&(helico->image[IMAGE1].position));

            return IMAGE2;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE2) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(helico->image[IMAGE1].image,NULL,ecran,&(helico->image[IMAGE1].position));

            return IMAGE1;
        }
    }
    else if(helico->imageUtilise.direction==DROITE)
    {
        //si l'image 3 est blitter et que le temps est supérieur a 50ms on blitte l'image 4
        if( ( (image==IMAGE3) && ((tempsActu-tempsPrece)>50) ) || image==IMAGE1 || image==IMAGE2 )
        {
            SDL_BlitSurface(helico->image[IMAGE4].image,NULL,ecran,&(helico->image[IMAGE1].position));

            return IMAGE4;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE4) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(helico->image[IMAGE3].image,NULL,ecran,&(helico->image[IMAGE1].position));

            return IMAGE3;
        }
    }
}

void calculTrajectoireTirHelico(SDL_Event* even,sprite *helico,int positionMap,tilesets *tilesetsMap)
{
    switch(even->type)
    {

        case SDL_MOUSEBUTTONDOWN:
                switch(even->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        //on prends la position de la cible en prenant la colonne ( unité de valeur : tileset ) et les y en pixel
                        helico->imageUtilise.tir.cibleTir.x=((even->button.x)/tilesetsMap->infoImage[0].image->w)+positionMap-((helico->imageUtilise.positionEcran->w/tilesetsMap->infoImage[0].image->w)/2);
                        helico->imageUtilise.tir.cibleTir.y=even->button.y;
                        //la position se fait a partir de la position de la map et on place le tir juste en dessosu de l'hélico
                        helico->imageUtilise.tir.positionTir.x=positionMap;
                        helico->imageUtilise.tir.positionTir.y=(helico->image[IMAGE1].position.y+helico->image[IMAGE1].position.h);
                        //on déclare enfin que le tir est en cour
                        helico->imageUtilise.tir.actionEnCour=1;
                        break;
                }
                break;
    }
}

//gestion des colisions
void GestionColision(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu)
{
    //Taille de l'helico par rapport a la taille des tilesets, le résultat est
    int tailleHelico=helico->image[IMAGE1].image->w/tilesetsMap->infoImage[IMAGE1].image->w;

    //On regarde si l'hélico ne sort pas du cadre de la fenetre
    if((helico->image[IMAGE1].position.x+helico->image[IMAGE1].position.w)>(helico->imageUtilise.positionEcran->w))
    {
        helico->image[IMAGE1].position.x--;
    }
    //Si l'hélico ne va pas plus bas que le sol ( le -1 est la pour arrondir le résultat )
    if(((helico->image[IMAGE1].position.y+helico->image[IMAGE1].position.h)>(helico->imageUtilise.positionEcran->h-hauteur_sol_max(map,tilesetsMap,helico->imageUtilise.positionEcran,positionActu-1-tailleHelico/2,positionActu+tailleHelico/2))) )
    {
        helico->image[IMAGE1].position.y-=VITESSE_HELICO;
    }
}

int atterrissageHelico(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu)
{
    //Taille de l'helico par rapport a la taille des tilesets, le résultat est
    int tailleHelico=helico->image[IMAGE1].image->w/tilesetsMap->infoImage[IMAGE1].image->w;

    //atterie (le -1 est la pour arrondir le résultat )
    if( (helico->image[IMAGE1].position.y+helico->image[IMAGE1].image->h) >=
       (helico->imageUtilise.positionEcran->h-hauteur_sol_max(map,tilesetsMap,helico->imageUtilise.positionEcran,positionActu-1-tailleHelico/2,positionActu+tailleHelico/2)-VITESSE_HELICO) )
    {
        return 1;
    }
    //vol
    else
    {
        return 0;
    }
}

void Gestion_Vie_helico(int *vie,sprite *helico,sprite *tank,SDL_Rect *tir_Tank,sprite *avion,SDL_Rect *tir_Avion)
{
    //regarde si dans un premier temps le tir du tank touche l'hélico puis regarde si le tir de l'avion touche l'hélico
    /*if( ( ( (helico->image1.position.x)<=(tir_Tank->x+tank->imageUtilise.tir->w) && (helico->image1.position.x+helico->image1.position.w)>=tir_Tank->x ) ||
         ( (helico->image1.position.y)<=(tir_Tank->y+tank->imageUtilise.tir->h) && (helico->image1.position.y+helico->image1.position.h)>=tir_Tank->y ) ) ||

        ( ( (helico->image1.position.x)<=(tir_Avion->x+avion->imageUtilise.tir->w) && (helico->image1.position.x+helico->image1.position.w)>=tir_Avion->x ) ||
         ( (helico->image1.position.y)<=(tir_Avion->y+avion->imageUtilise.tir->h) && (helico->image1.position.y+helico->image1.position.h)>=tir_Avion->y ) )
       )
       {
            *vie-=1;
       }*/
}
