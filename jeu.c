/*
jeu.c
----------

Fait par : Mehdi

Rôle : Permet de lancer le jeu et les fonction principal comme l'hélicoptère, les énnemies, la map, etc...

Date de dernière modification : 26/03/2013

*/
#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/jeu.h"
#include "headers/helicoptere.h"
#include "headers/actionsCommunes.h"
#include "headers/otage.h"
#include "headers/ennemie.h"
#include "headers/erreur.h"
#include "headers/map.h"

void jeu (SDL_Surface *ecran)
{
    //création d'un evennement
    SDL_Event even;

    //-------------------VARIABLE BASIQUE--------------------------
    //Sera utilisé pour une boucle ou double boucle
    int i=0,j=0;
    //--------------------------------------------------------------

    //--------------------CHARGEMENT DE LA MAP---------------------
    //map 2D contenant tout simplement des chiffres
    int **map=NULL;
    //positionMap permet de déplacé la map pour faire un scrolling
    int positionMap=0;
    tilesets tilesetsMap;

    #if MODE_DESIGNER==1
    chargTileAuto("img/Tilesets",&tilesetsMap);
    #else
    declaration_tilesets(&tilesetsMap);
    #endif

    map=chargement_map(ecran,tilesetsMap);
    //--------------------------------------------------------------

    //---------VARIABLE HELICO---------------
    //Initilisation position de l'hélico et chargement sprite
    sprite helico;
    iniPosHelicoptere(ecran,&helico);
    //---------------------------------------

    //---------VARIABLE SOUCOUPE VOLANTE--------
    //------------------------------------------

    //---------VARIABLE TANK--------------------
    sprite *tank;
    tank=malloc(sizeof(sprite)*NOMBRE_TANK);
    for(i=0;i<NOMBRE_TANK;i++)
    {
        iniTank(ecran,&tank[i]);
        tank[i].image[IMAGE1].position.x=spawnAlea(&tank[i],&tilesetsMap);
    }
    //------------------------------------------

    //---------VARIABLE AVION-------------------
    sprite *avion;
    avion=malloc(sizeof(sprite)*NOMBRE_AVION);
    for(i=0;i<NOMBRE_AVION;i++)
    {
        iniAvion(ecran,&avion[i]);
        avion[i].image[IMAGE1].position.x=spawnAlea(&avion[i],&tilesetsMap);
    }
    //------------------------------------------

    //---------VARIABLE SOUCOUPE-------------------
    sprite *soucoupe;
    soucoupe=malloc(sizeof(sprite)*NOMBRE_SOUCOUPE);
    for(i=0;i<NOMBRE_SOUCOUPE;i++)
    {
        iniSoucoupe(ecran,&soucoupe[i]);
        soucoupe[i].image[IMAGE1].position.x=spawnAlea(&soucoupe[i],&tilesetsMap);
    }
    //------------------------------------------

    //---------VARIABLE BASE--------------------
    sprite base,bariere;
    iniBase(ecran,&base,map,&tilesetsMap);
    iniBariere(ecran,&bariere,map,&tilesetsMap);
    //------------------------------------------

    //---------VARIABLE OTAGE-------------------
    //On cherche a savoir le nombre de caserne en fonction de la map pour trouver le nombre d'otage.
    int nbCaserne=nbCaserneMap(&helico,&tilesetsMap);
    //le plus "+1" serat le sprite qui se dirigera vers la base
    otage *Otage;
    Otage=malloc(sizeof(otage)*(nbCaserne+1) );
    //------------------------------------------

    //---------VARIABLE CASERNE-----------------
    sprite *caserne;
    caserne=malloc(sizeof(sprite)*(nbCaserne+1) );
    //On commence le point d'origine ( de la base ) des caserne après la moitier de la fenetre
    for(i=0;i<nbCaserne || i<NOMBRE_MAX_CASERNE;i++)
    {
        iniCaserne(ecran,&caserne[i],map,&tilesetsMap,( (ecran->w/tilesetsMap.infoImage[0].image->w/2)+i*ECART_BASE_CASERNE+ECART_CASERNE_CASERNE));
        iniOtage(ecran,&(Otage[i]),map,&tilesetsMap,caserne[i].image[IMAGE1].position.x);
    }
    /*initialisation du sprite Otage pour la base et la file par défaut a 0 car le joueur doit
    cherché d'abord les otages pour ensuite les déposé et créeer une file*/
     iniOtage(ecran,&(Otage[nbCaserne]),map,&tilesetsMap,0);
     Otage[nbCaserne].file=0;
    //------------------------------------------

    //----------------DECLARATION TEMPS JEU-----------------------
    temps tempsJeu;
    initilisationTemps(tempsJeu.tempsPrecedent);
    //------------------------------------------------------------

    //---------------Blitage---------------------
    SDL_BlitSurface(helico.image[IMAGE1].image,NULL,ecran,&helico.image[IMAGE1].position);
    //--------------------------------------------

    SDL_Flip(ecran);

    //activation des touches répétter
    SDL_EnableKeyRepeat(10,10);

    //-------------VARIABLE DU JEU GLOBAL-----------------------------
    int continuer=verif_erreur();

    //Variables concernant les otages et a la fois l'hélico
    int nbOtageBord=0;
    int nbOtageBase=0;

    tempsJeu.tempsActuel=SDL_GetTicks(); //temps actuel du jeu
    //----------------------------------------------------------------

    while(continuer)
    {
        option(&continuer,&even);

        deplacementHelico(&helico,&even,&positionMap,&tilesetsMap,map);

        //On blitte toute les surfaces et on rafraichie l'image
        affiche_map(map,ecran,tilesetsMap,positionMap);

        //-------------PARTIE TIR-------------
        //vérifie si l'hélico tir
        if(helico.imageUtilise.tir.actionEnCour==0)
        {
            //Si l'utilisateur tir, on calcullera la trajectoire
            calculTrajectoireTirHelico(&even,&helico,positionMap,&tilesetsMap);
        }
        //Si l'action est en cour, on blitte le tir enfonction de l'équation
        else
        {
            tir(&helico);
            decallement_image_map_hauteurPixel(&helico,&tilesetsMap,helico.imageUtilise.tir.positionTir.y,helico.imageUtilise.tir.positionTir.x,positionMap,helico.imageUtilise.tir.image[IMAGE1]);
        }
        //-------------FIN PARTIE TIR----------------

        //Gestion des colisions tir
        GestionColision(&helico,map,&tilesetsMap,positionMap);
        //On blitte les animations de l'hélico en fonction si l'hélico est atérie ou pas
        if(0==atterrissageHelico(&helico,map,&tilesetsMap,positionMap))
        {
            helico.imageUtilise.numeroImage=animationHelico(helico.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[1],ecran,&helico);
        }
        else
        {
            SDL_BlitSurface(helico.image[IMAGE1].image,NULL,ecran,&helico.image[IMAGE1].position);
        }

        //-------------animation ennemies---------
        for(i=0;i<NOMBRE_TANK && ((i*IMPORTANCE_TANK)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne)) ;i++)
        {
            if(tank[i].vie>0)
            {
                deplacementTank(&tank[i],positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[13+(i*TMP_DECAL_TAB_TANK)]));
                tank[i].imageUtilise.numeroImage=animationSprite(tank[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[12+(i*TMP_DECAL_TAB_TANK)],&tank[i],&tilesetsMap,positionMap);
                //vérifie si le tank tir
                if(tank[i].imageUtilise.tir.actionEnCour==0)
                {
                    //Si l'utilisateur tir, on calcullera la trajectoire
                    calculTrajectoireTank(&tank[i],&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[14+(i*TMP_DECAL_TAB_TANK)]));
                }
                //Si l'action est en cour, on blitte le tir enfonction de l'équation
                else
                {
                    if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[16+(i*TMP_DECAL_TAB_TANK)]+1000/VITESSE_TIR_ENNEMIE) )
                    {
                        tir(&tank[i]);
                        tempsJeu.tempsPrecedent[16+(i*TMP_DECAL_TAB_TANK)]=tempsJeu.tempsActuel;
                    }
                    decallement_image_map_hauteurPixel(&tank[i],&tilesetsMap,tank[i].imageUtilise.tir.positionTir.y,tank[i].imageUtilise.tir.positionTir.x,positionMap,tank[i].imageUtilise.tir.image[IMAGE1]);
                }
                Gestion_Vie_sprite(&tank[i],&helico,&tilesetsMap,tempsJeu.tempsActuel);
            }
            else
                respawn(&tank[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_TANK,VIE_TANK);
        }

        for(i=0;i<NOMBRE_AVION && (((i+1)*IMPORTANCE_AVION)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(avion[i].vie>0)
            {
                deplacementAvion(&avion[i],&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[26+(i*TMP_DECAL_TAB_AVION)]));
                avion[i].imageUtilise.numeroImage=animationSprite(avion[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[25+(i*TMP_DECAL_TAB_AVION)],&avion[i],&tilesetsMap,positionMap);
                //vérifie si l'avion tir
                if(avion[i].imageUtilise.tir.actionEnCour==0)
                {
                    //Si l'utilisateur tir, on calcullera la trajectoire
                    calculTrajectoireAvion(&avion[i],&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[27+(i*TMP_DECAL_TAB_AVION)]));
                }
                //Si l'action est en cour, on blitte le tir enfonction de l'équation
                else
                {
                    if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[28+(i*TMP_DECAL_TAB_AVION)]+1000/VITESSE_TIR_ENNEMIE) )
                    {
                        tir(&avion[i]);
                        tempsJeu.tempsPrecedent[28+(i*TMP_DECAL_TAB_AVION)]=tempsJeu.tempsActuel;
                    }
                    animationTir(&avion[i],&tilesetsMap,positionMap);
                }
                Gestion_Vie_sprite(&avion[i],&helico,&tilesetsMap,tempsJeu.tempsActuel);
            }
            else
                respawn(&avion[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_AVION,VIE_AVION);
        }

        for(i=0;i<NOMBRE_SOUCOUPE && (((i+1)*IMPORTANCE_SOUCOUPE)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(soucoupe[i].vie>0)
            {
                deplacementSoucoupe(&soucoupe[i],&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[30]));
                soucoupe[i].imageUtilise.numeroImage=animationSprite(soucoupe[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[29],&soucoupe[i],&tilesetsMap,positionMap);
                Gestion_Vie_sprite(&soucoupe[i],&helico,&tilesetsMap,tempsJeu.tempsActuel);
            }
            else
                respawn(&tank[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_SOUCOUPE,VIE_SOUCOUPE);
        }
        //------------------------------

        //--------------CASERNE ET OTAGE------------
        for(i=0;i<nbCaserne;i++)
        {
            if(caserne[i].vie>0)
            {
                decallement_image_map_hauteurPixel(&caserne[i],&tilesetsMap,caserne[i].image[IMAGE1].position.y,caserne[i].image[IMAGE1].position.x,positionMap,caserne[i].image[IMAGE1].image);
                //Si la caserne se fait attaquer par un ennemie ou l'hélico
                for(j=0;j<NOMBRE_TANK;j++)
                    Gestion_Vie_sprite(&caserne[i],&tank[j],&tilesetsMap,tempsJeu.tempsActuel);
                Gestion_Vie_sprite(&caserne[i],&helico,&tilesetsMap,tempsJeu.tempsActuel);
            }
            else
            {
                //concerne les casernes
                decallement_image_map_hauteurPixel(&caserne[i],&tilesetsMap,caserne[i].image[IMAGE1].position.y,caserne[i].image[IMAGE1].position.x,positionMap,caserne[i].image[IMAGE2].image);

                if(Otage[i].nbOtage>0)
                {
                    //concerne les otages
                    deplacementOtageVersHelico(&Otage[i],&helico,&bariere,map,&tilesetsMap,positionMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[nbCaserne+i]) );
                    Otage[i].strucSprite.imageUtilise.numeroImage=animationSprite(Otage[i].strucSprite.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[nbCaserne+i],&Otage[i].strucSprite,&tilesetsMap,positionMap);
                    //Si l'otage se fait attaqué par un ennemie ou l'hélico
                    Gestion_Vie_sprite(&(Otage[i].strucSprite),&helico,&tilesetsMap,tempsJeu.tempsActuel);
                    for(j=0;j<NOMBRE_TANK;j++)
                        Gestion_Vie_sprite(&(Otage[i].strucSprite),&tank[j],&tilesetsMap,tempsJeu.tempsActuel);
                    gestionFileOtage(&(Otage[i]),&helico,caserne[i].image[IMAGE1].position.x,caserne[i].image[IMAGE1].position.y,map,&tilesetsMap,positionMap,&nbOtageBord);
                }
            }
        }
        //--------------------------------

        //--------------BASE ET OTAGE------------
        decallement_image_map_hauteurPixel(&base,&tilesetsMap,base.image[IMAGE1].position.y,base.image[IMAGE1].position.x,positionMap,base.image[IMAGE1].image);
        //On vérifie si on otage veut déssendre et rejoindre la base
        hotage_dessend_helico(positionMap,&nbOtageBord,&nbOtageBase,&Otage[nbCaserne],&helico,&base,map,&tilesetsMap);
        if(Otage[nbCaserne].file==1)
        {
            deplacementOtageVersBase(&Otage[nbCaserne],&helico,&base,map,&tilesetsMap,positionMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[nbCaserne+nbCaserne]) );
            Otage[nbCaserne].strucSprite.imageUtilise.numeroImage=animationSprite(Otage[nbCaserne].strucSprite.imageUtilise.numeroImage,tempsJeu.tempsActuel,
                                                                                  tempsJeu.tempsPrecedent[nbCaserne+nbCaserne],&Otage[nbCaserne].strucSprite,&tilesetsMap,positionMap);
        }
        decallement_image_map_hauteurPixel(&bariere,&tilesetsMap,bariere.image[IMAGE1].position.y,bariere.image[IMAGE1].position.x,positionMap,bariere.image[IMAGE1].image);
        //---------------------------------------

        SDL_Flip(ecran);

        //Mise en place des FPS
        tempsJeu.tempsActuel=SDL_GetTicks();
        if((tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0])<(TEMPS_POSE))
        {
            SDL_Delay(TEMPS_POSE-(tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0]));
        }
    }

    //on libere les images chargeren mémoire
    for(i=0;i<=IMAGE4;i++)
    {
        SDL_FreeSurface(helico.image[i].image);
        for(j=0;j<NOMBRE_TANK;j++)
            SDL_FreeSurface(tank[j].image[i].image);
        for(j=0;j<NOMBRE_AVION;j++)
        {
            SDL_FreeSurface(avion[j].image[i].image);
            SDL_FreeSurface(avion[j].imageUtilise.tir.image[i]);
        }
    }
    SDL_FreeSurface(helico.imageUtilise.tir.image[IMAGE1]);
    for(j=0;j<NOMBRE_TANK;j++)
        SDL_FreeSurface(tank[j].imageUtilise.tir.image[IMAGE1]);
    for(i=0;i<=IMAGE2;i++)
    {
        for(j=0;j<NOMBRE_SOUCOUPE;j++)
            SDL_FreeSurface(soucoupe[j].image[i].image);
    }

    for(i=0;i<nbCaserne || i<NOMBRE_MAX_CASERNE;i++)
    {
        for(j=0;j<=IMAGE2;j++)
        {
            SDL_FreeSurface(caserne[i].image[j].image);
        }

        for(j=0;j<=IMAGE5;j++)
        {
            SDL_FreeSurface(Otage[i].strucSprite.image[j].image);
        }
    }
    for(j=0;j<=IMAGE5;j++)
    {
        SDL_FreeSurface(Otage[nbCaserne].strucSprite.image[j].image);
    }
    //liberation de la mémoire alouer
    free(tank);
    free(avion);
    free(soucoupe);
    free(Otage);
    free(caserne);
    SDL_FreeSurface(base.image[IMAGE1].image);
    SDL_FreeSurface(bariere.image[IMAGE1].image);
    liberation_tilesets(&tilesetsMap);

}

void option(int* continuer,SDL_Event *even)
{
    SDL_PollEvent(even);

    switch(even->type)
    {
        case SDL_QUIT:
            //*continuer=0;
            break;

        case SDL_KEYDOWN:
                switch(even->key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        *continuer=0;
                        break;
                }
                break;
    }

}

void initilisationTemps(int tab[TAILLE_TABLEAU_TEMPS])
{
    int i=0;
    for(i=0;i<TAILLE_TABLEAU_TEMPS;i++)
    {
        tab[i]=0;
    }
}
