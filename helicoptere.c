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
#define VITESSE_PROJECTILE 1

void iniPosHelicoptere(SDL_Surface *ecran,sprite *helico)
{
    //initialise les surface
    helico->image4.image=NULL;
    helico->image3.image=NULL;
    helico->image2.image=NULL;
    helico->image1.image=NULL;

    //chargement de l'hélicoptère au centre de l'écran
    helico->image1.image=IMG_Load(NOM_FICHIER_HELICO);
    helico->image2.image=IMG_Load(NOM_FICHIER_HELICO2);
    helico->image3.image=IMG_Load(NOM_FICHIER_HELICO3);
    helico->image4.image=IMG_Load(NOM_FICHIER_HELICO4);

    //verification que l'image soit bien charger
    if(helico->image1.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO);
    }
    if(helico->image2.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO2);
    }
    if(helico->image3.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO3);
    }
    if(helico->image4.image==NULL)
    {
        erreur_image(NOM_FICHIER_HELICO4);
    }

    //position de l'hélico
    helico->image1.position.x=0;
    helico->image1.position.y=0;
    helico->image1.position.x=((*ecran).w/2)-helico->image1.image->w/2;
    helico->image1.position.y=((*ecran).h/2)-helico->image1.image->h/2;

    //chargement de l'image du tir associer a l'hélicoptère
    helico->imageUtilise.tir=NULL;
    helico->imageUtilise.tir=IMG_Load(NOM_FICHIER_TIR_HELICO);

    //verification que l'image soit bien charger
    if(helico->imageUtilise.tir==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_HELICO);
    }
}


void deplacementHelico(sprite *helico,SDL_Event* even,int *positionMap,tilesets *tilesetsMap,int** map)
{
    switch(even->type)
    {

        case SDL_KEYDOWN:
                switch(even->key.keysym.sym)
                {
                    case SDLK_UP:
                        helico->image1.position.y-=VITESSE_HELICO;
                        break;
                    case SDLK_DOWN:
                        helico->image1.position.y+=VITESSE_HELICO;
                        break;
                    case SDLK_LEFT:
                        //on test si il est pas au bous de la map ou attérie
                        if( (*positionMap>0) && (atterrissageHelico(helico,map,tilesetsMap,*positionMap)==0 ) )
                        {
                            *positionMap=*positionMap-1;
                            helico->imageUtilise.direction=GAUCHE;
                        }
                        break;
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
            SDL_BlitSurface(helico->image2.image,NULL,ecran,&(helico->image1.position));

            return IMAGE2;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE2) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(helico->image1.image,NULL,ecran,&(helico->image1.position));

            return IMAGE1;
        }
    }
    else if(helico->imageUtilise.direction==DROITE)
    {
        //si l'image 3 est blitter et que le temps est supérieur a 50ms on blitte l'image 4
        if( ( (image==IMAGE3) && ((tempsActu-tempsPrece)>50) ) || image==IMAGE1 || image==IMAGE2 )
        {
            SDL_BlitSurface(helico->image4.image,NULL,ecran,&(helico->image1.position));

            return IMAGE4;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE4) && ((tempsActu-tempsPrece)>50) );
        {
            SDL_BlitSurface(helico->image3.image,NULL,ecran,&(helico->image1.position));

            return IMAGE3;
        }
    }
}

void tirHelico(SDL_Event* even,int *actionEnCour,SDL_Rect *cible, SDL_Rect *position,sprite *helico,double *a,double *b,int *equation,int positionMap,tilesets *tilesetsMap)
{
    //le i sera utilisé dans une boucle for plus tard dans cette fonction. Les différences seront comparait dans l'algo pour fluidé la trajectoire
    int i=0,differenceX=0,differenceY=0;

    //si aucune action est en cour
    if(*actionEnCour==0)
    {
        switch(even->type)
        {

            case SDL_MOUSEBUTTONDOWN:
                    switch(even->button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            cible->x=((even->button.x)/tilesetsMap->image[0]->w)+positionMap-((helico->imageUtilise.positionEcran->w/tilesetsMap->image[0]->w)/2);
                            //cible->y=(even->button.y)/tilesetsMap->image[0]->h;
                            cible->y=even->button.y;
                            position->x=positionMap;
                            //position->y=(helico->image1.position.y+helico->image1.position.h)/tilesetsMap->image[0]->h;
                            position->y=(helico->image1.position.y+helico->image1.position.h);
                            *actionEnCour=1;
                            break;
                    }
                    break;
        }
    }
    //sinon si l'action est en cour
    else if(*actionEnCour==1)
    {
        //Déclaration des variables
        const int negatif=1,positif=2;

        //calcul de la différence
        differenceY=cible->y-position->y;
        differenceX=cible->x-position->x;

        //si la position du tir est différente de la position de la cible
        if((cible->x!=position->x) || (cible->y!=position->y) )
        {
            //Si aucune équation a été établie
            if(*equation==0)
            {
                //calcul de a et b de l'équation y=ax+b
                *a=(cible->y*1.0-position->y*1.0)/(cible->x*1.0-position->x*1.0);
                *b=cible->y*1.0-(*a)*cible->x;

                //définition du type d'équation en fonction du x et du tir
                if(differenceX<0)
                {*equation=negatif;}
                else
                {*equation=positif;}
            }

            //Si l'équation est de type négative
            if(*equation==negatif)
            {
                //Si la position dépasse la cible a cause du PAS de la vitesse
                if( (position->x-VITESSE_PROJECTILE)<cible->x )
                {
                    //Alors on attribue les coordonnées de la cible a la position du tir
                    position->x=cible->x;
                    position->y=cible->y;
                }
                //Sinon on calcul le x puis le y de l'équation y=ax+b
                else
                {
                    position->x-=VITESSE_PROJECTILE;
                    position->y=(*a)*(position->x)+(*b);
                }
            }
            //Si l'équation est de type positive
            else if(*equation==positif)
            {
                //Si la position dépasse la cible a cause du PAS de la vitesse
                if( (position->x+VITESSE_PROJECTILE)>cible->x )
                {
                    //Alors on attribue les coordonnées de la cible a la position du tir
                    position->x=cible->x;
                    position->y=cible->y;
                }
                //Sinon on calcul le x puis le y de l'équation y=ax+b
                else
                {
                    position->x+=VITESSE_PROJECTILE;
                    position->y=(*a)*(position->x)+(*b);
                }
            }
        }

        //Sinon, si la position du tir et de la cible sont égaux alors l'action est fini
        else
        {
            //On initialise tout
            *actionEnCour=0;
            *equation=0;
            *a=0;
            *b=0;
        }
    }


}

//gestion des colisions
void GestionColision(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu)
{
    //Taille de l'helico par rapport a la taille des tilesets, le résultat est
    int tailleHelico=helico->image1.image->w/tilesetsMap->image[IMAGE1]->w;

    //On regarde si l'hélico ne sort pas du cadre de la fenetre
    if((helico->image1.position.x+helico->image1.position.w)>(helico->imageUtilise.positionEcran->w))
    {
        helico->image1.position.x--;
    }
    //Si l'hélico ne va pas plus bas que le sol
    if(((helico->image1.position.y+helico->image1.position.h)>(helico->imageUtilise.positionEcran->h-hauteur_sol(map,tilesetsMap,helico->imageUtilise.positionEcran,positionActu))) ||
       ((helico->image1.position.y+helico->image1.position.h)>(helico->imageUtilise.positionEcran->h-hauteur_sol(map,tilesetsMap,helico->imageUtilise.positionEcran,positionActu+tailleHelico))) )
    {
        helico->image1.position.y-=VITESSE_HELICO;
    }
}

int atterrissageHelico(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu)
{
    //Taille de l'helico par rapport a la taille des tilesets, le résultat est
    int tailleHelico=helico->image1.image->w/tilesetsMap->image[IMAGE1]->w;

    //atterie
    if( (helico->image1.position.y+helico->image1.position.h) >=(helico->imageUtilise.positionEcran->h-hauteur_sol(map,tilesetsMap,helico->imageUtilise.positionEcran,positionActu)-VITESSE_HELICO) ||
       (helico->image1.position.y+helico->image1.position.h) >=(helico->imageUtilise.positionEcran->h-hauteur_sol(map,tilesetsMap,helico->imageUtilise.positionEcran,positionActu+tailleHelico)-VITESSE_HELICO) )
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
    if( ( ( (helico->image1.position.x)<=(tir_Tank->x+tank->imageUtilise.tir->w) && (helico->image1.position.x+helico->image1.position.w)>=tir_Tank->x ) ||
         ( (helico->image1.position.y)<=(tir_Tank->y+tank->imageUtilise.tir->h) && (helico->image1.position.y+helico->image1.position.h)>=tir_Tank->y ) ) ||

        ( ( (helico->image1.position.x)<=(tir_Avion->x+avion->imageUtilise.tir->w) && (helico->image1.position.x+helico->image1.position.w)>=tir_Avion->x ) ||
         ( (helico->image1.position.y)<=(tir_Avion->y+avion->imageUtilise.tir->h) && (helico->image1.position.y+helico->image1.position.h)>=tir_Avion->y ) )
       )
       {
            *vie-=1;
       }
}
