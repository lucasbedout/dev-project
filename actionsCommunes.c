/*
actionsCommunes.c
----------

Fait par : Mehdi

Rôle : Regroupement de toute les fonctions qui permet une action commune a deux ellements

Date de dernière modifiction : 05/04/2013

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
#include "headers/helicoptere.h"

#define VITESSE_PROJECTILE 1

int animationSprite(int image,int tempsActu,int tempsPrece,sprite *spriteAnime,tilesets *tilesetsMap,int positionMap)
{
    if(spriteAnime->imageUtilise.direction==GAUCHE)
    {
        //si l'image 1 est blitter et que le temps est supérieur a 50ms on blitte l'image 2
        if( (image!=IMAGE2) && ((tempsActu-tempsPrece)>50) )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE2].image);

            return IMAGE2;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE2) && ((tempsActu-tempsPrece)>50) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE1].image);
            return IMAGE1;
        }
    }
    else if(spriteAnime->imageUtilise.direction==DROITE)
    {
        //si l'image 3 est blitter et que le temps est supérieur a 50ms on blitte l'image 4
        if( (image!=IMAGE4) && ((tempsActu-tempsPrece)>50) )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE4].image);
            return IMAGE4;
        }
        //même logique sauf qu'on veut blitter l'image 3
        else if ( (image==IMAGE4) && ((tempsActu-tempsPrece)>50) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE3].image);
            return IMAGE3;
        }
    }
    else if(spriteAnime->imageUtilise.direction==FACE)
    {
        decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE5].image);
        return IMAGE5;
    }
}

void animationTir (sprite *spriteAnime,tilesets *tilesetsMap,int positionMap)
{
    if(spriteAnime->imageUtilise.tir.directionTir==GAUCHE)
    {
        //si l'image 1 est blitter et que le temps est supérieur a 50ms on blitte l'image 2
        if(  (spriteAnime->imageUtilise.tir.numeroImage!=IMAGE2)  )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE2]);

            spriteAnime->imageUtilise.tir.numeroImage=IMAGE2;
        }
        //même logique sauf qu'on veut blitter l'image 1
        else if ( (spriteAnime->imageUtilise.tir.numeroImage==IMAGE2) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE1]);
            spriteAnime->imageUtilise.tir.numeroImage=IMAGE1;
        }
    }
    else if(spriteAnime->imageUtilise.tir.directionTir==DROITE)
    {
        //si l'image 3 est blitter et que le temps est supérieur a 50ms on blitte l'image 4
        if(  (spriteAnime->imageUtilise.tir.numeroImage!=IMAGE4)   )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE4]);
            spriteAnime->imageUtilise.tir.numeroImage=IMAGE4;
        }
        //même logique sauf qu'on veut blitter l'image 3
        else if ( (spriteAnime->imageUtilise.tir.numeroImage==IMAGE4) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE3]);
            spriteAnime->imageUtilise.tir.numeroImage=IMAGE3;
        }
    }
}

void tir(sprite *typeSprite)
{
    //le i sera utilisé dans une boucle for plus tard dans cette fonction. Les différences seront comparait dans l'algo pour fluidé la trajectoire
    int differenceX=0,differenceY=0;
    //Déclaration des variables
    const int negatif=1,positif=2;

    //calcul de la différence
    differenceY=typeSprite->imageUtilise.tir.cibleTir.y-typeSprite->imageUtilise.tir.positionTir.y;
    differenceX=typeSprite->imageUtilise.tir.cibleTir.x-typeSprite->imageUtilise.tir.positionTir.x;

    //si la position du tir est différente de la position de la cible
    if((typeSprite->imageUtilise.tir.cibleTir.x!=typeSprite->imageUtilise.tir.positionTir.x) || (typeSprite->imageUtilise.tir.cibleTir.y!=typeSprite->imageUtilise.tir.positionTir.y) )
    {
        //Si aucune équation a été établie
        if(typeSprite->imageUtilise.tir.signeEquation==0)
        {
            //calcul de a et b de l'équation y=ax+b
            typeSprite->imageUtilise.tir.coefDirecteur=(typeSprite->imageUtilise.tir.cibleTir.y*1.0-typeSprite->imageUtilise.tir.positionTir.y*1.0)/(typeSprite->imageUtilise.tir.cibleTir.x*1.0-typeSprite->imageUtilise.tir.positionTir.x*1.0);
            typeSprite->imageUtilise.tir.coefIndice=typeSprite->imageUtilise.tir.cibleTir.y*1.0-(typeSprite->imageUtilise.tir.coefDirecteur)*typeSprite->imageUtilise.tir.cibleTir.x;

            //définition du type d'équation en fonction du x et du tir
            if(differenceX<0)
            {typeSprite->imageUtilise.tir.signeEquation=negatif;}
            else
            {typeSprite->imageUtilise.tir.signeEquation=positif;}
        }

            //Si l'équation est de type négative
            if(typeSprite->imageUtilise.tir.signeEquation==negatif)
            {
                //Si la position dépasse la cible a cause du PAS de la vitesse
                if( (typeSprite->imageUtilise.tir.positionTir.x-VITESSE_PROJECTILE)<typeSprite->imageUtilise.tir.cibleTir.x )
                {
                    //Alors on attribue les coordonnées de la cible a la position du tir
                    typeSprite->imageUtilise.tir.positionTir.x=typeSprite->imageUtilise.tir.cibleTir.x;
                    typeSprite->imageUtilise.tir.positionTir.y=typeSprite->imageUtilise.tir.cibleTir.y;
                }
                //Sinon on calcul le x puis le y de l'équation y=ax+b
                else
                {
                    typeSprite->imageUtilise.tir.positionTir.x-=VITESSE_PROJECTILE;
                    typeSprite->imageUtilise.tir.positionTir.y=(typeSprite->imageUtilise.tir.coefDirecteur)*(typeSprite->imageUtilise.tir.positionTir.x)+(typeSprite->imageUtilise.tir.coefIndice);
                }

                typeSprite->imageUtilise.tir.directionTir=GAUCHE;
            }
            //Si l'équation est de type positive
            else if(typeSprite->imageUtilise.tir.signeEquation==positif)
            {
                //Si la position dépasse la cible a cause du PAS de la vitesse
                if( ( typeSprite->imageUtilise.tir.positionTir.x+VITESSE_PROJECTILE)>typeSprite->imageUtilise.tir.cibleTir.x )
                {
                    //Alors on attribue les coordonnées de la cible a la position du tir
                    typeSprite->imageUtilise.tir.positionTir.x=typeSprite->imageUtilise.tir.cibleTir.x;
                    typeSprite->imageUtilise.tir.positionTir.y=typeSprite->imageUtilise.tir.cibleTir.y;
                }
                //Sinon on calcul le x puis le y de l'équation y=ax+b
                else
                {
                    typeSprite->imageUtilise.tir.positionTir.x+=VITESSE_PROJECTILE;
                    typeSprite->imageUtilise.tir.positionTir.y=(typeSprite->imageUtilise.tir.coefDirecteur)*( typeSprite->imageUtilise.tir.positionTir.x)+(typeSprite->imageUtilise.tir.coefIndice);
                }

                typeSprite->imageUtilise.tir.directionTir=DROITE;
            }
    }
    //Sinon, si la position du tir et de la cible sont égaux alors l'action est fini
    else
    {
        //On initialise tout
        typeSprite->imageUtilise.tir.actionEnCour=0;
        typeSprite->imageUtilise.tir.signeEquation=0;
        typeSprite->imageUtilise.tir.coefDirecteur=0;
        typeSprite->imageUtilise.tir.coefIndice=0;
    }
}

void Gestion_Vie_sprite(sprite *Sprite,sprite *helico,tilesets *tilesetsMap,int tempsActu)
{
    //Variable créer pour réduire le nombre de ligne et facilité la lecture de la condition suivante
    int spritePosX=0,spritePosY=0,tirHelicoX=0,tirHelicoY=0,tailleSpriteX=0,tailleSpriteY=0,tailleTirHelicoX=0,tailleTirHelicoY=0;

    spritePosX=Sprite->image[IMAGE1].position.x;
    spritePosY=Sprite->image[IMAGE1].position.y;

    tirHelicoX=helico->imageUtilise.tir.positionTir.x;
    tirHelicoY=helico->imageUtilise.tir.positionTir.y;

    tailleSpriteX=Sprite->image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w;
    tailleSpriteY=Sprite->image[IMAGE1].image->h;

    tailleTirHelicoX=helico->imageUtilise.tir.image[IMAGE1]->w/tilesetsMap->infoImage[0].image->w;
    tailleTirHelicoY=helico->imageUtilise.tir.image[IMAGE1]->h;

    //regarde si dans un premier temps le tir du tank touche l'hélico puis regarde si le tir de l'avion touche l'hélico
    if( helico->imageUtilise.tir.actionEnCour==1 &&
       ( ( ( (tirHelicoX+tailleTirHelicoX)>=spritePosX ) && ( tirHelicoX<=(spritePosX+tailleSpriteX) ) ) &&
        ( ( ( (tirHelicoY+tailleTirHelicoY)>= spritePosY) && ( tirHelicoY<=( spritePosY+tailleSpriteY ) ) ) ) ) )
       {
           //on retire un point de vie
            Sprite->vie--;
            //Si la vie est a 0, on indique la date de la mort
            Sprite->tempsMort=tempsActu;
            //on indique que le tir est fini et on réinitialise le tir
            helico->imageUtilise.tir.actionEnCour=0;
            helico->imageUtilise.tir.signeEquation=0;
       }
}

int hotage_monte_helico(sprite *helico,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu)
{
    //Variable créer pour réduire le nombre de ligne et facilité la lecture de la condition suivante
    int otagePosX=0,otagePosY=0,helicoX=0,helicoY=0,tailleOtageX=0,tailleOtageY=0,tailleHelicoX=0,tailleHelicoY=0;

    otagePosX=Otage->strucSprite.image[IMAGE1].position.x;
    otagePosY=Otage->strucSprite.image[IMAGE1].position.y;

    helicoX=positionActu;
    helicoY=helico->image[IMAGE1].position.y;

    tailleOtageX=Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w;
    tailleOtageY=Otage->strucSprite.image[IMAGE1].image->h;

    //la taille de l'hélico de l'image est divisé par deux car l'image de l'hélico est blité au centre de la fenetre
    tailleHelicoX=(helico->image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w)/2;
    tailleHelicoY=helico->image[IMAGE1].image->h;

    //on regarde si l'hélico est attérie et si un otage "touche" l'hélico
    if(1==atterrissageHelico(helico,map,tilesetsMap,positionActu) && ( ( ( (otagePosX+tailleOtageX)>=(helicoX-tailleHelicoX) ) && ( otagePosX<=(helicoX+tailleHelicoX) ) ) &&
                                                                    ( ( ( (otagePosY+tailleOtageY)>= helicoY) && ( otagePosY<=( helicoY+tailleHelicoY ) ) ) ) ) )
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

void hotage_dessend_helico(int positionMap,int *nbOtageBord,int *nbOtageBase,otage *Otage,sprite *helico,sprite *base, int** map,tilesets *tilesetsMap)
{
    /*On vérifie : Si on est a la base, si on a des otages qui sont dans l'helico,
    si l'hélico est attérie, et si aucun otage est en route pour rentré a la base */
    if(positionMap==0 && *nbOtageBord>0 && atterrissageHelico(helico,map,tilesetsMap,positionMap)==1 && Otage->file==0)
    {
        //fonction a faire pour dirigé les otages vers la base
        *nbOtageBord-=1;
        Otage->file=1;
        Otage->strucSprite.image[IMAGE1].position.x=positionMap;
        Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-
                                                    hauteur_sol_max(map,tilesetsMap,Otage->strucSprite.imageUtilise.positionEcran,positionMap,(positionMap+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w) )-Otage->strucSprite.image[IMAGE1].image->h;

    }
    //Si l'otage est rentrer dans la base
    else if(Otage->file==1 && hotage_rentre_base(base,Otage, map,tilesetsMap,positionMap)==1)
    {
        *nbOtageBase+=1;
        Otage->file=0;
    }
}

int hotage_rentre_base(sprite *base,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu)
{
    //Variable créer pour réduire le nombre de ligne et facilité la lecture de la condition suivante
    int otagePosX=0,otagePosY=0,baseX=0,baseY=0,tailleOtageX=0,tailleOtageY=0,tailleBaseX=0,tailleBaseY=0;

    otagePosX=Otage->strucSprite.image[IMAGE1].position.x;
    otagePosY=Otage->strucSprite.image[IMAGE1].position.y;

    baseX=base->image[IMAGE1].position.x;
    baseY=base->image[IMAGE1].position.y;

    tailleOtageX=Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w;
    tailleOtageY=Otage->strucSprite.image[IMAGE1].image->h;

    tailleBaseX=(base->image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w);
    tailleBaseY=base->image[IMAGE1].image->h;

    //on regarde si l'otage "touche" la base
    if( ( ( (otagePosX+tailleOtageX)>=(baseX) ) && ( otagePosX<=(baseX+tailleBaseX) ) ) &&
                                                                    ( ( ( (otagePosY+tailleOtageY)>= baseY) && ( otagePosY<=( baseY+tailleBaseY ) ) ) ) )
    {
        //Si c'est le cas, on indique qu'un otage est rentré
        return 1;
    }
    //sinon on indique qu'aucun otage est rentré
    else
    {
        return 0;
    }
}

int saveZone(sprite *Sprite,tilesets *tilesetsMap)
{
    int moitierMap=0;
    moitierMap=Sprite->imageUtilise.positionEcran->w/tilesetsMap->infoImage[IMAGE1].image->w/2;

    return moitierMap+1;
}

int nbCaserneMap(sprite *Sprite,tilesets *tilesetsMap)
{
    int nbcaserne=0,limiteMap=taille_map(),savZon=saveZone(Sprite,tilesetsMap),tailleCaserne=0;

    tailleCaserne=Sprite->image[IMAGE1].image->w/tilesetsMap->infoImage[IMAGE1].image->w;

    if(limiteMap==0)
        limiteMap=TAILLE_MAP_PREDEFINIE;

    nbcaserne=(limiteMap-ECART_BASE_CASERNE-savZon-tailleCaserne)/ECART_CASERNE_CASERNE;

    if(nbcaserne<NOMBRE_MAX_CASERNE)
        return nbcaserne;
    else
        return NOMBRE_MAX_CASERNE;
}

int pourcentSavOtage(int nbOtageBord,int nbOtageBase,int nbCaserne)
{
    int prcntOtage=0;

    prcntOtage=100*(nbOtageBord+nbOtageBase)/(nbCaserne*NB_OTAGE_PAR_CASERNE);

    return prcntOtage;
}

//----------------------A VOIR BIEN PLUS TARD-------------------------------------------
int Helico_ecrase_otage(sprite *helico,sprite *otage,int** map,tilesets *tilesetsMap,int positionActu)
{
     //on regarde si l'hélico n'est pas attérie et si un otage "touche" l'hélico
    if(0==atterrissageHelico(helico,map,tilesetsMap,positionActu) && ( ( (helico->image[IMAGE1].position.x)<=(otage->image[IMAGE1].position.x+otage->image[IMAGE1].position.w) && (helico->image[IMAGE1].position.x+helico->image[IMAGE1].position.w)>=otage->image[1].position.x ) ||
         ( (helico->image[IMAGE1].position.y)<=(otage->image[IMAGE1].position.y+otage->image[IMAGE1].position.h) && (helico->image[IMAGE1].position.y+helico->image[IMAGE1].position.h)>=otage->image[IMAGE1].position.y ) ) )
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
