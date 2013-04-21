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
    sprite tank;
    iniTank(ecran,&tank);
    //test
    tank.image[IMAGE1].position.x=40;
    //------------------------------------------

    //---------VARIABLE AVION-------------------
    sprite avion;
    iniAvion(ecran,&avion);
    //test
    avion.image[IMAGE1].position.x=50;
    //------------------------------------------

    //---------VARIABLE AVION-------------------
    //------------------------------------------

    //---------VARIABLE BASE--------------------
    sprite base,bariere;
    iniBase(ecran,&base,map,&tilesetsMap);
    iniBariere(ecran,&bariere,map,&tilesetsMap);
    //------------------------------------------

    //---------VARIABLE OTAGE-------------------
    //le plus "+1" serat le sprite qui se dirigera vers la base
    otage Otage[NB_CASERNE+1];
    //------------------------------------------

    //---------VARIABLE CASERNE-----------------
    int nbCaserne=NB_CASERNE;
    sprite caserne[NB_CASERNE+1];
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
        if(tank.vie>0)
        {
            deplacementTank(&tank,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[13]));
            tank.imageUtilise.numeroImage=animationSprite(tank.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[12],&tank,&tilesetsMap,positionMap);
            //vérifie si le tank tir
            if(tank.imageUtilise.tir.actionEnCour==0)
            {
                //Si l'utilisateur tir, on calcullera la trajectoire
                calculTrajectoireTank(&tank,&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[14]));
            }
            //Si l'action est en cour, on blitte le tir enfonction de l'équation
            else
            {
                if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[16]+1000/VITESSE_TIR_ENNEMIE) )
                {
                    tir(&tank);
                    tempsJeu.tempsPrecedent[16]=tempsJeu.tempsActuel;
                }
                decallement_image_map_hauteurPixel(&tank,&tilesetsMap,tank.imageUtilise.tir.positionTir.y,tank.imageUtilise.tir.positionTir.x,positionMap,tank.imageUtilise.tir.image[IMAGE1]);
            }
            Gestion_Vie_sprite(&tank,&helico,&tilesetsMap);
        }

        if(avion.vie>0)
        {
            deplacementAvion(&avion,&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[26]));
            avion.imageUtilise.numeroImage=animationSprite(avion.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[25],&avion,&tilesetsMap,positionMap);
            //vérifie si le tank tir
            if(avion.imageUtilise.tir.actionEnCour==0)
            {
                //Si l'utilisateur tir, on calcullera la trajectoire
                calculTrajectoireAvion(&avion,&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[27]));
            }
            //Si l'action est en cour, on blitte le tir enfonction de l'équation
            else
            {
                if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[28]+1000/VITESSE_TIR_ENNEMIE) )
                {
                    tir(&avion);
                    tempsJeu.tempsPrecedent[28]=tempsJeu.tempsActuel;
                }
                //decallement_image_map_hauteurPixel(&avion,&tilesetsMap,avion.imageUtilise.tir.positionTir.y,avion.imageUtilise.tir.positionTir.x,positionMap,avion.imageUtilise.tir.image[IMAGE1]);
                animationTir(&avion,&tilesetsMap,positionMap);
            }
            Gestion_Vie_sprite(&avion,&helico,&tilesetsMap);
        }
        //------------------------------

        //--------------CASERNE ET OTAGE------------
        for(i=0;i<nbCaserne;i++)
        {
            if(caserne[i].vie>0)
            {
                decallement_image_map_hauteurPixel(&caserne[i],&tilesetsMap,caserne[i].image[IMAGE1].position.y,caserne[i].image[IMAGE1].position.x,positionMap,caserne[i].image[IMAGE1].image);
                //Si la caserne se fait attaquer par un ennemie ou l'hélico
                Gestion_Vie_sprite(&caserne[i],&tank,&tilesetsMap);
                Gestion_Vie_sprite(&caserne[i],&helico,&tilesetsMap);
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
                    Gestion_Vie_sprite(&(Otage[i].strucSprite),&helico,&tilesetsMap);
                    Gestion_Vie_sprite(&(Otage[i].strucSprite),&tank,&tilesetsMap);
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
            Otage[nbCaserne].strucSprite.imageUtilise.numeroImage=animationSprite(Otage[nbCaserne].strucSprite.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[nbCaserne+nbCaserne],&Otage[nbCaserne].strucSprite,&tilesetsMap,positionMap);
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
        SDL_FreeSurface(tank.image[i].image);
        SDL_FreeSurface(avion.image[i].image);
        SDL_FreeSurface(avion.imageUtilise.tir.image[i]);
    }
    SDL_FreeSurface(helico.imageUtilise.tir.image[IMAGE1]);
    SDL_FreeSurface(tank.imageUtilise.tir.image[IMAGE1]);

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
            *continuer=0;
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
