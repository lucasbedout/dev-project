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
//Pour tiré des nombres aléatoires
#include <time.h>

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

//--------------------------------DEFINE AVION-----------------------------
//Plus la vitesse est grande et plus l'avion se déplacera rapidement
#define VITESSE_AVION 2
//Plus la latence est petite et plus le tir est relancé rapidement ( 1 latence = 1 seconde par temps de réponse )
#define LATENCE_TIR_AVION 1
//Plus la porter est grande, plus l'avion poura tirré loin
#define PORTER_TIR_AVION 400
//------------------------------------------------------------------------

//--------------------------------DEFINE SOUCOUPE-----------------------------
//Plus la vitesse est grande et plus l'avion se déplacera rapidement
#define VITESSE_SOUCOUPE  3
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
    tank->imageUtilise.tir.image[IMAGE1]=NULL;
    tank->imageUtilise.tir.image[IMAGE1]=IMG_Load(NOM_FICHIER_TIR_TANK);

    //verification que l'image soit bien charger
    if(tank->imageUtilise.tir.image[IMAGE1]==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_TANK);
    }

    //vie du tank
    tank->vie=VIE_TANK;
    tank->tempsMort=0;

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
    limiteDeplacement=tank->imageUtilise.positionEcran->w/tilesetsMap->infoImage[IMAGE1].image->w;

    if(tempsActu>((*tempsTank)+(200/VITESSE_TANK) ) &&
       (positionMap-tank->image[IMAGE1].position.x<limiteDeplacement) && (positionMap-tank->image[IMAGE1].position.x>(limiteDeplacement*-1) )
       && (tank->image[IMAGE1].position.x>saveZone(tank,tilesetsMap) ) )
    {
        //permet de positionner le tank par rapport au sol
        tank->image[IMAGE1].position.y=tank->imageUtilise.positionEcran->h-hauteur_sol_max(map,tilesetsMap,tank->imageUtilise.positionEcran,tank->image[IMAGE1].position.x,
                                                                                           (tank->image[IMAGE1].position.x+tank->image[IMAGE1].image->w/tilesetsMap->infoImage[0].image->w))-tank->image[IMAGE1].image->h;

        //permet au tank de suivre l'hélico ( tout en étant centrer ) avec un temps de latence a chaque déplacement
        if(positionMap>((tank->image[IMAGE1].position.x)+((tank->image[IMAGE1].image->w/2)/tilesetsMap->infoImage[0].image->w)) )
        {
            tank->image[IMAGE1].position.x++;
            tank->imageUtilise.direction=DROITE;
        }
        else if(positionMap<((tank->image[IMAGE1].position.x)+((tank->image[IMAGE1].image->w/2)/tilesetsMap->infoImage[0].image->w)) )
        {
            tank->image[IMAGE1].position.x--;
            tank->imageUtilise.direction=GAUCHE;
        }

        //On vérifie si le tank est dans la saveZone, si c'est le cas, on le tien a distance
        if(tank->image[IMAGE1].position.x<=saveZone(tank,tilesetsMap) )
            tank->image[IMAGE1].position.x=saveZone(tank,tilesetsMap)+1;

        *tempsTank=tempsActu;
    }
}

void calculTrajectoireTank(sprite *tank,sprite *helico,int positionMap,tilesets *tilesetsMap,int tempsActu,int *tempsPrece)
{
    //On regarde si l'hélico est a la porté du tir du tank et si le tank a la possibilité de retirré
    if((tempsActu>(*tempsPrece+1000*LATENCE_TIR_TANK)) &&
       ( ((tank->image[IMAGE1].position.y-helico->image[IMAGE1].position.y)<PORTER_TIR_TANK) &&
        ( ((positionMap-tank->image[IMAGE1].position.x)<( PORTER_TIR_TANK/tilesetsMap->infoImage[IMAGE1].image->w )) &&
         ((positionMap-tank->image[IMAGE1].position.x)>( -1*PORTER_TIR_TANK/tilesetsMap->infoImage[IMAGE1].image->w ))  ) )
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
    avion->image[IMAGE1].image=IMG_Load(NOM_FICHIER_AVION);
    avion->image[IMAGE2].image=IMG_Load(NOM_FICHIER_AVION2);
    avion->image[IMAGE3].image=IMG_Load(NOM_FICHIER_AVION3);
    avion->image[IMAGE4].image=IMG_Load(NOM_FICHIER_AVION4);

    //verification que l'image soit bien charger
    if(avion->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_AVION);
    }
    if(avion->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_AVION2);
    }
    if(avion->image[IMAGE3].image==NULL)
    {
        erreur_image(NOM_FICHIER_AVION3);
    }
    if(avion->image[IMAGE4].image==NULL)
    {
        erreur_image(NOM_FICHIER_AVION4);
    }

    //initialisation des position de l'avion
    avion->image[IMAGE1].position.x=0;
    avion->image[IMAGE1].position.y=0;

    //chargement de l'image du tir associer a l'avion
    for(i=0;i<=IMAGE4;i++)
    {
        avion->imageUtilise.tir.image[i]=NULL;
    }

    avion->imageUtilise.tir.image[IMAGE1]=IMG_Load(NOM_FICHIER_TIR_AVION);
    avion->imageUtilise.tir.image[IMAGE2]=IMG_Load(NOM_FICHIER_TIR_AVION2);
    avion->imageUtilise.tir.image[IMAGE3]=IMG_Load(NOM_FICHIER_TIR_AVION3);
    avion->imageUtilise.tir.image[IMAGE4]=IMG_Load(NOM_FICHIER_TIR_AVION4);

    //verification que l'image soit bien charger
    if(avion->imageUtilise.tir.image[IMAGE1]==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_AVION);
    }
    if(avion->imageUtilise.tir.image[IMAGE2]==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_AVION2);
    }
    if(avion->imageUtilise.tir.image[IMAGE3]==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_AVION3);
    }
    if(avion->imageUtilise.tir.image[IMAGE4]==NULL)
    {
        erreur_image(NOM_FICHIER_TIR_AVION4);
    }

    //vie de l'avion
    avion->vie=VIE_AVION;
    avion->tempsMort=0;

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
    avion->imageUtilise.tir.numeroImage=IMAGE1;
    //-------------------------------
}

void deplacementAvion(sprite *avion,sprite *helico,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsAvion)
{
    int limiteDeplacement=0;
    limiteDeplacement=avion->imageUtilise.positionEcran->w/tilesetsMap->infoImage[IMAGE1].image->w;

    //On vérifie si l'avion peut repartir, si l'hélico est dans l'éparage et si il n'est pas dans la saveZone
    if(tempsActu>((*tempsAvion)+(200/VITESSE_AVION) ) &&
       (positionMap-avion->image[IMAGE1].position.x<limiteDeplacement*2) && (positionMap-avion->image[IMAGE1].position.x>(limiteDeplacement*2*-1) )
       && (avion->image[IMAGE1].position.x>saveZone(avion,tilesetsMap) ) )
    {
        //permet a l'avion de suivre l'hélico ( tout en étant centrer ) avec un temps de latence a chaque déplacement
        if(positionMap>((avion->image[IMAGE1].position.x)+((avion->image[IMAGE1].image->w/2)/tilesetsMap->infoImage[0].image->w)) )
        {
            avion->image[IMAGE1].position.x++;
            avion->imageUtilise.direction=DROITE;
        }
        else if(positionMap<((avion->image[IMAGE1].position.x)+((avion->image[IMAGE1].image->w/2)/tilesetsMap->infoImage[0].image->w)) )
        {
            avion->image[IMAGE1].position.x--;
            avion->imageUtilise.direction=GAUCHE;
        }
        if(helico->image[IMAGE1].position.y>avion->image[IMAGE1].position.y)
        {
            avion->image[IMAGE1].position.y+=VITESSE_AVION;
        }
        else if(helico->image[IMAGE1].position.y<avion->image[IMAGE1].position.y)
        {
            avion->image[IMAGE1].position.y-=VITESSE_AVION;
        }

        //vérifie si la distance qui sépare l'hélico et l'avion est maintenue, sinon on repositionne l'avion
        if( (positionMap-avion->image[IMAGE1].position.x>(limiteDeplacement*-1)/3 )&&  (positionMap-avion->image[IMAGE1].position.x<limiteDeplacement/3)  )
        {
            if(positionMap-avion->image[IMAGE1].position.x>(limiteDeplacement*-1)/3 && positionMap-avion->image[IMAGE1].position.x<0 )
                avion->image[IMAGE1].position.x+=(positionMap-avion->image[IMAGE1].position.x-(limiteDeplacement*-1)/3);
            else if(positionMap-avion->image[IMAGE1].position.x<limiteDeplacement/3 && positionMap-avion->image[IMAGE1].position.x>0 )
                avion->image[IMAGE1].position.x+=(positionMap-avion->image[IMAGE1].position.x-limiteDeplacement/3);
        }

        //Si on est dans la save zone alors on reste a distance
        if(avion->image[IMAGE1].position.x<=saveZone(avion,tilesetsMap) )
            avion->image[IMAGE1].position.x=saveZone(avion,tilesetsMap)+1;

        *tempsAvion=tempsActu;
    }
}

void calculTrajectoireAvion(sprite *avion,sprite *helico,int positionMap,tilesets *tilesetsMap,int tempsActu,int *tempsPrece)
{
    //création de variable qui vont permetre de rendre lisible la condition
    int differenceHauteur=0,positionHelicoY=0,positionAvionX=0,positionAvionY=0;
    differenceHauteur=helico->image[IMAGE1].image->h-avion->image[IMAGE1].image->h;

    positionHelicoY=helico->image[IMAGE1].position.y;

    positionAvionX=avion->image[IMAGE1].position.x;
    positionAvionY=avion->image[IMAGE1].position.y;

    //On regarde si l'hélico est a la porté du tir de l'avion et si l'avion a la possibilité de retiré et si l'avion est en face de l'hélico
    if((tempsActu>(*tempsPrece+1000*LATENCE_TIR_AVION)) && //partie : possibilité de retiré
       ( ((positionAvionY-positionHelicoY)<PORTER_TIR_AVION) && //partie porté avion
        ( ((positionMap-positionAvionX)<( PORTER_TIR_AVION/tilesetsMap->infoImage[IMAGE1].image->w )) && //idem
         ((positionMap-positionAvionX)>( -1*PORTER_TIR_AVION/tilesetsMap->infoImage[IMAGE1].image->w ))  ) && //idem
           ( positionAvionY-(differenceHauteur/2)>=positionHelicoY ) && ( positionAvionY-(differenceHauteur/2)<=positionHelicoY+helico->image[IMAGE1].image->h/2 ) ) ) //partie si l'hélico est en face
    {
        //on prends la position de la cible en prenant la colonne ( unité de valeur : tileset ) et les y en pixel
        avion->imageUtilise.tir.cibleTir.x=positionMap;
        avion->imageUtilise.tir.cibleTir.y=helico->image[IMAGE1].position.y+helico->image[IMAGE1].image->h/2;

        if(avion->imageUtilise.tir.positionTir.x<avion->imageUtilise.tir.cibleTir.x)
            avion->imageUtilise.direction=DROITE;
        else if(avion->imageUtilise.tir.positionTir.x>avion->imageUtilise.tir.cibleTir.x)
            avion->imageUtilise.direction=GAUCHE;

        //la position se fait a partir de la position de la map et on place le tir juste en dessosu de l'hélico
        if(avion->imageUtilise.direction==DROITE)
        {
            avion->imageUtilise.tir.positionTir.x=avion->image[IMAGE1].position.x+avion->image[IMAGE1].image->w/tilesetsMap->infoImage[IMAGE1].image->w;
            avion->imageUtilise.tir.positionTir.y=avion->image[IMAGE1].position.y+avion->image[IMAGE1].image->h/2;
        }
        else if(avion->imageUtilise.direction==GAUCHE)
        {
            avion->imageUtilise.tir.positionTir.x=avion->image[IMAGE1].position.x;
            avion->imageUtilise.tir.positionTir.y=avion->image[IMAGE1].position.y+avion->image[IMAGE1].image->h/2;
        }

        //on déclare enfin que le tir est en cour
        avion->imageUtilise.tir.actionEnCour=1;

        if(avion->imageUtilise.tir.cibleTir.x>avion->imageUtilise.tir.positionTir.x)
        {
            avion->imageUtilise.tir.signeEquation=2;
            avion->imageUtilise.tir.coefDirecteur=0;
            avion->imageUtilise.tir.coefIndice=avion->imageUtilise.tir.cibleTir.y;
        }
        else if(avion->imageUtilise.tir.cibleTir.x<avion->imageUtilise.tir.positionTir.x)
        {
            avion->imageUtilise.tir.signeEquation=1;
            avion->imageUtilise.tir.coefDirecteur=0;
            avion->imageUtilise.tir.coefIndice=avion->imageUtilise.tir.cibleTir.y;
        }

        *tempsPrece=tempsActu;
    }
}
//-------------------------------------------------------------------------------------------------------------

//-----------------------------------------------PARTIE SOUCOUPE-----------------------------------------------
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe)
{
    int i=0;

    //initialise les surface
    for(i=0;i<=IMAGE2;i++)
    {
        soucoupe->image[i].image=NULL;
    }

    //chargement des images de la soucoupe volante
    soucoupe->image[IMAGE1].image=IMG_Load(NOM_FICHIER_SOUCOUPE);
    soucoupe->image[IMAGE2].image=IMG_Load(NOM_FICHIER_SOUCOUPE2);

    //verification que l'image soit bien charger
    if(soucoupe->image[IMAGE1].image==NULL)
    {
        erreur_image(NOM_FICHIER_SOUCOUPE);
    }
    if(soucoupe->image[IMAGE2].image==NULL)
    {
        erreur_image(NOM_FICHIER_SOUCOUPE2);
    }

    //initialisation de la position de la soucoupe volante
    soucoupe->image[IMAGE1].position.x=0;
    soucoupe->image[IMAGE1].position.y=0;

    //vie de la soucoupe
    soucoupe->vie=VIE_SOUCOUPE;
    soucoupe->tempsMort=0;

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

void deplacementSoucoupe(sprite *soucoupe,sprite *helico,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsSoucoupe)
{
    int limiteDeplacement=0;
    limiteDeplacement=soucoupe->imageUtilise.positionEcran->w/tilesetsMap->infoImage[IMAGE1].image->w;

    if(tempsActu>((*tempsSoucoupe)+(200/VITESSE_SOUCOUPE) ) &&
       (positionMap-soucoupe->image[IMAGE1].position.x<limiteDeplacement) && (positionMap-soucoupe->image[IMAGE1].position.x>(limiteDeplacement*-1) )
       && (soucoupe->image[IMAGE1].position.x>saveZone(soucoupe,tilesetsMap) ) )
    {
        //permet a la soucoupe de suivre l'hélico ( tout en étant centrer ) avec un temps de latence a chaque déplacement
        if(positionMap>((soucoupe->image[IMAGE1].position.x)+((soucoupe->image[IMAGE1].image->w/2)/tilesetsMap->infoImage[0].image->w)) )
        {
            soucoupe->image[IMAGE1].position.x++;
        }
        else if(positionMap<((soucoupe->image[IMAGE1].position.x)+((soucoupe->image[IMAGE1].image->w/2)/tilesetsMap->infoImage[0].image->w)) )
        {
            soucoupe->image[IMAGE1].position.x--;
        }
        if(helico->image[IMAGE1].position.y>soucoupe->image[IMAGE1].position.y)
        {
            soucoupe->image[IMAGE1].position.y+=VITESSE_SOUCOUPE;
        }
        else if(helico->image[IMAGE1].position.y<soucoupe->image[IMAGE1].position.y)
        {
            soucoupe->image[IMAGE1].position.y-=VITESSE_SOUCOUPE;
        }

        *tempsSoucoupe=tempsActu;
    }
}
//-------------------------------------------------------------------------------------------------------------

//-----------------------------------------------PARTIE GENERALE-----------------------------------------------
int spawnAlea(sprite *Sprite,tilesets *tilesetsMap)
{
    int nombre_aleatoire=0,limiteMax=taille_map(),limiteMin=saveZone(Sprite,tilesetsMap);

    if(limiteMax==0)
        limiteMax=TAILLE_MAP_PREDEFINIE;

    limiteMax-=Sprite->imageUtilise.positionEcran->w/tilesetsMap->infoImage[IMAGE1].image->w;

    //srand(time(NULL));
    nombre_aleatoire = rand()%(limiteMax-limiteMin) +limiteMin;

    return nombre_aleatoire;
}

int autorisationRespawn(sprite *Sprite,int tempsActu,int tempsRespawn)
{
    if(((tempsRespawn+Sprite->tempsMort))>tempsActu)
        return 0;
    else
        return 1;
}

void respawn(sprite *Sprite,tilesets *tilesetsMap,int tempsActu,int tempsRespawn,int vie)
{
    if(autorisationRespawn(Sprite,tempsActu,tempsRespawn)==1 && Sprite->vie<=0)
    {
        test();
        Sprite->vie=vie;
        Sprite->image[IMAGE1].position.x=spawnAlea(Sprite,tilesetsMap);
    }
}
//-------------------------------------------------------------------------------------------------------------
