/*
classement.c
----------

Fait par : Mehdi

Rôle : Permet de gerrer le classement du jeu

Date de dernière modifiction : 08/06/2013

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "headers/erreur.h"
#include "headers/constantes.h"
#include "headers/classement.h"

void verif_clmt()
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen(FICHIER_CLASSEMENT,"r");

    //si le fichier n'a pas été créer, on le crée
    if (fichier == NULL)
    {
        fichier=fopen(FICHIER_CLASSEMENT,"w");

        fputs("AAA 25350\nimpossiblu 20432\nMyName 15000\nMehdi 9999\nNoob 100",fichier);
    }
    //on ferme le fichier
    fclose(fichier);
}

void affClmt(SDL_Surface *ecran,TTF_Font *police,int flagCheat)
{
    verif_clmt();

    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen(FICHIER_CLASSEMENT,"r");

    char nomJoueur[5][20]={"N/A"},nomScore[5][10]={0},nomJoueurInt[5][20]={"N/A"};
    int i=0;

    for(i=0;i<5 && EOF!=fscanf(fichier, "%s %s",nomJoueurInt[i],nomScore[i]);i++);

    SDL_Surface *texteJoueurTTF=NULL,*texteScoreTTF=NULL,*imgClmt=NULL,*texteTitre=NULL,*clearScreen=NULL;
    SDL_Color couleur={255,255,255};
    SDL_Rect positionText={0};

    //On crée une surface noir qui fera office de "clear ecran"
    clearScreen=SDL_CreateRGBSurface(SDL_HWSURFACE,ecran->w,ecran->h,32,0,0,0,0);
    SDL_FillRect(clearScreen,NULL,SDL_MapRGB(ecran->format,0,0,0));

    imgClmt=IMG_Load(NOM_FICHIER_CLMT);
    if(imgClmt==NULL)
        erreur_image(NOM_FICHIER_CLMT);
    else{

        SDL_BlitSurface(clearScreen,NULL,ecran,&positionText);

        positionText.x=ecran->w/2-imgClmt->w/2;
        positionText.y=ecran->h/2-imgClmt->h/2;
        SDL_BlitSurface(imgClmt,NULL,ecran,&positionText);

        //création et blittage du titre
        TTF_SetFontStyle(police,TTF_STYLE_UNDERLINE);
        texteTitre=TTF_RenderText_Blended(police,"Classement :",couleur);
        TTF_SetFontStyle(police,TTF_STYLE_NORMAL);
        positionText.y=0;
        positionText.x=ecran->w/2-texteTitre->w/2;
        SDL_BlitSurface(texteTitre,NULL,ecran,&positionText);


        for(i=0;i<5;i++)
        {
            //Si le cheat est activé, on modifie les noms des joueurs
            if(flagCheat&CHEAT_YANN)
                sprintf(nomJoueur[i],"%d . YANN",i+1);
            else
                sprintf(nomJoueur[i],"%d . %s",i+1,nomJoueurInt[i]);
            texteJoueurTTF=TTF_RenderText_Blended(police,nomJoueur[i],couleur);
            texteScoreTTF=TTF_RenderText_Blended(police,nomScore[i],couleur);

            positionText.x=ecran->w/4;
            positionText.y=(i+1)*(ecran->h/6);
            SDL_BlitSurface(texteJoueurTTF,NULL,ecran,&positionText);
            positionText.x=ecran->w/1.5;
            SDL_BlitSurface(texteScoreTTF,NULL,ecran,&positionText);
        }

        SDL_Flip(ecran);

        SDL_Event even;

        //on hiberne le programme 1 seconde
        SDL_Delay(1000);

        //on attends que le joueur appuis sur une touche pour continuer
        even.type=NULL;
        while(even.type!=SDL_KEYUP)
            SDL_WaitEvent(&even);
    }

    //on ferme le fichier
    fclose(fichier);
    SDL_FreeSurface(clearScreen);
    SDL_FreeSurface(imgClmt);
    SDL_FreeSurface(texteJoueurTTF);
    SDL_FreeSurface(texteScoreTTF);
}

int newScoreClmt(int score)
{
    verif_clmt();

    int i=0;

    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen(FICHIER_CLASSEMENT,"r");

    char nomJoueur[20]={"N/A"};
    int nomScore=0;

    for(i=0;i<5 && EOF!=fscanf(fichier, "%s %d",nomJoueur,&nomScore);i++);

    fclose(fichier);

    return score>nomScore;
}

void insertNewScore(char nomJoueur[],int scoreJoueur)
{
    verif_clmt();

    int i=0;

    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen(FICHIER_CLASSEMENT,"r+");

    char nomJoueurClmt[5][20]={"N/A"};
    int nomScore[5]={0};

    for(i=0;i<5;i++)
        fscanf(fichier, "%s %d",nomJoueurClmt[i],&nomScore[i]);

    //On revient au debut du fichier
    rewind(fichier);

    for(i=0;i<5;i++)
    {
        if(scoreJoueur>nomScore[i])
        {
            if(i-1>=0)
            {
                if(nomScore[i-1]>=scoreJoueur)
                    fprintf(fichier,"%s %d\n",nomJoueur,scoreJoueur);
                else
                    fprintf(fichier,"%s %d\n",nomJoueurClmt[i-1],nomScore[i-1]);
            }
            else
                fprintf(fichier,"%s %d\n",nomJoueur,scoreJoueur);
        }
        else
            fprintf(fichier,"%s %d\n",nomJoueurClmt[i],nomScore[i]);
    }

    fclose(fichier);
}

char lectureCara(char *touche)
{
    //conversion de qwerty en azerty
    if(strlen(touche) == 1 && ((touche[0]>='a'&& touche[0] <='z') || touche[0] == ';' ) )
    {
        switch(touche[0])
        {
            case 'q':
                touche[0] = 'a';
            break;

            case 'w':
                touche[0] = 'z';
            break;

            case 'a':
                touche[0] = 'q';
            break;

            case ';':
                touche[0] = 'm';
            break;

            case 'z':
                touche[0] = 'w';
            break;
        }
        return touche[0];
    }
    else if(!strcmp(touche,"space"))
        return '_';

    else if(touche[0] == '[' && touche[2] == ']')
        return touche[1];

    else
        return '0';
}

char* enterName(int score,TTF_Font *police,SDL_Surface *ecran)
{
    SDL_Surface *texteTitreTTF=NULL,*texteJoueurTTF=NULL,*fond=NULL,*clearScreen=NULL;
    SDL_Color couleur={255,255,255};
    SDL_Rect positionText={0};
    SDL_Event even;
    int done=0,i=0;
    char name[20]="",touche[20]="";

    texteTitreTTF=TTF_RenderText_Blended(police,"NOUVEAU SCORE, ENTRER VOTRE PSEUDO",couleur);

    //On crée une surface noir qui fera office de "clear ecran"
    clearScreen=SDL_CreateRGBSurface(SDL_HWSURFACE,ecran->w,ecran->h,32,0,0,0,0);
    SDL_FillRect(clearScreen,NULL,SDL_MapRGB(ecran->format,0,0,0));

    fond=IMG_Load(NOM_FICHIER_CLMT);
    if(fond==NULL)
        erreur_image(NOM_FICHIER_CLMT);

    done=!(verif_erreur());
    while(!done){
        SDL_WaitEvent(&even);

        if(even.type==SDL_KEYDOWN){
            if(even.key.keysym.sym==SDLK_RETURN)
                done=1;
            else if(even.key.keysym.sym==SDLK_BACKSPACE && i>0)
            {
                i--;
                name[i]='\0';

                texteJoueurTTF=TTF_RenderText_Blended(police,name,couleur);
            }
            else{
                if(i<(20-1)){
                    sprintf(touche,"%s",SDL_GetKeyName(even.key.keysym.sym));
                    //Si la lecture de caractère est un caractère accepter
                    if('0'!=lectureCara(touche))
                    {
                        name[i]=lectureCara(touche);
                        i++;
                        name[i]='\0';

                        texteJoueurTTF=TTF_RenderText_Blended(police,name,couleur);
                    }
                }
            }
        }

        positionText.x=0;
        positionText.y=0;
        SDL_BlitSurface(clearScreen,NULL,ecran,&positionText);

        positionText.x=ecran->w/2-fond->w/2;
        positionText.y=ecran->h/2-fond->h/2;
        SDL_BlitSurface(fond,NULL,ecran,&positionText);

        positionText.x= ecran->w/2 - texteTitreTTF->w/2;
        positionText.y=50;
        SDL_BlitSurface(texteTitreTTF,NULL,ecran,&positionText);

        if(texteJoueurTTF!=NULL)
        {
            positionText.x= ecran->w/2 - texteJoueurTTF->w/2;
            positionText.y=ecran->h/2 - texteJoueurTTF->h/2;
            SDL_BlitSurface(texteJoueurTTF,NULL,ecran,&positionText);
        }

        SDL_Flip(ecran);
    }

    SDL_FreeSurface(clearScreen);
    SDL_FreeSurface(texteTitreTTF);
    SDL_FreeSurface(texteJoueurTTF);
    SDL_FreeSurface(fond);

    return name;
}
