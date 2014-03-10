/*
menu.c
----------

Fait par : Mehdi

Rôle : Gere la gestion du menu

Date de dernière modifiction : 01/05/2013

*/

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

void menuPause(int* continuer,SDL_Event *even,TTF_Font *police,SDL_Surface *ecran,systEfSound *sound)
{
    //initialisation variable
    int reprendre=0,quitter=0;

    //initialisation des surfaces
    SDL_Surface *texteReprendreTTF=NULL,*texteQuitterTTF=NULL,*screen=NULL,*titreTTF=NULL;
    SDL_Rect posiText={0};
    imgMenu buton,butonOn;
    iniBouton(&buton,&butonOn,ecran);

    //remplissage
    SDL_Color couleur={255,255,255};
    texteReprendreTTF=TTF_RenderText_Blended(police,"Reprendre",couleur);
    texteQuitterTTF=TTF_RenderText_Blended(police,"Quitter",couleur);
    titreTTF=TTF_RenderText_Blended(police,"PAUSE",couleur);
    //mise en place de la transparence pour le screen
    screen=SDL_CreateRGBSurface(SDL_HWSURFACE,ecran->w,ecran->h,32,0,0,0,0);
    SDL_FillRect(screen,NULL,SDL_MapRGB(ecran->format,0,0,0));
    SDL_SetAlpha(screen,SDL_SRCALPHA,128);

    //Initialisation du type d'évennement
    even->type=NULL;

    //mise en place  du fond
    SDL_BlitSurface(screen,NULL,ecran,&posiText);

    posiText.x=ecran->w/2-titreTTF->w/2;
    posiText.y=ecran->h/4;


    do{
        SDL_BlitSurface(titreTTF,NULL,ecran,&posiText);

        reprendre=bouton(&buton,&butonOn,ecran->w/4,ecran->h/2,*even,texteReprendreTTF);
        quitter=bouton(&buton,&butonOn,ecran->w/2,ecran->h/2,*even,texteQuitterTTF);

        if(reprendre || quitter)
            Mix_PlayChannel( -1, sound->button, 0 );

        SDL_Flip(ecran);

        SDL_WaitEvent(even);
    }while( (even->type!=SDL_KEYDOWN || even->key.keysym.sym==SDLK_ESCAPE) && (!reprendre && !quitter) );
    //Si une touche clavier est enfoncé, alors on stop la pause

    //Si la touche espace est enfoncé, alors on quite le jeu
    if(even->key.keysym.sym==SDLK_SPACE || quitter )
        *continuer=0;

    SDL_FreeSurface(texteReprendreTTF);
    SDL_FreeSurface(texteQuitterTTF);
    SDL_FreeSurface(titreTTF);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(buton.img);
    SDL_FreeSurface(butonOn.img);
}

int bouton(imgMenu *buton,imgMenu *butonSouriOn,int cordonneeX,int cordonneeY,SDL_Event even,SDL_Surface *texte)
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

    //on blit le texte
    if(texte!=NULL)
    {
        SDL_Rect posiText;
        posiText.x=buton->img->w/2-texte->w/2+buton->positionImg.x;
        posiText.y=buton->img->h/2-texte->h/2+buton->positionImg.y;
        SDL_BlitSurface(texte, NULL, buton->ecran,&posiText);
    }

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
