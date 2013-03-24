/*
actionsCommunes.c
----------

Fait par : Mehdi

Rôle : Regroupement de toute les fonctions qui permet une action commune a deux ellements

Date de dernière modifiction : 24/03/2013

*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/erreur.h"
#include "headers/map.h"
#include "headers/actionsCommunes.h"

int hotage_monte_helico(sprite *helico,sprite *otage, int** map,tilesets *tilesetsMap,int positionActu)
{
    //on regarde si l'hélico est attérie et si un otage "touche" l'hélico
    if(1==atterrissageHelico(helico,map,tilesetsMap,positionActu) && ( ( (helico->image1.position.x)<=(otage->image1.position.x+otage->image1.position.w) && (helico->image1.position.x+helico->image1.position.w)>=otage->image1.position.x ) ||
         ( (helico->image1.position.y)<=(otage->image1.position.y+otage->image1.position.h) && (helico->image1.position.y+helico->image1.position.h)>=otage->image1.position.y ) ) )
    {
        //on rajoute un otage
        return 1;
    }
    //sinon on ne fait rien et on rajoute aucun otage
    else
    {
        return 0;
    }
}

void hotage_dessend_helico(int positionMap,int *nbOtageHelico,int file,sprite *helico, int** map,tilesets *tilesetsMap,int positionActu)
{
    /*On vérifie : Si on est a la base, si on a des otages qui sont dans l'helico,
    si l'hélico est attérie, et si aucun otage est en route pour rentré a la base */
    if(positionMap==0 && *nbOtageHelico>0 && atterrissageHelico(helico,map,tilesetsMap,positionActu)==1 && file==0)
    {
        //fonction a faire pour dirigé les otages vers la base
        *nbOtageHelico-=1;
    }
}

int Helico_ecrase_otage(sprite *helico,sprite *otage,int** map,tilesets *tilesetsMap,int positionActu)
{
     //on regarde si l'hélico n'est pas attérie et si un otage "touche" l'hélico
    if(0==atterrissageHelico(helico,map,tilesetsMap,positionActu) && ( ( (helico->image1.position.x)<=(otage->image1.position.x+otage->image1.position.w) && (helico->image1.position.x+helico->image1.position.w)>=otage->image1.position.x ) ||
         ( (helico->image1.position.y)<=(otage->image1.position.y+otage->image1.position.h) && (helico->image1.position.y+helico->image1.position.h)>=otage->image1.position.y ) ) )
    {
        //on confirme qu'un otage a été écrasé
        return 1;
    }
    //sinon on ne fait rien
    else
    {
        return 0;
    }
}
