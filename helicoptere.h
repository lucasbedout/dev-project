/*
helicoptere.h
----------

Fait par : Mehdi

Rôle : Prototypes fonctions lier a l'hélicoptère

Date de dernière modification : 03/03/2013

*/
#ifndef HELICOPTERE_H_INCLUDED
#define HELICOPTERE_H_INCLUDED

//initilisation de l'hélicoptère : position,chargement image,etc...
void iniPosHelicoptere(SDL_Surface *ecran,sprite *helico);
//Déplace l'hélicoptère dans la map
void deplacementHelico(sprite *helico,SDL_Event* even,int *positionMap,tilesets *tilesetsMap,int** map);
//Anime le sprite de l'hélicoptère
int animationHelico(int image,int tempsActu,int tempsPrece,SDL_Surface *ecran,sprite *helico);
//Gestion du tir de l'hélicoptère
void tirHelico(SDL_Event* even,int *actionEnCour,SDL_Rect *cible, SDL_Rect *position,sprite *helico,SDL_Surface *ecran,double *a,double *b,int *equation);
//gestion des colisions
void GestionColision(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu);
//retour 0 si l'hélicoptère vole sinon retourne 1 si il est atterrie
int atterrissageHelico(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu);
//Gestion de la vie de l'hélicoptère
void Gestion_Vie_helico(int *vie,sprite *helico,sprite *tank,SDL_Rect *tir_Tank,sprite *avion,SDL_Rect *tir_Avion);
//Regarde si un otage monte dans l'hélico et retourne 1 si c'est le cas sinon 0
int hotage_monte_helico(sprite *helico,sprite *otage, int** map,tilesets *tilesetsMap,int positionActu);
//Fait dessendre les otages dans la base
void hotage_dessend_helico(int positionMap,int *nbOtageHelico,int file,sprite *helico, int** map,tilesets *tilesetsMap,int positionActu);
//On regarde si un otage a été écrasé, si c'est le cas on renvoit 1 sinon 0
int Helico_ecrase_otage(sprite *helico,sprite *otage,int** map,tilesets *tilesetsMap,int positionActu);

#endif // HELICOPTERE_H_INCLUDED
