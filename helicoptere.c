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
    for(i=0;i<=IMAGE6;i++)
    {
        helico->image[i].image=NULL;
    }

    //chargement de l'hélicoptère au centre de l'écran
    helico->image[IMAGE1].image=IMG_Load(NOM_FICHIER_HELICO);
    helico->image[IMAGE2].image=IMG_Load(NOM_FICHIER_HELICO2);
    helico->image[IMAGE3].image=IMG_Load(NOM_FICHIER_HELICO3);
    helico->image[IMAGE4].image=IMG_Load(NOM_FICHIER_HELICO4);
    helico->image[IMAGE5].image=IMG_Load(NOM_FICHIER_HELICO5);
    helico->image[IMAGE6].image=IMG_Load(NOM_FICHIER_HELICO6);

    //verification que l'image soit bien charger
    if(helico->image[IMAGE1].image==NULL)
        erreur_image(NOM_FICHIER_HELICO);
    if(helico->image[IMAGE2].image==NULL)
        erreur_image(NOM_FICHIER_HELICO2);
    if(helico->image[IMAGE3].image==NULL)
        erreur_image(NOM_FICHIER_HELICO3);
    if(helico->image[IMAGE4].image==NULL)
        erreur_image(NOM_FICHIER_HELICO4);
    if(helico->image[IMAGE5].image==NULL)
        erreur_image(NOM_FICHIER_HELICO5);
    if(helico->image[IMAGE6].image==NULL)
        erreur_image(NOM_FICHIER_HELICO6);

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
    helico->imageUtilise.tir.nbExplosion=0;
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
                        helico->imageUtilise.direction=FACE;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        helico->image[IMAGE1].position.y+=VITESSE_HELICO;
                        helico->imageUtilise.direction=FACE;
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
        if( (image!=IMAGE2) && ((tempsActu-tempsPrece)>50) )
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
        if( (image!=IMAGE4) && ((tempsActu-tempsPrece)>50) )
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
    else if(helico->imageUtilise.direction==FACE)
    {
        if( (image!=IMAGE5) && ((tempsActu-tempsPrece)>50) )
        {
            SDL_BlitSurface(helico->image[IMAGE5].image,NULL,ecran,&(helico->image[IMAGE1].position));

            return IMAGE5;
        }
        else if ( (image==IMAGE5) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(helico->image[IMAGE6].image,NULL,ecran,&(helico->image[IMAGE1].position));

            return IMAGE6;
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

int Gestion_Vie_helico(sprite *helico,sprite *ennemie,int positionMap,tilesets tilesetsMap)
{
    //Variable créer pour réduire le nombre de ligne et facilité la lecture de la condition suivante
    int helicoPosX=0,helicoPosY=0,tirSpriteX=0,tirSpriteY=0,tailleHelicoX=0,tailleHelicoY=0,tailleTirSpriteX=0,tailleTirSpriteY=0;

    helicoPosX=positionMap;
    helicoPosY=helico->image[IMAGE1].position.y;

    tirSpriteX=ennemie->imageUtilise.tir.positionTir.x;
    tirSpriteY=ennemie->imageUtilise.tir.positionTir.y;

    tailleHelicoX=helico->image[IMAGE1].image->w/tilesetsMap.infoImage[IMAGE1].image->w;
    tailleHelicoY=helico->image[IMAGE1].image->h;

    tailleTirSpriteX=ennemie->imageUtilise.tir.image[IMAGE1]->w/tilesetsMap.infoImage[IMAGE1].image->w;
    tailleTirSpriteY=ennemie->imageUtilise.tir.image[IMAGE1]->h;

    //regarde si dans un premier temps le tir du tank touche l'hélico puis regarde si le tir de l'avion touche l'hélico
    if( ennemie->imageUtilise.tir.actionEnCour==1 &&
        ( ( ( (tirSpriteX+tailleTirSpriteX)>=(helicoPosX-tailleHelicoX/2) ) && ( tirSpriteX<=(helicoPosX+tailleHelicoX/2) ) ) &&
        ( ( ( (tirSpriteY+tailleTirSpriteY)>= helicoPosY) && ( tirSpriteY<=( helicoPosY+tailleHelicoY ) ) ) ) ) )
        {
            //on retire un point de vie
            helico->vie--;

            //on indique que le tir est fini et on réinitialise le tir
            ennemie->imageUtilise.tir.actionEnCour=0;
            ennemie->imageUtilise.tir.signeEquation=0;
            return 1;
        }
    else
        return 0;
}

int gestion_colision_helico(sprite *helico,sprite *ennemie,int positionMap,tilesets tilesetsMap)
{
    //Si l'hélico se fait toucher par la soucoupe
    if( colisionSpriteHelico(*helico,*ennemie,tilesetsMap,positionMap)==1 && ennemie->vie>0 )
       {
           //l'hélico est détruit
            helico->vie=0;

            return 1;
       }
    else
        return 0;
}

void gestionColisionSprite(sprite *helico,sprite spriteCible,tilesets tilesetsMap,int *positionMap,SDL_Event even)
{
    if(colisionSpriteHelico(*helico,spriteCible,tilesetsMap,*positionMap))
        {
            while(colisionSpriteHelico(*helico,spriteCible,tilesetsMap,*positionMap))
            {
                if(even.key.keysym.sym==SDLK_LEFT || even.key.keysym.sym==SDLK_a)
                    *positionMap+=1;
                else if(even.key.keysym.sym==SDLK_RIGHT || even.key.keysym.sym==SDLK_d)
                    *positionMap-=1;
                else if(even.key.keysym.sym==SDLK_DOWN || even.key.keysym.sym==SDLK_s)
                    helico->image[IMAGE1].position.y--;
                else if(even.key.keysym.sym==SDLK_UP || even.key.keysym.sym==SDLK_w)
                    helico->image[IMAGE1].position.y++;
            }
        }
}
