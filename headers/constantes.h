/*
constantes.h
----------

Fait par : Mehdi

Rôle : contient l'énumération, les structures et les défines pour le nom des fichiers a charger

Date de dernière modification : 05/04/2013

*/
#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//--------------------NOM FICHIER ECRAN/MAP--------------
#define NOM_FICHIER_MENU "img/MenuEcran.png"
#define NOM_FICHIER_VICTOIRE "img/MenuResult/Victoire.png"
#define NOM_FICHIER_DEFAITE "img/MenuResult/Defaite.png"

#define NOM_FICHIER_MAP_TERRE "img/Tilesets/terre.png"
#define NOM_FICHIER_MAP_CIEL "img/Tilesets/ciel.png"
#define NOM_FICHIER_MAP_ROUGE "img/Tilesets/scrolling.png"
//-------------------------------------------------------

//-----------------NOM FICHIER HELICO----------------------
#define NOM_FICHIER_HELICO "img/Helico/helico.png"
#define NOM_FICHIER_HELICO2 "img/Helico/helico2.png"
#define NOM_FICHIER_HELICO3 "img/Helico/helico3.png"
#define NOM_FICHIER_HELICO4 "img/Helico/helico4.png"
#define NOM_FICHIER_TIR_HELICO "img/Helico/tirHelico.png"
//---------------------------------------------------------

//-----------------NOM FICHIER TANK----------------------
#define NOM_FICHIER_TANK "img/Ennemie/Tank/tank1.png"
#define NOM_FICHIER_TANK2 "img/Ennemie/Tank/tank2.png"
#define NOM_FICHIER_TANK3 "img/Ennemie/Tank/tank3.png"
#define NOM_FICHIER_TANK4 "img/Ennemie/Tank/tank4.png"
#define NOM_FICHIER_TIR_TANK "img/Ennemie/Tank/tirTank.png"
//---------------------------------------------------------

//-----------------NOM FICHIER CASERNE--------------------
#define NOM_FICHIER_CASERNE "img/caserne.png"
#define NOM_FICHIER_CASERNE2 "img/caserne2.png"
#define NOM_FICHIER_BARIERE "img/bariere.png"

#define ECART_BASE_CASERNE 25
#define ECART_CASERNE_CASERNE 25
//---------------------------------------------------------

//-----------------NOM FICHIER OTAGE--------------------
#define NOM_FICHIER_OTAGE "img/Otage/otage.png"
#define NOM_FICHIER_OTAGE2 "img/Otage/otage2.png"
#define NOM_FICHIER_OTAGE3 "img/Otage/otage3.png"
#define NOM_FICHIER_OTAGE4 "img/Otage/otage4.png"
#define NOM_FICHIER_OTAGE5 "img/Otage/otage5.png"
//---------------------------------------------------------

//-----------------NOM FICHIER BASE--------------------
#define NOM_FICHIER_BASE "img/base.png"
//---------------------------------------------------------

//----------------------ECRAN------------------------------
#define LARGEUR 800
#define HAUTEUR 600
//---------------------------------------------------------

//----------------------TILESET------------------------------
#define NB_TILESETS 3
//-----------------------------------------------------------

//----------------------NOMBRE DE SPRITE---------------------
#define NOMBRE_TANK 3
#define NOMBRE_AVION 2
#define NOMBRE_SOUCOUPE 1
#define NB_CASERNE 5
#define NOMBRE_MAX_CASERNE 5
#define NB_OTAGE_PAR_CASERNE 6
//-----------------------------------------------------------

//----------------------VIE SPRITE---------------------------
#define VIE_HELICO 3
#define VIE_TANK 1
#define VIE_AVION 2
#define VIE_SOUCOUPE 1
#define VIE_CASERNE 1
#define VIE_OTAGE 1
//-----------------------------------------------------------

//Plus le nombre est élevé plus le tir est rapide
#define VITESSE_TIR_ENNEMIE 15

#define TEMPS_POSE 50
#define TAILLE_TABLEAU_TEMPS 30

enum {IMAGE1,IMAGE2,IMAGE3,IMAGE4,IMAGE5,IMAGE6,IMAGE7,IMAGE8,IMAGE9,IMAGE10};
enum {GAUCHE,DROITE,BAS,HAUT,FACE};

typedef struct temps temps;
struct temps
{
    int tempsPrecedent[TAILLE_TABLEAU_TEMPS];
    int tempsActuel;
};

//CoefDirecteur et coefIndice sont les constantes de l'équation y=ax+b du tir. A est le coefDirecteur et B est le coefIndice.
typedef struct tirSprite tirSprite;
struct tirSprite
{
    SDL_Surface *image;
    SDL_Rect positionTir;
    SDL_Rect cibleTir;
    int actionEnCour;
    int signeEquation;
    double coefDirecteur;
    double coefIndice;

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
    tirSprite tir;
    SDL_Rect positionMap;
    SDL_Surface *positionEcran;

};


typedef struct sprite sprite;
struct sprite
{
    numImage image[10];
    imageActu imageUtilise;
    int vie;
};

typedef struct tilesets tilesets;
struct tilesets
{
    SDL_Surface *image[10];
};

typedef struct otage otage;
struct otage
{
    int file;
    sprite strucSprite;
    int nbOtage;
};

#endif // CONSTANTES_H_INCLUDED
