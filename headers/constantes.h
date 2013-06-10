/*
constantes.h
----------

Fait par : Mehdi

Rôle : contient l'énumération, les structures et les défines pour le nom des fichiers a charger

Date de dernière modification : 05/04/2013

*/
#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//Activé ou non le mode designer
#define MODE_DESIGNER 0

//--------------------NOM FICHIER ECRAN/MAP--------------
#define NOM_FICHIER_MENU "img/MenuEcran.png"
#define NOM_FICHIER_CLMT "img/classement.png"
#define NOM_FICHIER_VICTOIRE "img/MenuResult/Victoire.png"
#define NOM_FICHIER_DEFAITE "img/MenuResult/Defaite.png"

#define NOM_FICHIER_MAP_TERRE "img/Tilesets/terre.png"
#define NOM_FICHIER_MAP_CIEL "img/Tilesets/ciel.png"
#define NOM_FICHIER_MAP_ROUGE "img/Tilesets/scrolling.png"

#define NOM_FICHIER_EXPLOSION "img/explosion.png"
#define NB_EXPLOSION 5

#define NOM_FONT "font/spaceranger.ttf"
#define NOM_BOUTON_NORMAL "img/Bouton/Buton.png"
#define NOM_BOUTON_ON "img/Bouton/ButonOn.png"

#define TAILLE_MAP_PREDEFINIE 50
//-------------------------------------------------------

//-----------------NOM FICHIER HELICO----------------------
#define NOM_FICHIER_HELICO "img/Helico/helico.png"
#define NOM_FICHIER_HELICO2 "img/Helico/helico2.png"
#define NOM_FICHIER_HELICO3 "img/Helico/helico3.png"
#define NOM_FICHIER_HELICO4 "img/Helico/helico4.png"
#define NOM_FICHIER_HELICO5 "img/Helico/helico5.png"
#define NOM_FICHIER_HELICO6 "img/Helico/helico6.png"
#define NOM_FICHIER_TIR_HELICO "img/Helico/tirHelico.png"
//---------------------------------------------------------

//-----------------NOM FICHIER TANK----------------------
#define NOM_FICHIER_TANK "img/Ennemie/Tank/tank1.png"
#define NOM_FICHIER_TANK2 "img/Ennemie/Tank/tank2.png"
#define NOM_FICHIER_TANK3 "img/Ennemie/Tank/tank3.png"
#define NOM_FICHIER_TANK4 "img/Ennemie/Tank/tank4.png"
#define NOM_FICHIER_TIR_TANK "img/Ennemie/Tank/tirTank.png"
//---------------------------------------------------------

//-----------------NOM FICHIER AVION----------------------
#define NOM_FICHIER_AVION "img/Ennemie/Avion/Avion.png"
#define NOM_FICHIER_AVION2 "img/Ennemie/Avion/Avion2.png"
#define NOM_FICHIER_AVION3 "img/Ennemie/Avion/Avion3.png"
#define NOM_FICHIER_AVION4 "img/Ennemie/Avion/Avion4.png"

#define NOM_FICHIER_TIR_AVION "img/Ennemie/Avion/roquette1.png"
#define NOM_FICHIER_TIR_AVION2 "img/Ennemie/Avion/roquette2.png"
#define NOM_FICHIER_TIR_AVION3 "img/Ennemie/Avion/roquette3.png"
#define NOM_FICHIER_TIR_AVION4 "img/Ennemie/Avion/roquette4.png"
//---------------------------------------------------------

//-----------------NOM FICHIER SOUCOUPE----------------------
#define NOM_FICHIER_SOUCOUPE "img/Ennemie/Soucoupe/Soucoupe1.png"
#define NOM_FICHIER_SOUCOUPE2 "img/Ennemie/Soucoupe/Soucoupe2.png"
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
#define NOM_FICHIER_OTAGE6 "img/Otage/otage6.png"
//---------------------------------------------------------

//-----------------NOM FICHIER BASE--------------------
#define NOM_FICHIER_BASE "img/base.png"
//---------------------------------------------------------

//----------------------ECRAN------------------------------
#define LARGEUR 800
#define HAUTEUR 600
//---------------------------------------------------------

//----------------------NOMBRE DE SPRITE---------------------
#define NOMBRE_TANK 3
#define NOMBRE_AVION 2
#define NOMBRE_SOUCOUPE 1
#define NB_CASERNE 5
#define NOMBRE_MAX_CASERNE 5
#define NB_OTAGE_PAR_CASERNE 16
//-----------------------------------------------------------

//----------------------ORDRE D APARITION---------------------
//Nombre entre 0 et 99. Plus le nombre est élevé et plus l'ennemie mettra du temps a venir dans le jeu
#define IMPORTANCE_TANK 10
#define IMPORTANCE_AVION 30
#define IMPORTANCE_SOUCOUPE 50
//-----------------------------------------------------------

//----------------------TEMPS AVANT RESPAWN(en ms)-----------
#define RESPAWN_SOUCOUPE 10000
#define RESPAWN_TANK 5000
#define RESPAWN_AVION 15000
//-----------------------------------------------------------

//----------------------VIE SPRITE---------------------------
#define VIE_HELICO 3
#define VIE_TANK 1
#define VIE_AVION 2
#define VIE_SOUCOUPE 3
#define VIE_CASERNE 1
#define VIE_OTAGE 1
//-----------------------------------------------------------

//----------------------SCORE COEF------------
#define OTAGE_BORD_COEF 100
#define OTAGE_BASE_COEF 300
#define COEF_TMP_VIC 10000000
#define COEF_VIE_VIC 1000
//---------------------------------------------

//----------------------POLICE------------
#define TAILLE_POLICE 20
#define NOM_FONT "font/spaceranger.ttf"
//----------------------------------------

//----------------------TEMPS ECART TABLEAU TEMPS------------
#define TMP_DECAL_TAB_TANK 4
#define TMP_DECAL_TAB_AVION 4
#define TMP_DECAL_TAB_SOUCOUPE 2
//-----------------------------------------------------------

//----------------------FLAG SCREEN------------
#define SCREEN_800x600 1
#define SCREEN_1024x768 2
#define SCREEN_1280x800 4
#define SCREEN_1440x900 8
#define SCREEN_AUTO 16
#define SCREEN_FULL 32
//---------------------------------------------

//----------------------FLAG CHEAT-------------
#define CHEAT_INVINCIBLE 1
#define CHEAT_YANN 2
//---------------------------------------------

//----------------------NOM FICHIER JEU-------------
#define FICHIER_CONFIG "all.txt"
#define FICHIER_CLASSEMENT "classement.sav"
#define FICHIER_MAP "map.txt"
//---------------------------------------------

//nombre total d'otage a sauvé dans le jeu
#define NB_TOTAL_OTAGE NB_CASERNE*NB_OTAGE_PAR_CASERNE

//Plus le nombre est élevé plus le tir est rapide
#define VITESSE_TIR_ENNEMIE 15

#define TEMPS_POSE 50
#define TAILLE_TABLEAU_TEMPS 40

enum {IMAGE1,IMAGE2,IMAGE3,IMAGE4,IMAGE5,IMAGE6,IMAGE7,IMAGE8,IMAGE9,IMAGE10};
enum {GAUCHE,DROITE,BAS,HAUT,FACE};

typedef struct temps temps;
struct temps
{
    int tempsPrecedent[TAILLE_TABLEAU_TEMPS];
    int tempsActuel;
    int tempsDebut;
};

//CoefDirecteur et coefIndice sont les constantes de l'équation y=ax+b du tir. A est le coefDirecteur et B est le coefIndice.
typedef struct tirSprite tirSprite;
struct tirSprite
{
    SDL_Surface *image[10];
    SDL_Rect positionTir;
    SDL_Rect cibleTir;
    int actionEnCour;
    int signeEquation;
    double coefDirecteur;
    double coefIndice;
    int directionTir;
    int numeroImage;
    int nbExplosion;

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

//Temps mort représente la date de mort du sprite dans le jeu
typedef struct sprite sprite;
struct sprite
{
    numImage image[10];
    imageActu imageUtilise;
    int vie;
    int tempsMort;
};

typedef struct imageTileset imageTileset;
struct imageTileset
{
    SDL_Surface *image;
    char nom[50];
    char cheminImage[260];
};

typedef struct tilesets tilesets;
struct tilesets
{
    imageTileset    infoImage[10];
    int             nbTilesets;
};

typedef struct otage otage;
struct otage
{
    int file;
    sprite strucSprite;
    int nbOtage;
};

typedef struct imgMenu imgMenu;
struct imgMenu
{
    SDL_Rect positionImg;
    SDL_Surface *img;
    SDL_Surface *ecran;
};

typedef struct conf conf;
struct conf
{
    int flagCheat;
    int flagScreen;
};

#endif // CONSTANTES_H_INCLUDED
