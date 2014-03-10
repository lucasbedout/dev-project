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
#include "headers/classement.h"

void jeu (SDL_Surface *ecran,TTF_Font *police,conf config,systEfSound *sound)
{
    //création d'un evennement
    SDL_Event even;

    //-------------------VARIABLE BASIQUE--------------------------
    //Sera utilisé pour une boucle ou double boucle
    int i=0,j=0;
    //--------------------------------------------------------------

    //---------VARIABLE AUDIO----------------
    //Initilisation les variables audio
    Mix_Chunk *avionEffect=NULL,*soucoupeEffect=NULL,*tankEffect=NULL,*helicoEffect=NULL,*explosionEffect=NULL,*shootEffect=NULL;
    iniSoundJeu(config,&avionEffect,&soucoupeEffect,&tankEffect,&helicoEffect,&explosionEffect,&shootEffect);
    activSoundJeu(config,&avionEffect,&soucoupeEffect,&tankEffect,&helicoEffect);
    //---------------------------------------

    //-------------------VARIABLE TTF--------------------------
    SDL_Surface *texteVieTTF=NULL,*texteOtageBordTTF=NULL,*texteOtageSauveTTF=NULL,*texteScoreTTF=0;
    SDL_Color couleur={255,255,255};
    SDL_Rect positionText={0};
    char texteVie[10]="3/3 vie",texteOtageBord[20]="0/16 otages a bord",texteOtageSauve[20]="0 otages sauvé",texteScore[20]="Score : 0";

    texteVieTTF=TTF_RenderText_Blended(police,texteVie,couleur);
    texteOtageBordTTF=TTF_RenderText_Blended(police,texteOtageBord,couleur);
    texteOtageSauveTTF=TTF_RenderText_Blended(police,texteOtageSauve,couleur);
    texteScoreTTF=TTF_RenderText_Blended(police,texteScore,couleur);
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

    //---------VARIABLE TANK--------------------
    sprite *tank;
    tank=malloc(sizeof(sprite)*NOMBRE_TANK);
    for(i=0;i<NOMBRE_TANK;i++)
    {
        iniTank(ecran,&tank[i],config.level);
        tank[i].image[IMAGE1].position.x=spawnAlea(&tank[i],&tilesetsMap,positionMap);
    }
    //------------------------------------------

    //---------VARIABLE AVION-------------------
    sprite *avion;
    avion=malloc(sizeof(sprite)*NOMBRE_AVION);
    for(i=0;i<NOMBRE_AVION;i++)
    {
        iniAvion(ecran,&avion[i],config.level);
        avion[i].image[IMAGE1].position.x=spawnAlea(&avion[i],&tilesetsMap,positionMap);
    }
    //------------------------------------------

    //---------VARIABLE SOUCOUPE-------------------
    sprite *soucoupe;
    soucoupe=malloc(sizeof(sprite)*NOMBRE_SOUCOUPE);
    for(i=0;i<NOMBRE_SOUCOUPE;i++)
    {
        iniSoucoupe(ecran,&soucoupe[i],config.level);
        soucoupe[i].image[IMAGE1].position.x=spawnAlea(&soucoupe[i],&tilesetsMap,positionMap);
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

    //----------------DECLARATION RESULTAT------------------------
    imgMenu victoire,defaite,explosion;
    iniResult(&victoire,&defaite,ecran);
    iniExplosion(&explosion,ecran);
    //------------------------------------------------------------

    //----------------VARIABLE CHEAT------------------------------
    int invincible=config.flagCheat&CHEAT_INVINCIBLE;
    //------------------------------------------------------------

    //activation des touches répétter
    SDL_EnableKeyRepeat(10,10);

    //-------------VARIABLE DU JEU GLOBAL-----------------------------
    int continuer=verif_erreur();

    //Variables concernant les otages et a la fois l'hélico
    int nbOtageBord=0;
    int nbOtageBase=0;

    //Variable score du joueur
    int score=0;

    tempsJeu.tempsActuel=SDL_GetTicks(); //temps actuel du jeu
    tempsJeu.tempsDebut=SDL_GetTicks(); //temps du debut du jeu
    //----------------------------------------------------------------

    while(continuer && helico.vie>0 && restOtage(Otage,nbCaserne,nbOtageBord) )
    {
        option(&continuer,&even,police,ecran,sound,config,&helicoEffect);

        deplacementHelico(&helico,&even,&positionMap,&tilesetsMap,map);

        //verification des colision par rapport aux sprite
        gestionColisionSprite(&helico,bariere,tilesetsMap,&positionMap,even);
        for(i=0;i<nbCaserne;i++)
            gestionColisionSprite(&helico,caserne[i],tilesetsMap,&positionMap,even);

        //On blitte toute les surfaces et on rafraichie l'image
        affiche_map(map,ecran,tilesetsMap,positionMap);

        //On affiche le texte
        sprintf(texteVie,"%d /%d vie",helico.vie,VIE_HELICO);
        positionText.x=0;
        positionText.y=0;
        texteVieTTF=TTF_RenderText_Blended(police,texteVie,couleur);
        SDL_BlitSurface(texteVieTTF,NULL,ecran,&positionText);

        sprintf(texteOtageBord,"%d /%d otage a bord",nbOtageBord,NB_OTAGE_PAR_CASERNE);
        positionText.x=ecran->w/3;
        texteOtageBordTTF=TTF_RenderText_Blended(police,texteOtageBord,couleur);
        SDL_BlitSurface(texteOtageBordTTF,NULL,ecran,&positionText);

        sprintf(texteOtageSauve,"%d otages sauvés",nbOtageBase);
        positionText.x=ecran->w-texteOtageSauveTTF->w;
        texteOtageSauveTTF=TTF_RenderText_Blended(police,texteOtageSauve,couleur);
        SDL_BlitSurface(texteOtageSauveTTF,NULL,ecran,&positionText);

        //on calcul le score
        score=nbOtageBord*OTAGE_BORD_COEF+nbOtageBase*OTAGE_BASE_COEF;
        sprintf(texteScore,"Score : %d",score);
        texteScoreTTF=TTF_RenderText_Blended(police,texteScore,couleur);
        positionText.x=(ecran->w/2-texteScoreTTF->w/2);
        positionText.y=texteVieTTF->h*2;
        SDL_BlitSurface(texteScoreTTF,NULL,ecran,&positionText);

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
            tir(&helico,tilesetsMap,&shootEffect);
            decallement_image_map_hauteurPixel(&helico,&tilesetsMap,helico.imageUtilise.tir.positionTir.y,helico.imageUtilise.tir.positionTir.x,positionMap,helico.imageUtilise.tir.image[IMAGE1]);
        }
        //-------------FIN PARTIE TIR----------------

        //Gestion des colisions tir
        GestionColision(&helico,map,&tilesetsMap,positionMap);
        //On blitte les animations de l'hélico en fonction si l'hélico est atérie ou pas
        if(0==atterrissageHelico(&helico,map,&tilesetsMap,positionMap))
        {
            helico.imageUtilise.numeroImage=animationHelico(helico.imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[1],ecran,&helico,&helicoEffect);
        }
        else
        {
            SDL_BlitSurface(helico.image[IMAGE5].image,NULL,ecran,&helico.image[IMAGE1].position);
        }

        //-------------animation ennemies---------
        for(i=0;i<NOMBRE_TANK && ((i*IMPORTANCE_TANK)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne)) && config.level;i++)
        {
            if(tank[i].vie>0)
            {
                deplacementTank(&tank[i],positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[13+(i*TMP_DECAL_TAB_TANK)]));
                tank[i].imageUtilise.numeroImage=animationSprite(tank[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[12+(i*TMP_DECAL_TAB_TANK)],&tank[i],&tilesetsMap,positionMap);
                jaugeVie(tank[i],(config.level==LEVEL_LEGENDE) ? VIE_TANK*2 : VIE_TANK,tank[i].vie,tilesetsMap.infoImage[0].image->w,positionMap);
                //vérifie si le tank tir
                if(tank[i].imageUtilise.tir.actionEnCour==0)
                {
                    //Si l'utilisateur tir, on calcullera la trajectoire
                    calculTrajectoireTank(&tank[i],&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[14+(i*TMP_DECAL_TAB_TANK)]));
                }
                if(Gestion_Vie_sprite(&tank[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&tank[i],helico,&explosionEffect);
            }
            else
                respawn(&tank[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_TANK,(config.level==LEVEL_LEGENDE) ? VIE_TANK*2 : VIE_TANK,config.level,positionMap);

            //Si l'action est en cour, on blitte le tir enfonction de l'équation
            if(tank[i].imageUtilise.tir.actionEnCour==1)
            {
                if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[16+(i*TMP_DECAL_TAB_TANK)]+1000/VITESSE_TIR_ENNEMIE) )
                {
                    tir(&tank[i],tilesetsMap,&shootEffect);
                    tempsJeu.tempsPrecedent[16+(i*TMP_DECAL_TAB_TANK)]=tempsJeu.tempsActuel;
                }
                decallement_image_map_hauteurPixel(&tank[i],&tilesetsMap,tank[i].imageUtilise.tir.positionTir.y,tank[i].imageUtilise.tir.positionTir.x,positionMap,tank[i].imageUtilise.tir.image[IMAGE1]);
            }

            if(tank[i].imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&tank[i],explosion,tilesetsMap,positionMap);
        }

        for(i=0;i<NOMBRE_AVION && (((i+1)*IMPORTANCE_AVION)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne)) && config.level;i++)
        {
            if(avion[i].vie>0)
            {
                deplacementAvion(&avion[i],&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[26+(i*TMP_DECAL_TAB_AVION)]));
                avion[i].imageUtilise.numeroImage=animationSprite(avion[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[25+(i*TMP_DECAL_TAB_AVION)],&avion[i],&tilesetsMap,positionMap);
                jaugeVie(avion[i],(config.level==LEVEL_LEGENDE) ? VIE_AVION*2 : VIE_AVION,avion[i].vie,tilesetsMap.infoImage[0].image->w,positionMap);
                //vérifie si l'avion tir
                if(avion[i].imageUtilise.tir.actionEnCour==0)
                {
                    //Si l'utilisateur tir, on calcullera la trajectoire
                    calculTrajectoireAvion(&avion[i],&helico,positionMap,&tilesetsMap,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[27+(i*TMP_DECAL_TAB_AVION)]),&shootEffect);
                }
                if(Gestion_Vie_sprite(&avion[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&avion[i],helico,&explosionEffect);
            }
            else
                respawn(&avion[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_AVION,(config.level==LEVEL_LEGENDE) ? VIE_AVION*2 : VIE_AVION,config.level,positionMap);

            //Si l'action est en cour, on blitte le tir enfonction de l'équation
            if(avion[i].imageUtilise.tir.actionEnCour==1)
            {
                if(tempsJeu.tempsActuel>(tempsJeu.tempsPrecedent[28+(i*TMP_DECAL_TAB_AVION)]+1000/VITESSE_TIR_ENNEMIE) )
                {
                    tir(&avion[i],tilesetsMap,&shootEffect);
                    tempsJeu.tempsPrecedent[28+(i*TMP_DECAL_TAB_AVION)]=tempsJeu.tempsActuel;
                }
                animationTir(&avion[i],&tilesetsMap,positionMap);
            }

            if(avion[i].imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&avion[i],explosion,tilesetsMap,positionMap);

        }

        for(i=0;i<NOMBRE_SOUCOUPE && (((i+1)*IMPORTANCE_SOUCOUPE)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne)) && config.level;i++)
        {
            if(soucoupe[i].vie>0)
            {
                deplacementSoucoupe(&soucoupe[i],&helico,positionMap,&tilesetsMap,map,tempsJeu.tempsActuel,&(tempsJeu.tempsPrecedent[30]));
                soucoupe[i].imageUtilise.numeroImage=animationSprite(soucoupe[i].imageUtilise.numeroImage,tempsJeu.tempsActuel,tempsJeu.tempsPrecedent[29],&soucoupe[i],&tilesetsMap,positionMap);
                jaugeVie(soucoupe[i],(config.level==LEVEL_LEGENDE) ? VIE_SOUCOUPE*2 : VIE_SOUCOUPE,soucoupe[i].vie,tilesetsMap.infoImage[0].image->w,positionMap);

                if(Gestion_Vie_sprite(&soucoupe[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&soucoupe[i],helico,&explosionEffect);
            }
            else
                respawn(&soucoupe[i],&tilesetsMap,tempsJeu.tempsActuel,RESPAWN_SOUCOUPE,(config.level==LEVEL_LEGENDE) ? VIE_SOUCOUPE*2 : VIE_SOUCOUPE,config.level,positionMap);

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
                //Si la caserne se fait attaquer par un ennemie ou l'hélico
                for(j=0;j<NOMBRE_TANK;j++)
                    {
                        if(Gestion_Vie_sprite(&caserne[i],&tank[j],&tilesetsMap,tempsJeu.tempsActuel))
                            declenchementExplosion(&explosion,&caserne[i],tank[j],&explosionEffect);
                    }

                if(Gestion_Vie_sprite(&caserne[i],&helico,&tilesetsMap,tempsJeu.tempsActuel))
                    declenchementExplosion(&explosion,&caserne[i],helico,&explosionEffect);
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
                    if(Gestion_Vie_sprite(&(Otage[i].strucSprite),&helico,&tilesetsMap,tempsJeu.tempsActuel))
                        declenchementExplosion(&explosion,&Otage[i].strucSprite,helico,&explosionEffect);

                    //Si un otage se fait écrasé et que l'hélico n'était pas en train de décoller
                    if(Helico_ecrase_otage(helico,Otage[i].strucSprite,map,tilesetsMap,positionMap))
                        Otage[i].strucSprite.vie=0;

                    for(j=0;j<NOMBRE_TANK;j++)
                    {
                        if(Gestion_Vie_sprite(&(Otage[i].strucSprite),&tank[j],&tilesetsMap,tempsJeu.tempsActuel))
                            declenchementExplosion(&explosion,&Otage[i].strucSprite,tank[i],&explosionEffect);
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

        //Si l'hélico n'est pas invincible
        if(!invincible){
            //On regarde enfin si l'hélicoptère c'est fait toucher par un ennemie
            for(i=0;i<NOMBRE_TANK && ((i*IMPORTANCE_TANK)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
            {
                if(Gestion_Vie_helico(&helico,&tank[i],positionMap,tilesetsMap,helico.imageUtilise.numeroImage))
                    declenchementExplosion(&explosion,&helico,tank[i],&explosionEffect);
            }
            for(i=0;i<NOMBRE_AVION && (((i+1)*IMPORTANCE_AVION)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
            {
                if(Gestion_Vie_helico(&helico,&avion[i],positionMap,tilesetsMap,helico.imageUtilise.numeroImage))
                    declenchementExplosion(&explosion,&helico,avion[i],&explosionEffect);
            }
            for(i=0;i<NOMBRE_SOUCOUPE && (((i+1)*IMPORTANCE_SOUCOUPE)<=pourcentSavOtage(nbOtageBord,nbOtageBase,nbCaserne));i++)
            {
                if(gestion_colision_helico(&helico,&soucoupe[i],positionMap,tilesetsMap))
                    declenchementExplosion(&explosion,&helico,soucoupe[i],&explosionEffect);
            }

            if(helico.imageUtilise.tir.nbExplosion>0)
                afficheExplosion(&helico,explosion,tilesetsMap,positionMap);
        }

        SDL_Flip(ecran);

        //liberation du texte qui seront ensuite réatribué
        SDL_FreeSurface(texteOtageBordTTF);
        SDL_FreeSurface(texteVieTTF);
        SDL_FreeSurface(texteOtageSauveTTF);
        SDL_FreeSurface(texteScoreTTF);

        //Mise en place des FPS
        tempsJeu.tempsActuel=SDL_GetTicks();
        if((tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0])<(TEMPS_POSE))
        {
            SDL_Delay(TEMPS_POSE-(tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0]));
        }
    }

    //On stop la répétition des touches
    SDL_EnableKeyRepeat(0,0);

    //stop son
    stopSoundJeu(config,&helicoEffect);

    //création de la surface qui va faire office de fond transparent
    SDL_Surface *screen;
    SDL_Rect posiScreen={0};
    screen=SDL_CreateRGBSurface(SDL_HWSURFACE,ecran->w,ecran->h,32,0,0,0,0);
    SDL_FillRect(screen,NULL,SDL_MapRGB(ecran->format,0,0,0));
    SDL_SetAlpha(screen,SDL_SRCALPHA,128);
    SDL_BlitSurface(screen,NULL,ecran,&posiScreen);
    SDL_FreeSurface(screen);
    //Fin de la surface qui fait office de fond transparent

    //On affiche le résultat, victoire si l'hélico est encore vivant, defaite si l'hélico est détruit
    if(helico.vie>0 && restOtage(Otage,nbCaserne,nbOtageBord)==0)
    {
        SDL_BlitSurface(victoire.img,NULL,ecran,&victoire.positionImg);
        //On fonction du temps on ajoute des points au score. Plus le temps est court et plus il y a de point ajouter
        score+=(1/(tempsJeu.tempsActuel-tempsJeu.tempsDebut))*COEF_TMP_VIC+helico.vie*COEF_VIE_VIC+COEF_DIFFICULT*config.level;
    }
    else if(helico.vie<=0)
        SDL_BlitSurface(defaite.img,NULL,ecran,&defaite.positionImg);

    if( (helico.vie>0 && restOtage(Otage,nbCaserne,nbOtageBord)==0) || helico.vie<=0 ){
        SDL_Flip(ecran);
        //pause de 1 seconde du programme pour que le joueur voit qu'il a gagner ou perdu avant de continuer
        SDL_Delay(1000);
        //on initialise le type d'evennement puis on attends que le joeur appuis sur une touche du clavier pour continuer
        even.type=NULL;
        while(even.type!=SDL_KEYUP)
            SDL_WaitEvent(&even);
    }

    //On verifie si un new score a été produit
    if(newScoreClmt(score))
    {
        if(config.flagSound&FLAG_EFFET)
            Mix_PlayChannel( -1, sound->aplause, 0 );
        char name[20]="";
        sprintf(name,"%s",enterName(score,police,ecran));
        insertNewScore(name,score);
        affClmt(ecran,police,config.flagCheat);
    }

    //on libere les images charger en mémoire
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
    //liberation de la mémoire alouer
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

    //liberation des sons
    libSoundJeu(config,&avionEffect,&soucoupeEffect,&tankEffect,&helicoEffect,&explosionEffect,&shootEffect);

}

void option(int* continuer,SDL_Event *even,TTF_Font *police,SDL_Surface *ecran,systEfSound *sound,conf config,Mix_Chunk **helicoSound)
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
                        stopSoundJeu(config,helicoSound);
                        if(config.flagSound&FLAG_EFFET)
                            Mix_PlayChannel( -1, sound->touche, 0 );
                        menuPause(continuer,even,police,ecran,sound);
                        repriseSoundJeu(config,helicoSound);
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

void iniSoundJeu(conf config,Mix_Chunk **avionEffect,Mix_Chunk **soucoupeEffect,Mix_Chunk **tankEffect,Mix_Chunk **helicoEffect,Mix_Chunk **explosionEffect,Mix_Chunk **shootEffect)
{
    if(config.flagSound&FLAG_EFFET)
    {
        *avionEffect = Mix_LoadWAV(EFFECT_AVION);
        if(*avionEffect==NULL)
            erreur_indef(EFFECT_AVION);

        *soucoupeEffect = Mix_LoadWAV(EFFECT_SOUCOUPE);
        if(*soucoupeEffect==NULL)
            erreur_indef(EFFECT_SOUCOUPE);

        *tankEffect = Mix_LoadWAV(EFFECT_TANK);
        if(*tankEffect==NULL)
            erreur_indef(EFFECT_TANK);

        *helicoEffect = Mix_LoadWAV(EFFECT_HELICO);
        if(*helicoEffect==NULL)
            erreur_indef(EFFECT_HELICO);

        *shootEffect = Mix_LoadWAV(EFFECT_SHOOT);
        if(*shootEffect==NULL)
            erreur_indef(EFFECT_SHOOT);
        else
            Mix_VolumeChunk(*shootEffect, MIX_MAX_VOLUME/2);

        *explosionEffect = Mix_LoadWAV(EFFECT_EXPLOSION);
        if(*explosionEffect==NULL)
            erreur_indef(EFFECT_EXPLOSION);
        else
            Mix_VolumeChunk(*explosionEffect, MIX_MAX_VOLUME/2);
    }
}

void libSoundJeu(conf config,Mix_Chunk **avionEffect,Mix_Chunk **soucoupeEffect,Mix_Chunk **tankEffect,Mix_Chunk **helicoEffect,Mix_Chunk **explosionEffect,Mix_Chunk **shootEffect)
{
    if(config.flagSound&FLAG_EFFET)
    {
        Mix_FreeChunk( *avionEffect );
        Mix_FreeChunk( *soucoupeEffect );
        Mix_FreeChunk( *tankEffect );
        Mix_FreeChunk( *helicoEffect );
        Mix_FreeChunk( *explosionEffect );
        Mix_FreeChunk( *shootEffect );
    }
}

void activSoundJeu(conf config,Mix_Chunk **avionEffect,Mix_Chunk **soucoupeEffect,Mix_Chunk **tankEffect,Mix_Chunk **helicoEffect)
{
    if(config.flagSound&FLAG_EFFET)
    {
        //on déclenche le son du tir
        Mix_PlayChannel(0, *helicoEffect, -1);
        Mix_PlayChannel(1, *soucoupeEffect, -1);
        Mix_PlayChannel(2, *avionEffect, -1);
        Mix_PlayChannel(3, *tankEffect, -1);

        //réglage du son
        Mix_VolumeChunk(*helicoEffect, MIX_MAX_VOLUME/2);
        Mix_VolumeChunk(*soucoupeEffect, 0);
        Mix_VolumeChunk(*avionEffect, 0);
        Mix_VolumeChunk(*tankEffect, 0);
    }
}

void stopSoundJeu(conf config,Mix_Chunk **sound)
{
    if(config.flagSound&FLAG_EFFET)
        Mix_VolumeChunk(*sound, 0);
}

void repriseSoundJeu(conf config,Mix_Chunk **sound)
{
    if(config.flagSound&FLAG_EFFET)
        Mix_VolumeChunk(*sound, MIX_MAX_VOLUME/2);
}
