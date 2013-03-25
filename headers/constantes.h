/*
constantes.h
----------

Fait par : Mehdi

R�le : contient l'�num�ration, les structures et les d�fines pour le nom des fichiers a charger

Date de derni�re modification : 28/02/2013

*/
#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//--------------------NOM FICHIER ECRAN/MAP--------------
#define NOM_FICHIER_MENU "img/MenuEcran.png"
#define NOM_FICHIER_MAP "img/map_scrolling.png"
#define NOM_FICHIER_MAP_TERRE "img/terre.png"
#define NOM_FICHIER_MAP_CIEL "img/ciel.png"
#define NOM_FICHIER_MAP_ROUGE "img/scrolling.png"
//-------------------------------------------------------

//-----------------NOM FICHIER HELICO----------------------
#define NOM_FICHIER_HELICO "img/helico.png"
#define NOM_FICHIER_HELICO2 "img/helico2.png"
#define NOM_FICHIER_HELICO3 "img/helico3.png"
#define NOM_FICHIER_HELICO4 "img/helico4.png"
#define NOM_FICHIER_TIR_HELICO "img/tirHelico.png"
//---------------------------------------------------------

//-----------------NOM FICHIER TANK----------------------
#define NOM_FICHIER_TANK "img/tank1.png"
#define NOM_FICHIER_TANK2 "img/tank2.png"
#define NOM_FICHIER_TANK3 "img/tank3.png"
#define NOM_FICHIER_TANK4 "img/tank4.png"
//---------------------------------------------------------

#define LARGEUR 800
#define HAUTEUR 600
#define TEMPS_POSE 50
#define TAILLE_TABLEAU_TEMPS 30

enum {IMAGE1,IMAGE2,IMAGE3,IMAGE4,IMAGE5};
enum {GAUCHE,DROITE};

typedef struct temps temps;
struct temps
{
    int tempsPrecedent[TAILLE_TABLEAU_TEMPS];
    int tempsActuel;
};

typedef struct numImage numImage;
struct numImage
{
    SDL_Surface *image;
    SDL_Rect position;
};

typedef struct imageActu imageActu;
struct imageActu
{
    int numeroImage;
    int direction;
    SDL_Surface *tir;
    SDL_Rect positionMap;
    SDL_Surface *positionEcran;

};


typedef struct sprite sprite;
struct sprite
{
    numImage image1;
    numImage image2;
    numImage image3;
    numImage image4;
    numImage image5;
    imageActu imageUtilise;
};

typedef struct tilesets tilesets;
struct tilesets
{
    SDL_Surface *image[10];
};

#endif // CONSTANTES_H_INCLUDED
