/*
menu.c
----------

Fait par : Mehdi

Rôle : Gere la gestion du menu

Date de dernière modifiction : 01/05/2013

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/menu.h"
#include "headers/erreur.h"

void iniResult(imgMenu *vic,imgMenu *def,SDL_Surface *ecran)
{
    vic->img=NULL;
    def->img=NULL;

    //On charge l'image et on regarde si il y a une erreur
    vic->img=IMG_Load(NOM_FICHIER_VICTOIRE);
    if(vic->img==NULL)
        erreur_image(NOM_FICHIER_VICTOIRE);

    def->img=IMG_Load(NOM_FICHIER_DEFAITE);
    if(def->img==NULL)
        erreur_image(NOM_FICHIER_DEFAITE);

    //On attribu les propriété écran
    vic->ecran=ecran;
    def->ecran=ecran;

    //On centre l'image
    vic->positionImg.x=(ecran->w-vic->img->w)/2;
    vic->positionImg.y=(ecran->h-vic->img->h)/2;

    def->positionImg.x=(ecran->w-def->img->w)/2;
    def->positionImg.y=(ecran->h-def->img->h)/2;
}

void menuPause(int* continuer,SDL_Event *even)
{
    //Initialisation du type d'évennement
    even->type=NULL;

    //Si une touche clavier est enfoncé, alors on stop la pause
    while(even->type!=SDL_KEYDOWN || even->key.keysym.sym==SDLK_ESCAPE)
        SDL_WaitEvent(even);
    //Si la touche H est enfoncé, alors on quite le jeu
    if(even->key.keysym.sym==SDLK_SPACE)
        *continuer=0;
}

int bouton(imgMenu *buton,imgMenu *butonSouriOn,int cordonneeX,int cordonneeY,SDL_Event even)
{
   buton->positionImg.x=cordonneeX;
   buton->positionImg.y=cordonneeY;

   if(butonSouriOn!=NULL)
   {
       butonSouriOn->positionImg.x=cordonneeX;
       butonSouriOn->positionImg.y=cordonneeY;
   }


    if( ( (even.motion.x>=cordonneeX) &&  (even.motion.x<=(cordonneeX+buton->img->w) ) ) &&
        ( (even.motion.y>=cordonneeY) && (even.motion.y<=(cordonneeY+buton->img->h)) ) &&
        even.type==SDL_MOUSEMOTION && butonSouriOn!=NULL)
        SDL_BlitSurface(butonSouriOn->img, NULL, butonSouriOn->ecran,&(butonSouriOn->positionImg));
    else
        SDL_BlitSurface(buton->img, NULL, buton->ecran,&(buton->positionImg));

   if(even.type==SDL_MOUSEBUTTONDOWN && even.button.button==SDL_BUTTON_LEFT)
   {
       if( even.button.x>cordonneeX && even.button.x<(cordonneeX+buton->img->w) &&
          even.button.y>cordonneeY && even.button.y<(cordonneeY+buton->img->h) )
            return 1;
       else
            return 0;
   }
   else
        return 0;

}
