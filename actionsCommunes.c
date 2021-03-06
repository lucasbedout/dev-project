/*
actionsCommunes.c
----------

Fait par : Mehdi

R�le : Regroupement de toute les fonctions qui permet une action commune a deux ellements

Date de derni�re modifiction : 05/04/2013

*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'�diteur de lien
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
        //si l'image 1 est blitter et que le temps est sup�rieur a 50ms on blitte l'image 2
        if( (image!=IMAGE2) && ((tempsActu-tempsPrece)>50) )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE2].image);

            return IMAGE2;
        }
        //m�me logique sauf qu'on veut blitter l'image 1
        else if ( (image==IMAGE2) && ((tempsActu-tempsPrece)>50) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE1].image);
            return IMAGE1;
        }
    }
    else if(spriteAnime->imageUtilise.direction==DROITE)
    {
        //si l'image 3 est blitter et que le temps est sup�rieur a 50ms on blitte l'image 4
        if( (image!=IMAGE4) && ((tempsActu-tempsPrece)>50) )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE4].image);
            return IMAGE4;
        }
        //m�me logique sauf qu'on veut blitter l'image 3
        else if ( (image==IMAGE4) && ((tempsActu-tempsPrece)>50) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE3].image);
            return IMAGE3;
        }
    }
    else if(spriteAnime->imageUtilise.direction==FACE)
    {
        if( (image!=IMAGE5) && ((tempsActu-tempsPrece)>50) )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE5].image);
            return IMAGE5;
        }
        else if((image==IMAGE5) && ((tempsActu-tempsPrece)>50));
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->image[IMAGE1].position.y,spriteAnime->image[IMAGE1].position.x,positionMap,spriteAnime->image[IMAGE6].image);
            return IMAGE6;
        }
    }
}

void animationTir (sprite *spriteAnime,tilesets *tilesetsMap,int positionMap)
{
    if(spriteAnime->imageUtilise.tir.directionTir==GAUCHE)
    {
        //si l'image 1 est blitter et que le temps est sup�rieur a 50ms on blitte l'image 2
        if(  (spriteAnime->imageUtilise.tir.numeroImage!=IMAGE2)  )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE2]);

            spriteAnime->imageUtilise.tir.numeroImage=IMAGE2;
        }
        //m�me logique sauf qu'on veut blitter l'image 1
        else if ( (spriteAnime->imageUtilise.tir.numeroImage==IMAGE2) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE1]);
            spriteAnime->imageUtilise.tir.numeroImage=IMAGE1;
        }
    }
    else if(spriteAnime->imageUtilise.tir.directionTir==DROITE)
    {
        //si l'image 3 est blitter et que le temps est sup�rieur a 50ms on blitte l'image 4
        if(  (spriteAnime->imageUtilise.tir.numeroImage!=IMAGE4)   )
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE4]);
            spriteAnime->imageUtilise.tir.numeroImage=IMAGE4;
        }
        //m�me logique sauf qu'on veut blitter l'image 3
        else if ( (spriteAnime->imageUtilise.tir.numeroImage==IMAGE4) );
        {
            decallement_image_map_hauteurPixel(spriteAnime,tilesetsMap,spriteAnime->imageUtilise.tir.positionTir.y,spriteAnime->imageUtilise.tir.positionTir.x,positionMap,spriteAnime->imageUtilise.tir.image[IMAGE3]);
            spriteAnime->imageUtilise.tir.numeroImage=IMAGE3;
        }
    }
}

int tir(sprite *typeSprite,tilesets tilesetsMap,Mix_Chunk **soundTir)
{
    //le i sera utilis� dans une boucle for plus tard dans cette fonction. Les diff�rences seront comparait dans l'algo pour fluid� la trajectoire
    int differenceX=0,differenceY=0;
    double test=0.0;

    //D�claration des variables
    const int negatif=1,positif=2;

    //calcul de la diff�rence
    differenceY=typeSprite->imageUtilise.tir.cibleTir.y-typeSprite->imageUtilise.tir.positionTir.y;
    differenceX=typeSprite->imageUtilise.tir.cibleTir.x-typeSprite->imageUtilise.tir.positionTir.x;

    //si la position du tir est diff�rente de la position de la cible
    if((typeSprite->imageUtilise.tir.cibleTir.x!=typeSprite->imageUtilise.tir.positionTir.x) || (typeSprite->imageUtilise.tir.cibleTir.y!=typeSprite->imageUtilise.tir.positionTir.y) )
    {
        //Si aucune �quation a �t� �tablie
        if(typeSprite->imageUtilise.tir.signeEquation==0)
        {
            //calcul de a et b de l'�quation y=ax+b
            typeSprite->imageUtilise.tir.coefDirecteur=(typeSprite->imageUtilise.tir.cibleTir.y*1.0-typeSprite->imageUtilise.tir.positionTir.y*1.0)/(typeSprite->imageUtilise.tir.cibleTir.x*1.0-typeSprite->imageUtilise.tir.positionTir.x*1.0);
            typeSprite->imageUtilise.tir.coefIndice=typeSprite->imageUtilise.tir.cibleTir.y*1.0-(typeSprite->imageUtilise.tir.coefDirecteur)*typeSprite->imageUtilise.tir.cibleTir.x;
            //d�finition du type d'�quation en fonction du x et du tir
            if(differenceX<0)
            {typeSprite->imageUtilise.tir.signeEquation=negatif;}
            else
            {typeSprite->imageUtilise.tir.signeEquation=positif;}

            //on d�clenche le son du tir
            if(*soundTir!=NULL)
                Mix_PlayChannel( -1, *soundTir, 0 );
        }

        if( typeSprite->imageUtilise.tir.coefDirecteur<17 && typeSprite->imageUtilise.tir.coefDirecteur>-17 ){
            //Si l'�quation est de type n�gative
            if(typeSprite->imageUtilise.tir.signeEquation==negatif)
            {
                //Si la position d�passe la cible a cause du PAS de la vitesse
                if( (typeSprite->imageUtilise.tir.positionTir.x-VITESSE_PROJECTILE)<typeSprite->imageUtilise.tir.cibleTir.x )
                {
                    //Alors on attribue les coordonn�es de la cible a la position du tir
                    typeSprite->imageUtilise.tir.positionTir.x=typeSprite->imageUtilise.tir.cibleTir.x;
                    typeSprite->imageUtilise.tir.positionTir.y=typeSprite->imageUtilise.tir.cibleTir.y;
                }
                //Sinon on calcul le x puis le y de l'�quation y=ax+b
                else
                {
                    typeSprite->imageUtilise.tir.positionTir.x-=VITESSE_PROJECTILE;
                    typeSprite->imageUtilise.tir.positionTir.y=(typeSprite->imageUtilise.tir.coefDirecteur)*(typeSprite->imageUtilise.tir.positionTir.x)+(typeSprite->imageUtilise.tir.coefIndice);
                }

                typeSprite->imageUtilise.tir.directionTir=GAUCHE;
            }
            //Si l'�quation est de type positive
            else if(typeSprite->imageUtilise.tir.signeEquation==positif)
            {
                //Si la position d�passe la cible a cause du PAS de la vitesse
                if( ( typeSprite->imageUtilise.tir.positionTir.x+VITESSE_PROJECTILE)>typeSprite->imageUtilise.tir.cibleTir.x )
                {
                    //Alors on attribue les coordonn�es de la cible a la position du tir
                    typeSprite->imageUtilise.tir.positionTir.x=typeSprite->imageUtilise.tir.cibleTir.x;
                    typeSprite->imageUtilise.tir.positionTir.y=typeSprite->imageUtilise.tir.cibleTir.y;
                }
                //Sinon on calcul le x puis le y de l'�quation y=ax+b
                else
                {
                    typeSprite->imageUtilise.tir.positionTir.x+=VITESSE_PROJECTILE;
                    typeSprite->imageUtilise.tir.positionTir.y=(typeSprite->imageUtilise.tir.coefDirecteur)*( typeSprite->imageUtilise.tir.positionTir.x)+(typeSprite->imageUtilise.tir.coefIndice);
                }

                typeSprite->imageUtilise.tir.directionTir=DROITE;
            }
        }
        else if( !(typeSprite->imageUtilise.tir.coefDirecteur<17 && typeSprite->imageUtilise.tir.coefDirecteur>-17) )
        {
            //Si l'�quation est de type n�gative
            if(differenceY<=0)
            {
                //Si la position d�passe la cible a cause du PAS de la vitesse
                if( (typeSprite->imageUtilise.tir.positionTir.y-(VITESSE_PROJECTILE*tilesetsMap.infoImage[IMAGE1].image->h) )<typeSprite->imageUtilise.tir.cibleTir.y )
                {
                    //Alors on attribue les coordonn�es de la cible a la position du tir
                    typeSprite->imageUtilise.tir.positionTir.x=typeSprite->imageUtilise.tir.cibleTir.x;
                    typeSprite->imageUtilise.tir.positionTir.y=typeSprite->imageUtilise.tir.cibleTir.y;
                }
                //Sinon on calcul le x puis le y de l'�quation y=ax+b
                else
                {
                    typeSprite->imageUtilise.tir.positionTir.y-=VITESSE_PROJECTILE*tilesetsMap.infoImage[IMAGE1].image->h;
                    typeSprite->imageUtilise.tir.positionTir.x=(typeSprite->imageUtilise.tir.positionTir.y-typeSprite->imageUtilise.tir.coefIndice)/typeSprite->imageUtilise.tir.coefDirecteur;
                }

                typeSprite->imageUtilise.tir.directionTir=GAUCHE;
            }
            //Si l'�quation est de type positive
            else if(differenceY>=0)
            {
                //Si la position d�passe la cible a cause du PAS de la vitesse
                if( ( typeSprite->imageUtilise.tir.positionTir.y+(VITESSE_PROJECTILE*tilesetsMap.infoImage[IMAGE1].image->h) )>typeSprite->imageUtilise.tir.cibleTir.y )
                {
                    //Alors on attribue les coordonn�es de la cible a la position du tir
                    typeSprite->imageUtilise.tir.positionTir.x=typeSprite->imageUtilise.tir.cibleTir.x;
                    typeSprite->imageUtilise.tir.positionTir.y=typeSprite->imageUtilise.tir.cibleTir.y;
                }
                //Sinon on calcul le x puis le y de l'�quation y=ax+b
                else
                {
                    typeSprite->imageUtilise.tir.positionTir.y+=VITESSE_PROJECTILE*tilesetsMap.infoImage[IMAGE1].image->h;
                    typeSprite->imageUtilise.tir.positionTir.x=(typeSprite->imageUtilise.tir.positionTir.y-typeSprite->imageUtilise.tir.coefIndice)/typeSprite->imageUtilise.tir.coefDirecteur;
                }

                typeSprite->imageUtilise.tir.directionTir=DROITE;
            }
        }

        return 1;
    }
    //Sinon, si la position du tir et de la cible sont �gaux alors l'action est fini
    else
    {
        //On initialise tout
        typeSprite->imageUtilise.tir.actionEnCour=0;
        typeSprite->imageUtilise.tir.signeEquation=0;
        typeSprite->imageUtilise.tir.coefDirecteur=0;
        typeSprite->imageUtilise.tir.coefIndice=0;

        return 0;
    }
}

int Gestion_Vie_sprite(sprite *Sprite,sprite *helico,tilesets *tilesetsMap,int tempsActu)
{
    //Variable cr�er pour r�duire le nombre de ligne et facilit� la lecture de la condition suivante
    int spritePosX=0,spritePosY=0,tirHelicoX=0,tirHelicoY=0,tailleSpriteX=0,tailleSpriteY=0,tailleTirHelicoX=0,tailleTirHelicoY=0;

    spritePosX=Sprite->image[IMAGE1].position.x;
    spritePosY=Sprite->image[IMAGE1].position.y;

    tirHelicoX=helico->imageUtilise.tir.positionTir.x;
    tirHelicoY=helico->imageUtilise.tir.positionTir.y;

    tailleSpriteX=Sprite->image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w;
    tailleSpriteY=Sprite->image[IMAGE1].image->h;

    tailleTirHelicoX=helico->imageUtilise.tir.image[IMAGE1]->w/tilesetsMap->infoImage[0].image->w;
    tailleTirHelicoY=helico->imageUtilise.tir.image[IMAGE1]->h;

    //regarde si dans un premier temps le tir du tank touche l'h�lico puis regarde si le tir de l'avion touche l'h�lico
    if( helico->imageUtilise.tir.actionEnCour==1 &&
       ( ( ( (tirHelicoX+tailleTirHelicoX)>=spritePosX ) && ( tirHelicoX<=(spritePosX+tailleSpriteX) ) ) &&
        ( ( ( (tirHelicoY+tailleTirHelicoY)>= spritePosY) && ( tirHelicoY<=( spritePosY+tailleSpriteY ) ) ) ) ) )
       {
           //on retire un point de vie
            Sprite->vie--;
            //initialisation pour stop le tir
            helico->imageUtilise.tir.actionEnCour=0;
            helico->imageUtilise.tir.signeEquation=0;
            //Si la vie est a 0, on indique la date de la mort
            Sprite->tempsMort=tempsActu;

            //si le sprite meur, on r�initialise tout
            if(Sprite->vie<0)
            {
                Sprite->image[IMAGE1].position.x=0;
                Sprite->image[IMAGE1].position.y=0;
            }

            return 1;
       }
       else
            return 0;
}

int hotage_monte_helico(sprite *helico,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu)
{
    //Variable cr�er pour r�duire le nombre de ligne et facilit� la lecture de la condition suivante
    int otagePosX=0,otagePosY=0,helicoX=0,helicoY=0,tailleOtageX=0,tailleOtageY=0,tailleHelicoX=0,tailleHelicoY=0;

    otagePosX=Otage->strucSprite.image[IMAGE1].position.x;
    otagePosY=Otage->strucSprite.image[IMAGE1].position.y;

    helicoX=positionActu;
    helicoY=helico->image[IMAGE1].position.y;

    tailleOtageX=Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w;
    tailleOtageY=Otage->strucSprite.image[IMAGE1].image->h;

    //la taille de l'h�lico de l'image est divis� par deux car l'image de l'h�lico est blit� au centre de la fenetre
    tailleHelicoX=(helico->image[IMAGE5].image->w/tilesetsMap->infoImage[0].image->w)/2;
    tailleHelicoY=helico->image[IMAGE5].image->h;

    //on regarde si l'h�lico est att�rie et si un otage "touche" l'h�lico ( 2 et 6 son adapter pour augment� la colision en fonction de l'image de l'h�lico
    if(1==atterrissageHelico(helico,map,tilesetsMap,positionActu) && ( ( ( (otagePosX+tailleOtageX)>=(helicoX-tailleHelicoX/2) ) && ( otagePosX<=(helicoX+tailleHelicoX/6) ) ) &&
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
    /*On v�rifie : Si on est a la base, si on a des otages qui sont dans l'helico,
    si l'h�lico est att�rie, et si aucun otage est en route pour rentr� a la base */
    if(positionMap==0 && *nbOtageBord>0 && atterrissageHelico(helico,map,tilesetsMap,positionMap)==1 && Otage->file==0)
    {
        //fonction a faire pour dirig� les otages vers la base
        *nbOtageBord-=1;
        Otage->file=1;
        Otage->strucSprite.image[IMAGE1].position.x=positionMap-helico->image[IMAGE1].image->w/2/tilesetsMap->infoImage[IMAGE1].image->w;
        Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.image[IMAGE1].position.y=Otage->strucSprite.imageUtilise.positionEcran->h-
                                                    hauteur_sol_max(map,tilesetsMap,Otage->strucSprite.imageUtilise.positionEcran,positionMap,
                                                                    (positionMap+Otage->strucSprite.image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w) )-Otage->strucSprite.image[IMAGE1].image->h;

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
    //Variable cr�er pour r�duire le nombre de ligne et facilit� la lecture de la condition suivante
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
        //Si c'est le cas, on indique qu'un otage est rentr�
        return 1;
    }
    //sinon on indique qu'aucun otage est rentr�
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

int pourcentSavOtageBase(int nbOtageBase,int nbCaserne)
{
    int prcntOtage=0;

    prcntOtage=100*nbOtageBase/nbCaserne*NB_OTAGE_PAR_CASERNE;

    return prcntOtage;
}

int restOtage(otage tabOtage[],int nbCaserne,int nbOtageBord)
{
    int i=0,nbOtage=0;

    //On compte si il reste des otages dans des casernes
    for(i=0;i<nbCaserne;i++)
        nbOtage+=tabOtage[i].nbOtage;

    //Si il en reste aucun dans les casernes, a bord et que le dernier otage soit rentr� dans la caserne, alors il ne reste plus d'otage
    if(nbOtage==0 && nbOtageBord==0 && tabOtage[i].file==0)
        return 0;
    else
        return 1;
}

int colisionSprite(sprite spriteMouvant,sprite spriteColision,tilesets tilesetsMap)
{
    //Variable cr�er pour r�duire le nombre de ligne et facilit� la lecture de la condition suivante
    int spriteMovPosX=0,spriteMovPosY=0,spriteColPosX=0,spriteColPosY=0,tailleSpriteMovX=0,tailleSpriteMovY=0,tailleSpriteColX=0,tailleSpriteColY=0;

    spriteMovPosX=spriteMouvant.image[IMAGE1].position.x;
    spriteMovPosY=spriteMouvant.image[IMAGE1].position.y;

    spriteColPosX=spriteColision.image[IMAGE1].position.x;
    spriteColPosY=spriteColision.image[IMAGE1].position.y;

    tailleSpriteMovX=spriteMouvant.image[IMAGE1].image->w/tilesetsMap.infoImage[0].image->w;
    tailleSpriteMovY=spriteMouvant.image[IMAGE1].image->h;

    tailleSpriteColX=spriteColision.image[IMAGE1].image->w/tilesetsMap.infoImage[0].image->w;
    tailleSpriteColY=spriteColision.image[IMAGE1].image->h;

    //regarde si il y a eu colision
    if( ( ( ( (spriteColPosX+tailleSpriteColX)>=spriteMovPosX ) && ( spriteColPosX<=(spriteMovPosX+tailleSpriteMovX) ) ) &&
        ( ( ( (spriteColPosY+tailleSpriteColY)>= spriteMovPosY) && ( spriteColPosY<=( spriteMovPosY+tailleSpriteMovY ) ) ) ) ) )
       return 1;
    else
        return 0;
}

int colisionSpriteHelico(sprite helico,sprite Sprite,tilesets tilesetsMap,int positionMap)
{
    //Variable cr�er pour r�duire le nombre de ligne et facilit� la lecture de la condition suivante
    int helicoPosX=0,helicoPosY=0,SpriteX=0,SpriteY=0,tailleHelicoX=0,tailleHelicoY=0,tailleSpriteX=0,tailleSpriteY=0;

    helicoPosX=positionMap;
    helicoPosY=helico.image[IMAGE1].position.y;

    SpriteX=Sprite.image[IMAGE1].position.x;
    SpriteY=Sprite.image[IMAGE1].position.y;

    tailleHelicoX=helico.image[IMAGE1].image->w/tilesetsMap.infoImage[IMAGE1].image->w;
    tailleHelicoY=helico.image[IMAGE1].image->h;

    tailleSpriteX=Sprite.image[IMAGE1].image->w/tilesetsMap.infoImage[IMAGE1].image->w;
    tailleSpriteY=Sprite.image[IMAGE1].image->h;

    //regarde si dans un premier temps le tir du tank touche l'h�lico puis regarde si le tir de l'avion touche l'h�lico
    if( ( ( ( (SpriteX+tailleSpriteX)>=(helicoPosX-tailleHelicoX/2) ) && ( SpriteX<=(helicoPosX+tailleHelicoX/2) ) ) &&
        ( ( ( (SpriteY+tailleSpriteY)>= helicoPosY) && ( SpriteY<=( helicoPosY+tailleHelicoY ) ) ) ) ) )
        return 1;
    else
        return 0;
}

void iniExplosion(imgMenu *explosion,SDL_Surface *ecran)
{
    explosion->img=NULL;

    //On charge l'image et on regarde si il y a une erreur
    explosion->img=IMG_Load(NOM_FICHIER_EXPLOSION);
    if(explosion->img==NULL)
        erreur_image(NOM_FICHIER_EXPLOSION);

    //On attribu les propri�t� �cran
    explosion->ecran=ecran;

    //On initialise la position
    explosion->positionImg.x=0;
    explosion->positionImg.y=0;
}

void declenchementExplosion(imgMenu *explosion,sprite *spriteToucher, sprite spriteTirreur,Mix_Chunk **soundTir)
{
    //on d�clenche le son du tir
    if(*soundTir!=NULL)
        Mix_PlayChannel( -1, *soundTir, 0 );

    spriteToucher->imageUtilise.tir.nbExplosion=NB_EXPLOSION;
    explosion->positionImg.x=spriteTirreur.imageUtilise.tir.positionTir.x;
    explosion->positionImg.y=spriteTirreur.imageUtilise.tir.positionTir.y;
}

void afficheExplosion(sprite *Sprite,imgMenu explosion,tilesets tilesetsMap,int positionMap)
{
    decallement_image_map_hauteurPixel(Sprite,&tilesetsMap,explosion.positionImg.y,explosion.positionImg.x,positionMap,explosion.img);
    Sprite->imageUtilise.tir.nbExplosion-=1;
}

int Helico_ecrase_otage(sprite helico,sprite Otage,int** map,tilesets tilesetsMap,int positionActu)
{
     //on regarde si l'h�lico n'est pas att�rie et si un otage "touche" l'h�lico
    if( !(atterrissageHelico(&helico,map,&tilesetsMap,positionActu)) && colisionSpriteHelico(helico,Otage,tilesetsMap,positionActu))
    {
        //on confirme qu'un otage a �t� �cras�
        return 1;
    }
    //sinon on ne fait rien
    else
    {
        return 0;
    }
}

void jaugeVie(sprite spriteCible,int vieTotal,int vieActu,int largueurTile,int positionMap)
{
    //bare de 50 pixel max en largeur
    int longueurBare=100*vieActu/vieTotal,prctVie=vieActu*1.0/vieTotal*100.0;

    SDL_Surface *bareVie=NULL;
    SDL_Rect posiBare={0};

    bareVie=SDL_CreateRGBSurface(SDL_HWSURFACE,longueurBare,10,32,0,0,0,0);
    SDL_FillRect(bareVie,NULL,SDL_MapRGB(spriteCible.imageUtilise.positionEcran->format,2.55*(100-prctVie),2.55*prctVie,0));

    //calcul des coordonn�es de la bare
    posiBare.x=(spriteCible.image[0].position.x-positionMap)*largueurTile+spriteCible.imageUtilise.positionEcran->w/2+spriteCible.image[0].image->w/2-bareVie->w/2;
    posiBare.y=spriteCible.image[0].position.y-bareVie->h*2;

    SDL_BlitSurface(bareVie, NULL, spriteCible.imageUtilise.positionEcran, &posiBare);

    SDL_FreeSurface(bareVie);

}
