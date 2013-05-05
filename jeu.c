/*
jeu.c
----------

Fait par : Mehdi

R�le : Permet de lancer le jeu et les fonction principal comme l'h�licopt�re, les �nnemies, la map, etc...

Date de derni�re modification : 26/03/2013

*/
#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'�diteur de lien
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
    //cr�ation d'un evennement
    SDL_Event even;

    //-------------------VARIABLE BASIQUE--------------------------
    //Sera utilis� pour une boucle ou double boucle
    int i=0,j=0;
    //--------------------------------------------------------------

    //--------------------CHARGEMENT DE LA MAP---------------------
    //map 2D contenant tout simplement des chiffres
    int **map=NULL;
    //positionMap permet de d�plac� la map pour faire un scrolling
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
    //Initilisation position de l'h�lico et chargement sprite
    sprite helico;
    iniPosHelicoptere(ecran,&helico);
    //---------------------------------------

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
    //On commence le point d'origine ( de la base ) des caserne apr�s la moitier de la fenetre
    for(i=0;i<nbCaserne || i<NOMBRE_MAX_CASERNE;i++)
    {
        iniCaserne(ecran,&caserne[i],map,&tilesetsMap,( (ecran->w/tilesetsMap.infoImage[0].image->w/2)+i*ECART_BASE_CASERNE+ECART_CASERNE_CASERNE));
        iniOtage(ecran,&(Otage[i]),map,&tilesetsMap,caserne[i].image[IMAGE1].position.x);
    }
    /*initialisation du sprite Otage pour la base et la file par d�faut a 0 car le joueur doit
    cherch� d'abord les otages pour ensuite les d�pos� et cr�eer une file*/
     iniOtage(ecran,&(Otage[nbCaserne]),map,&tilesetsMap,0);
     Otage[nbCaserne].file=0;
    //------------------------------------------

    //----------------DECLARATION TEMPS JEU-----------------------
    temps tempsJeu;
    initilisationTemps(tempsJeu.tempsPrecedent);
    //------------------------------------------------------------

    //----------------DECLARATION RESULTAT------------------------
    imgMenu victoire,defaite,explosion;
    iniResult(&victoire,&defaite,ecran);
    iniExplosion(&explosion,ecran);
    //------------------------------------------------------------

    //activation des touches r�p�tter
    SDL_EnableKeyRepeat(10,10);

    //-------------VARIABLE DU JEU GLOBAL-----------------------------
    int continuer=verif_erreur();

    //Variables concernant les otages et a la fois l'h�lico
    int nbOtageBord=0;
    int nbOtageBase=0;

    tempsJeu.tempsActuel=SDL_GetTicks(); //temps actuel du jeu
    //----------------------------------------------------------------

    while(continuer && helico.vie>0 && restOtage(Otage,nbCaserne,nbOtageBord) )
    {
        option(&continuer,&even);

        deplacementHelico(&helico,&even,&positionMap,&tilesetsMap,map);

        //verification des colision par rapport aux sprite
        gestionColisionSprite(&helico,bariere,tilesetsMap,&positionMap,even);
        for(i=0;i<nbCaserne;i++)
            gestionColisionSprite(&helico,caserne[i],tilesetsMap,&positionMap,even);

        //On blitte toute les surfaces et on rafraichie l'image
        affiche_map(map,ecran,tilesetsMap,positionMap);

        //-------------PARTIE TIR-------------
        //v�rifie si l'h�lico tir
        if(helico.imageUtilise.tir.actionEnCour==0)
        {
            //Si l'utilisateur tir, on calcullera la trajectoire
            calculTrajectoireTirHelico(&even,&helico,positionMap,&tilesetsMap);
        }
        //Si l'action est en cour, on blitte le tir enfonction de l'�quation
        else
        {
            tir(&helico,tilesetsMap);
            decallement_image_map_hauteurPixel(&helico,&tilesetsMap,helico.imageUtilise.tir.positionTir.y,helico.imageUtilise.tir.positionTir.x,positionMap,helico.imageUtilise.tir.image[IMAGE1]);
        }
        //-------------FIN PARTIE TIR----------------

        //Gestion des colisions tir
        GestionColision(&helico,map,&tilesetsMap,positionMap);
        //On blitte les animations de l'h�lico en fonction si l'h�lico est at�rie ou pas
        if(0==atterrissageHelico(&helico,map,&tilesetsMap,positionMap))
        {
            helico.imageUtilise.numeroImage=animationHelico(helico.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[1],ecran,&helico);
        }
        else
        {
            SDL_BlitSurface(helico.image[IMAGE5].image,NULL,ecran,&helico.image[IMAGE1].position);
        }

        //-------------animation ennemies---------
        for(i=0;i<NOMBRE_TANK && ((i*IMPORTANCE_TANK)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne)) ;i++)
        {
            if(tank[i].vie>0)
            {
                deplacementTank(&tank[i],positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[13+(i*TMP_DECAL_TAB_TANK)]));
                tank[i].imageUtilise.numeroImage=animationSprite(tank[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[12+(i*TMP_DECAL_TAB_TANK)],&tank[i],&tilesetsMap,positionMap);
                //v�rifie si le tank tir
                if(tank[i].imageUtilise.tir.actionEnCour==0)
                {
                    //Si l'utilisateur tir, on calcullera la trajectoire
                    calculTrajectoireTank(&tank[i],&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[14+(i*TMP_DECAL_TAB_TANK)]));
                }
                if(Gestion_Vie_sprite(&tank[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&tank[i],helico);
            }
            else
                respawn(&tank[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_TANK,VIE_TANK);

            //Si l'action est en cour, on blitte le tir enfonction de l'�quation
            if(tank[i].imageUtilise.tir.actionEnCour==1)
            {
                if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[16+(i*TMP_DECAL_TAB_TANK)]+1000/VITESSE_TIR_ENNEMIE) )
                {
                    tir(&tank[i],tilesetsMap);
                    tempsJeu.tempsPrecedent[16+(i*TMP_DECAL_TAB_TANK)]=tempsJeu.tempsActuel;
                }
                decallement_image_map_hauteurPixel(&tank[i],&tilesetsMap,tank[i].imageUtilise.tir.positionTir.y,tank[i].imageUtilise.tir.positionTir.x,positionMap,tank[i].imageUtilise.tir.image[IMAGE1]);
            }

            if(tank[i].imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&tank[i],explosion,tilesetsMap,positionMap);
        }

        for(i=0;i<NOMBRE_AVION && (((i+1)*IMPORTANCE_AVION)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(avion[i].vie>0)
            {
                deplacementAvion(&avion[i],&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[26+(i*TMP_DECAL_TAB_AVION)]));
                avion[i].imageUtilise.numeroImage=animationSprite(avion[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[25+(i*TMP_DECAL_TAB_AVION)],&avion[i],&tilesetsMap,positionMap);
                //v�rifie si l'avion tir
                if(avion[i].imageUtilise.tir.actionEnCour==0)
                {
                    //Si l'utilisateur tir, on calcullera la trajectoire
                    calculTrajectoireAvion(&avion[i],&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[27+(i*TMP_DECAL_TAB_AVION)]));
                }
                if(Gestion_Vie_sprite(&avion[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&avion[i],helico);
            }
            else
                respawn(&avion[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_AVION,VIE_AVION);

            //Si l'action est en cour, on blitte le tir enfonction de l'�quation
            if(avion[i].imageUtilise.tir.actionEnCour==1)
            {
                if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[28+(i*TMP_DECAL_TAB_AVION)]+1000/VITESSE_TIR_ENNEMIE) )
                {
                    tir(&avion[i],tilesetsMap);
                    tempsJeu.tempsPrecedent[28+(i*TMP_DECAL_TAB_AVION)]=tempsJeu.tempsActuel;
                }
                animationTir(&avion[i],&tilesetsMap,positionMap);
            }

            if(avion[i].imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&avion[i],explosion,tilesetsMap,positionMap);

        }

        for(i=0;i<NOMBRE_SOUCOUPE && (((i+1)*IMPORTANCE_SOUCOUPE)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(soucoupe[i].vie>0)
            {
                deplacementSoucoupe(&soucoupe[i],&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[30]));
                soucoupe[i].imageUtilise.numeroImage=animationSprite(soucoupe[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[29],&soucoupe[i],&tilesetsMap,positionMap);

                if(Gestion_Vie_sprite(&soucoupe[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&soucoupe[i],helico);
            }
            else
                respawn(&soucoupe[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_SOUCOUPE,VIE_SOUCOUPE);

            if(soucoupe[i].imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&soucoupe[i],explosion,tilesetsMap,positionMap);
        }
        //------------------------------

        //--------------CASERNE ET OTAGE------------
        for(i=0;i<nbCaserne;i++)
        {
            if(caserne[i].vie>0)
            {
                decallement_image_map_hauteurPixel(&caserne[i],&tilesetsMap,caserne[i].image[IMAGE1].position.y,caserne[i].image[IMAGE1].position.x,positionMap,caserne[i].image[IMAGE1].image);
                //Si la caserne se fait attaquer par un ennemie ou l'h�lico
                for(j=0;j<NOMBRE_TANK;j++)
                    {
                        if(Gestion_Vie_sprite(&caserne[i],&tank[j],&tilesetsMap,tempsJeu.tempsActuel))
                            declenchementExplosion(&explosion,&caserne[i],tank[j]);
                    }

                if(Gestion_Vie_sprite(&caserne[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&caserne[i],helico);
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
                    //Si l'otage se fait attaqu� par un ennemie ou l'h�lico
                    if(Gestion_Vie_sprite(&(Otage[i].strucSprite),&helico,&tilesetsMap,tempsJeu.tempsActuel))
                        declenchementExplosion(&explosion,&Otage[i].strucSprite,helico);

                    //Si un otage se fait �cras� et que l'h�lico n'�tait pas en train de d�coller
                    if(Helico_ecrase_otage(helico,Otage[i].strucSprite,map,tilesetsMap,positionMap))
                        Otage[i].strucSprite.vie=0;

                    for(j=0;j<NOMBRE_TANK;j++)
                    {
                        if(Gestion_Vie_sprite(&(Otage[i].strucSprite),&tank[j],&tilesetsMap,tempsJeu.tempsActuel))
                            declenchementExplosion(&explosion,&Otage[i].strucSprite,tank[i]);
                    }
                    gestionFileOtage(&(Otage[i]),&helico,caserne[i].image[IMAGE1].position.x,caserne[i].image[IMAGE1].position.y,map,&tilesetsMap,positionMap,&nbOtageBord);
                }
                if(Otage[i].strucSprite.imageUtilise.tir.nbExplosion>0)
                    afficheExplosion(&Otage[i].strucSprite,explosion,tilesetsMap,positionMap);
            }
            if(caserne[i].imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&caserne[i],explosion,tilesetsMap,positionMap);
        }
        //--------------------------------

        //--------------BASE ET OTAGE------------
        decallement_image_map_hauteurPixel(&base,&tilesetsMap,base.image[IMAGE1].position.y,base.image[IMAGE1].position.x,positionMap,base.image[IMAGE1].image);
        //On v�rifie si on otage veut d�ssendre et rejoindre la base
        hotage_dessend_helico(positionMap,&nbOtageBord,&nbOtageBase,&Otage[nbCaserne],&helico,&base,map,&tilesetsMap);
        if(Otage[nbCaserne].file==1)
        {
            deplacementOtageVersBase(&Otage[nbCaserne],&helico,&base,map,&tilesetsMap,positionMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[nbCaserne+nbCaserne]) );
            Otage[nbCaserne].strucSprite.imageUtilise.numeroImage=animationSprite(Otage[nbCaserne].strucSprite.imageUtilise.numeroImage,tempsJeu.tempsActuel,
                                                                                  tempsJeu.tempsPrecedent[nbCaserne+nbCaserne],&Otage[nbCaserne].strucSprite,&tilesetsMap,positionMap);
        }
        decallement_image_map_hauteurPixel(&bariere,&tilesetsMap,bariere.image[IMAGE1].position.y,bariere.image[IMAGE1].position.x,positionMap,bariere.image[IMAGE1].image);
        //---------------------------------------

        #if INVINCIBLE==0
        //On regarde enfin si l'h�licopt�re c'est fait toucher par un ennemie
        for(i=0;i<NOMBRE_TANK && ((i*IMPORTANCE_TANK)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(Gestion_Vie_helico(&helico,&tank[i],positionMap,tilesetsMap))
                declenchementExplosion(&explosion,&helico,tank[i]);
        }
        for(i=0;i<NOMBRE_AVION && (((i+1)*IMPORTANCE_AVION)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(Gestion_Vie_helico(&helico,&avion[i],positionMap,tilesetsMap))
                declenchementExplosion(&explosion,&helico,avion[i]);
        }
        for(i=0;i<NOMBRE_SOUCOUPE && (((i+1)*IMPORTANCE_SOUCOUPE)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
        {
            if(Gestion_Vie_helico(&helico,&soucoupe[i],positionMap,tilesetsMap))
                declenchementExplosion(&explosion,&helico,soucoupe[i]);
        }

        if(helico.imageUtilise.tir.nbExplosion>0)
            afficheExplosion(&helico,explosion,tilesetsMap,positionMap);
        #endif

        SDL_Flip(ecran);

        //Mise en place des FPS
        tempsJeu.tempsActuel=SDL_GetTicks();
        if((tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0])<(TEMPS_POSE))
        {
            SDL_Delay(TEMPS_POSE-(tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0]));
        }
    }

    //On affiche le r�sultat, victoire si l'h�lico est encore vivant, defaite si l'h�lico est d�truit
    if(helico.vie>0 && restOtage(Otage,nbCaserne,nbOtageBord)==0)
        SDL_BlitSurface(victoire.img,NULL,ecran,&victoire.positionImg);
    else if(helico.vie<=0)
        SDL_BlitSurface(defaite.img,NULL,ecran,&defaite.positionImg);

    if( (helico.vie>0 && restOtage(Otage,nbCaserne,nbOtageBord)==0) || helico.vie<=0 ){
    SDL_Flip(ecran);
    while(even.type!=SDL_KEYUP)
        SDL_WaitEvent(&even);
    }

    //on libere les images chargeren m�moire
    for(i=0;i<=IMAGE4;i++)
    {
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

        for(j=0;j<=IMAGE6;j++)
        {
            SDL_FreeSurface(Otage[i].strucSprite.image[j].image);
        }
    }
    for(j=0;j<=IMAGE6;j++)
    {
        SDL_FreeSurface(helico.image[j].image);
        SDL_FreeSurface(Otage[nbCaserne].strucSprite.image[j].image);
    }
    //liberation de la m�moire alouer
    free(tank);
    free(avion);
    free(soucoupe);
    free(Otage);
    free(caserne);
    SDL_FreeSurface(base.image[IMAGE1].image);
    SDL_FreeSurface(bariere.image[IMAGE1].image);
    liberation_tilesets(&tilesetsMap);

    //liberation des images lier a la victoire/defaite
    SDL_FreeSurface(victoire.img);
    SDL_FreeSurface(defaite.img);

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
                        menuPause(continuer,even);
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
