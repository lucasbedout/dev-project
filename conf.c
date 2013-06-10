/*
conf.c
----------

Fait par : Mehdi

Rôle : Permet de gerrer les options du jeu comme les dimentions, écran, cheat, etc...

Date de dernière modifiction : 09/06/2013

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "headers/erreur.h"
#include "headers/constantes.h"
#include "headers/menu.h"
#include "headers/conf.h"

int ecranConf(SDL_Surface *ecran,TTF_Font *police,imgMenu buton,imgMenu butonOn)
{
    SDL_Surface *texteCheatTTF=NULL,*texteEcranDimTTF=NULL,*texteOnTTF=NULL,*texteOffTTF=NULL,*texteDimEcTTF=NULL,*titreTTF=NULL;
    SDL_Surface *imgFond=NULL,*restartTTF=NULL,*exitTTF=NULL,*clearScreen=NULL,*fullScreenTTF=NULL,*texteYannTTF=NULL,*titreCheatTTF=NULL,*titreGeneralTTF=NULL;
    SDL_Color couleur={255,255,255};
    SDL_Rect positionText={0};
    SDL_Event even;
    conf config;
    int done=0,restart=0,ecranDim=1,invin=0,fullscreen=0,yann=0;
    char texteEcranDim[20]="800x600";

    //Lecture du fichier config et initialisation des variables de configuration
    config=lec_conf();
    ecranDim=(config.flagScreen&SCREEN_FULL) ? config.flagScreen-SCREEN_FULL : config.flagScreen;
    fullscreen=config.flagScreen&SCREEN_FULL;
    invin=config.flagCheat&CHEAT_INVINCIBLE;
    yann=config.flagCheat&CHEAT_YANN;
    //Fin de l'initialisation

    imgFond=IMG_Load(NOM_FICHIER_CLMT);
    if(imgFond==NULL)
        erreur_image(NOM_FICHIER_CLMT);

    //On va surligné le titre
    TTF_SetFontStyle(police,TTF_STYLE_UNDERLINE);
    titreTTF=TTF_RenderText_Blended(police,"Option du jeu",couleur);
    titreCheatTTF=TTF_RenderText_Blended(police,"Cheat",couleur);
    titreGeneralTTF=TTF_RenderText_Blended(police,"Parametres généraux",couleur);
    //Et on remet la police a la normal
    TTF_SetFontStyle(police,TTF_STYLE_NORMAL);
    texteCheatTTF=TTF_RenderText_Blended(police,"Invinsibilité :",couleur);
    texteOnTTF=TTF_RenderText_Blended(police,"ON",couleur);
    texteOffTTF=TTF_RenderText_Blended(police,"OFF",couleur);
    texteDimEcTTF=TTF_RenderText_Blended(police,"Dimention ecran :",couleur);
    restartTTF=TTF_RenderText_Blended(police,"Restart",couleur);
    exitTTF=TTF_RenderText_Blended(police,"Quitter",couleur);
    fullScreenTTF=TTF_RenderText_Blended(police,"Plein écran :",couleur);
    texteYannTTF=TTF_RenderText_Blended(police,"Yann is everywhere :",couleur);

    //On crée une surface noir qui fera office de "clear ecran"
    clearScreen=SDL_CreateRGBSurface(SDL_HWSURFACE,ecran->w,ecran->h,32,0,0,0,0);
    SDL_FillRect(clearScreen,NULL,SDL_MapRGB(ecran->format,0,0,0));

    done=!(verif_erreur());

    while(!done && !restart)
    {
        SDL_WaitEvent(&even);

        if(even.type==SDL_KEYDOWN && even.key.keysym.sym==SDLK_ESCAPE)
            done=1;

        positionText.x=0;
        positionText.y=0;
        SDL_BlitSurface(clearScreen,NULL,ecran,&positionText);

        positionText.x=ecran->w/2-imgFond->w/2;
        positionText.y=ecran->h/2-imgFond->h/2;
        SDL_BlitSurface(imgFond,NULL,ecran,&positionText);

        //Mise en place des titres
        positionText.x=ecran->w/2-titreTTF->w/2;
        positionText.y=0;
        SDL_BlitSurface(titreTTF,NULL,ecran,&positionText);

        positionText.x=ecran->w/2-titreGeneralTTF->w/2;
        positionText.y=ecran->h/4-fullScreenTTF->h;
        SDL_BlitSurface(titreGeneralTTF,NULL,ecran,&positionText);

        positionText.x=ecran->w/2-titreCheatTTF->w/2;
        positionText.y=ecran->h/2-texteCheatTTF->h;
        SDL_BlitSurface(titreCheatTTF,NULL,ecran,&positionText);

        //on blitte le texte et le bouton du cheat invincibilité
        positionText.x=ecran->w/4;
        positionText.y=ecran->h/2;
        SDL_BlitSurface(texteCheatTTF,NULL,ecran,&positionText);
        if(bouton(&buton,&butonOn,ecran->w/2,ecran->h/2,even, (invin) ? texteOnTTF : texteOffTTF))
            invin= !(invin);

        //On blitte le texte et le bouton des dimentions de l'écran
        strcpy(texteEcranDim,valScreen(ecranDim));
        positionText.x=ecran->w/4;
        positionText.y=ecran->h/3;
        SDL_BlitSurface(texteDimEcTTF,NULL,ecran,&positionText);
        texteEcranDimTTF = TTF_RenderText_Blended(police,texteEcranDim,couleur);
        if(bouton(&buton,&butonOn,ecran->w/2,ecran->h/3,even,texteEcranDimTTF))
            ecranDim=(ecranDim<SCREEN_AUTO) ? ecranDim*2 : 1;

        //on blitte le texte et le bouton pour le full screen
        positionText.x=ecran->w/4;
        positionText.y=ecran->h/4;
        SDL_BlitSurface(fullScreenTTF,NULL,ecran,&positionText);
        if(bouton(&buton,&butonOn,ecran->w/2,ecran->h/4,even, (fullscreen) ? texteOnTTF : texteOffTTF))
            fullscreen= !(fullscreen);

        //on blitte le texte et le bouton pour le cheat everywhere
        positionText.x=ecran->w/4;
        positionText.y=ecran->h/2+texteCheatTTF->h*2;
        SDL_BlitSurface(texteYannTTF,NULL,ecran,&positionText);
        if(bouton(&buton,&butonOn,ecran->w/2,ecran->h/2+texteCheatTTF->h*2,even, (yann) ? texteOnTTF : texteOffTTF))
            yann= !(yann);

        //On blitte le bouton restart et le bouton quitter
        if(bouton(&buton,&butonOn,ecran->w/4,ecran->h-buton.img->h*2,even,restartTTF))
            restart=1;
        if(bouton(&buton,&butonOn,ecran->w/2,ecran->h-buton.img->h*2,even,exitTTF))
            done=1;

        //On flip l'écran
        SDL_Flip(ecran);

        //on libére la mémoire alouer pour ensuite pourvoir realoué
        SDL_FreeSurface(texteEcranDimTTF);
    }

    SDL_FreeSurface(clearScreen);
    SDL_FreeSurface(imgFond);
    SDL_FreeSurface(texteCheatTTF);
    SDL_FreeSurface(texteOnTTF);
    SDL_FreeSurface(texteOffTTF);
    SDL_FreeSurface(texteDimEcTTF);
    SDL_FreeSurface(titreTTF);
    SDL_FreeSurface(titreCheatTTF);
    SDL_FreeSurface(titreGeneralTTF);
    SDL_FreeSurface(fullScreenTTF);
    SDL_FreeSurface(texteYannTTF);
    SDL_FreeSurface(restartTTF);
    SDL_FreeSurface(exitTTF);

    //Si un demande de restart est fait, on modifie le fichier config.
    if(restart)
    {
        config.flagScreen=ecranDim;
        config.flagScreen+=(fullscreen) ? SCREEN_FULL : 0;
        config.flagCheat=invin;
        config.flagCheat+=(yann) ? CHEAT_YANN  : 0;
        modif_conf(config);
    }

    return restart;
}

char* valScreen(int ecranDim)
{
    if(ecranDim&SCREEN_800x600)
        return "800x600";
    else if(ecranDim&SCREEN_1024x768)
        return "1024x768";
    else if(ecranDim&SCREEN_1280x800)
        return "1280x800";
    else if(ecranDim&SCREEN_1440x900)
        return "1440x900";
    else if(ecranDim&SCREEN_AUTO)
        return "auto";
    else
        return "Undefined size";
}

void verif_conf()
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen(FICHIER_CONFIG,"r");

    //si le fichier n'a pas été créer, on le crée
    if (fichier == NULL)
    {
        fichier=fopen(FICHIER_CONFIG,"w");

        fputs("Dimention=1\nCheat=0",fichier);
    }
    //on ferme le fichier
    fclose(fichier);
}

conf lec_conf()
{
    verif_conf();

    FILE* fichier=NULL;
    //ouverture du fichier
    fichier=fopen(FICHIER_CONFIG,"r");

    conf parametreConf;

    fscanf(fichier,"Dimention=%d\nCheat=%d",&parametreConf.flagScreen,&parametreConf.flagCheat);

    //on ferme le fichier
    fclose(fichier);

    return parametreConf;
}

void dimEcran(int *largeur,int *hauteur,int *flag)
{
    if(*flag&SCREEN_800x600){
        *hauteur=600;
        *largeur=800;
    }
    else if(*flag&SCREEN_1024x768){
        *hauteur=768;
        *largeur=1024;
    }
    else if(*flag&SCREEN_1280x800){
        *hauteur=800;
        *largeur=1280;
    }
    else if(*flag&SCREEN_1440x900){
        *hauteur=900;
        *largeur=1440;
    }
    else if(*flag&SCREEN_AUTO){
        const SDL_VideoInfo* myPointer = SDL_GetVideoInfo();
        *hauteur=myPointer->current_h;
        *largeur=myPointer->current_w;
    }
    else{
        *hauteur=600;
        *largeur=800;
        *flag=SCREEN_800x600;
    }
}

void modif_conf(conf config)
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen(FICHIER_CONFIG,"w+");

    fprintf(fichier,"Dimention=%d\nCheat=%d",config.flagScreen,config.flagCheat);

    fclose(fichier);
}
