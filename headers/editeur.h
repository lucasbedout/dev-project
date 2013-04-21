/*
editeur.h
----------

Fait par : Mehdi

R�le : contient les fonction pour l'�diteur de scrolling map

Date de derni�re modification : 10/03/2013

*/
#ifndef EDITEUR_H_INCLUDED
#define EDITEUR_H_INCLUDED

#define NB_LIGNE_MAP_HAUTEUR 50

//-----------STRUCTURE POUR LES LISTE CHAINEE------------
typedef struct Element Element;
struct Element
{
    int colonne[NB_LIGNE_MAP_HAUTEUR];
    Element *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    Element *premier;
    Element *dernier;
};
//-------------------------------------------------------

//�diteur de map
void editeur (SDL_Surface *ecran);
//D�claration du tableau qui fait office de map
int** declaration_tableau_map(SDL_Surface *ecran,tilesets imageMap);
//initialise la map en mettant que des tilesets ciel
int** initilisationMap(int** map,int n,int m);
//permet de selectionner le tileset ou de sortir du programme
void selection(int* continuer,SDL_Event *even,int *typeImage,int *molette,int imageMax);
//Blitte la surface selectionner au niveau du curseur
void surface_map (SDL_Event *even,int typeImage,tilesets tilesetsMap,SDL_Surface *ecran,SDL_Rect *position);
//colle la surface a la map
int** coller_surface(int typeImage,tilesets tilesetsMap,int** map,SDL_Rect *position,SDL_Event *even,Liste *liste,int *positionActu,SDL_Surface *ecran);
//sauvegarde la map dans un fichier map.txt
void save_map(int** map,SDL_Surface *ecran,tilesets imageMap,SDL_Event *even,Liste* liste,int positionMax);
//ajoute un �l�ment a la fin de la liste
Liste* insertion_liste(Liste *liste);
//initialise une liste
Liste* initilisation_liste();
//initialise le tableau a 2
void initilisation_tab(int tab[],int limite);
//g�re la position au sein de la map et les d�placement de celle-ci
double position_map(SDL_Event *even,int* positionActu);
//aggrandie la map en fonction de l'utilisateur
void aggrandissement_map(int* positionActu,int* positionMax,Liste *liste);
//affiche la map avec gestion des listes
void affiche_map_editeur(int **map,SDL_Surface *ecran,tilesets imageMap,int y,Liste *liste);
//Suprime toute la liste ( lib�ration de la m�moire allouer par la liste
void suppression(Liste *liste);

#endif // EDITEUR_H_INCLUDED
