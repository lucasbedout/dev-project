/*
main.c
initialise les composants du jeu (fenêtres, variables etc...)

*/
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "headers/erreur.h"
#include "headers/constantes.h"
#include "headers/main.h"
#include "headers/menu.h"
#include "headers/conf.h"
#include "headers/jeu.h"
#include "headers/classement.h"
#include "headers/editeur.h"

int main (int argc, char** argv)
{
    supr_fichier_erreur();

	int continuer = 1,hauteur=0,largeur=0,restart=0;
	conf config;
    SDL_Surface *ecran, *Menu,*clearScreen;
    SDL_Rect positionMenu,positionClear;

    do{
        //initialisation du restart
        restart=0;

        //On li le fichier config et on remplie la structure config
        config=lec_conf();

        // initialisation de la SDL VIDEO
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
        {
            printf("Impossible d'initialiser la librairie vidéo : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        //on recuperre les parametre de la fenetre en fonction du fichier de configuration
        dimEcran(&largeur,&hauteur,&config.flagScreen);

        // création de l'écran
        ecran = SDL_SetVideoMode(largeur, hauteur, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|((config.flagScreen&SCREEN_FULL) ? SDL_FULLSCREEN : 0));
        //(config.flagScreen&SCREEN_FULL) ? SDL_FULLSCREEN : 0
        if (!ecran)
        {
            printf("Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        //mise en place de l'audio
        if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        {
            printf("Erreur lors de la création audio: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        Mix_Music *music = NULL;
        systEfSound effectSound;
        effectSound.button=NULL,effectSound.touche=NULL;
        iniSound(config,&music,&effectSound);

        SDL_WM_SetCaption("Choplifter",NULL);

        //affichage du menu
        Menu = IMG_Load(NOM_FICHIER_MENU);

        positionMenu.x = 0;
        positionMenu.y = 0;

        //on vérifie si une erreur c'est produite
        continuer=verif_erreur();

        if(TTF_Init()==-1)
        {
            erreur_indef("Erreur chargement de la TTF.");
            exit(EXIT_FAILURE);
        }
        //création de la police
        TTF_Font *police=NULL;
        SDL_Surface *texte=NULL,*texte2=NULL,*texte3=NULL,*texte4=NULL,*texte5=NULL;
        //couleur de la police
        SDL_Color couleur={255,255,255};

        police=TTF_OpenFont(NOM_FONT,TAILLE_POLICE);
        if(police==NULL)
            erreur_indef("Chargement de la police échouer.");
        else{
            texte=TTF_RenderText_Blended(police,"Jeu",couleur);
            texte2=TTF_RenderText_Blended(police,"Editeur",couleur);
            texte3=TTF_RenderText_Blended(police,"Classement",couleur);
            texte4=TTF_RenderText_Blended(police,"Option",couleur);
            texte5=TTF_RenderText_Blended(police,"Quitter",couleur);
        }

        //création des boutons
        imgMenu buton,butonOn;
        iniBouton(&buton,&butonOn,ecran);

        //On crée une surface noir qui fera office de "clear ecran"
        clearScreen=SDL_CreateRGBSurface(SDL_HWSURFACE,ecran->w,ecran->h,32,0,0,0,0);
        SDL_FillRect(clearScreen,NULL,SDL_MapRGB(ecran->format,0,0,0));
        positionClear.x=0;
        positionClear.y=0;

        //choix du menu
        while(continuer)
        {
            //On vérifie si une erreur c'est produit dans le jeu, dans l'éditeur ou dans le menu
            continuer=verif_erreur();

            SDL_Event event;

            SDL_WaitEvent(&event);

            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
               break;

                case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE:
                            case SDLK_5:
                            case SDLK_KP5:
                                Mix_PlayChannel( -1, effectSound.touche , 0 );
                                continuer=0;
                            break;

                            case SDLK_j:
                            case SDLK_1:
                            case SDLK_KP1:
                                Mix_PlayChannel( -1, effectSound.touche , 0 );
                                jeu(ecran,police,config,&effectSound);
                            break;

                            case SDLK_e:
                            case SDLK_2:
                            case SDLK_KP2:
                                Mix_PlayChannel( -1, effectSound.touche , 0 );
                                editeur(ecran);
                            break;

                            case SDLK_q:
                            case SDLK_3:
                            case SDLK_KP3:
                                Mix_PlayChannel( -1, effectSound.touche , 0 );
                                affClmt(ecran,police,config.flagCheat);
                            break;

                            case SDLK_o:
                            case SDLK_4:
                            case SDLK_KP4:
                                Mix_PlayChannel( -1, effectSound.touche , 0 );
                                restart = ecranConf(ecran,police,buton,butonOn,&effectSound);
                                continuer = !(restart);
                            break;
                        }
                break;
            }

            SDL_BlitSurface(clearScreen, NULL, ecran, &positionClear);

            /*Celon la doc' de la SDL, si l'image est "coupé", les coordonnées sont restauré a 0.
            Conclusion : On re attribue les coordonnées au cas où si sa arrive*/
            positionMenu.x = ecran->w/2-Menu->w/2;
            positionMenu.y = ecran->h/2-Menu->h/2;
            SDL_BlitSurface(Menu, NULL, ecran, &positionMenu);

            if(bouton(&buton,&butonOn,ecran->w/2-buton.img->w/2,ecran->h*1.5/10,event,texte)){
                Mix_PlayChannel( -1, effectSound.button, 0 );
                jeu(ecran,police,config,&effectSound);
            }
            else if(bouton(&buton,&butonOn,ecran->w/2-buton.img->w/2,ecran->h*3/10,event,texte2)){
                Mix_PlayChannel( -1, effectSound.button, 0 );
                editeur(ecran);
            }
            else if(bouton(&buton,&butonOn,ecran->w/2-buton.img->w/2,ecran->h*4.5/10,event,texte3)){
                Mix_PlayChannel( -1, effectSound.button, 0 );
                affClmt(ecran,police,config.flagCheat);
            }
            else if(bouton(&buton,&butonOn,ecran->w/2-buton.img->w/2,ecran->h*6/10,event,texte4)){
                Mix_PlayChannel( -1, effectSound.button, 0 );
                restart = ecranConf(ecran,police,buton,butonOn,&effectSound);
                continuer = !(restart);
            }
            else if(bouton(&buton,&butonOn,ecran->w/2-buton.img->w/2,ecran->h*7.5/10,event,texte5)){
                Mix_PlayChannel( -1, effectSound.button, 0 );
                continuer=0;
            }

            SDL_Flip(ecran);

        }

        SDL_FreeSurface(clearScreen);
        SDL_FreeSurface(Menu);
        SDL_FreeSurface(buton.img);
        SDL_FreeSurface(butonOn.img);
        SDL_FreeSurface(texte);
        SDL_FreeSurface(texte2);
        SDL_FreeSurface(texte3);
        SDL_FreeSurface(texte4);
        if(config.flagSound&FLAG_MUSIQUE)
            //Libération de la musique
            Mix_FreeMusic( music );
        if(config.flagSound&FLAG_EFFET)
        {
            Mix_FreeChunk( effectSound.button );
            Mix_FreeChunk( effectSound.touche );
            Mix_FreeChunk( effectSound.aplause );
        }
        //On quitte SDL_mixer
        Mix_CloseAudio();

        TTF_CloseFont(police);
        TTF_Quit();
        SDL_Quit();
    }while(restart);

    return 0;
}

void iniBouton(imgMenu *buton,imgMenu *butonOn,SDL_Surface *ecran)
{
    buton->img=NULL;
    butonOn->img=NULL;
    buton->img=IMG_Load(NOM_BOUTON_NORMAL);
    if(buton->img==NULL)
        erreur_image(NOM_BOUTON_NORMAL);
    butonOn->img=IMG_Load(NOM_BOUTON_ON);
    if(butonOn->img==NULL)
        erreur_image(NOM_BOUTON_ON);
    buton->ecran=ecran;
    butonOn->ecran=ecran;
}

void iniSound(conf config,Mix_Music **music,systEfSound *sound)
{
    if(config.flagSound&FLAG_MUSIQUE)
    {
        *music = Mix_LoadMUS(MUSIQUE_MENU);
        if(NULL!=*music)
            Mix_PlayMusic( *music, -1 );
        else
            erreur_indef(MUSIQUE_MENU);
    }
    if(config.flagSound&FLAG_EFFET)
    {
        sound->button = Mix_LoadWAV(SYSTEME_BUTTON);
        if(sound->button==NULL)
            erreur_indef(SYSTEME_BUTTON);

        sound->touche = Mix_LoadWAV(SYSTEME_ESCAP);
        if(sound->touche==NULL)
            erreur_indef(SYSTEME_ESCAP);

        sound->aplause = Mix_LoadWAV(SYSTEME_APLAUSE);
        if(sound->aplause==NULL)
            erreur_indef(SYSTEME_APLAUSE);
    }
}
