/*
helicoptere.h
----------

Fait par : Mehdi

R�le : Prototypes fonctions lier a l'h�licopt�re

Date de derni�re modification : 03/03/2013

*/
#ifndef HELICOPTERE_H_INCLUDED
#define HELICOPTERE_H_INCLUDED

//initilisation de l'h�licopt�re : position,chargement image,etc...
void iniPosHelicoptere(SDL_Surface *ecran,sprite *helico);
//D�place l'h�licopt�re dans la map
void deplacementHelico(sprite *helico,SDL_Event* even,int *positionMap,tilesets *tilesetsMap,int** map);
//Anime le sprite de l'h�licopt�re
int animationHelico(int image,int tempsActu,int tempsPrece,SDL_Surface *ecran,sprite *helico);
//Calcul la trajectoire du tir
void calculTrajectoireTirHelico(SDL_Event* even,sprite *helico,int positionMap,tilesets *tilesetsMap);
//gestion des colisions
void GestionColision(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu);
//retour 0 si l'h�licopt�re vole sinon retourne 1 si il est atterrie
int atterrissageHelico(sprite *helico,int** map,tilesets *tilesetsMap,int positionActu);
//Gestion de la vie de l'h�licopt�re
void Gestion_Vie_helico(int *vie,sprite *helico,sprite *tank,SDL_Rect *tir_Tank,sprite *avion,SDL_Rect *tir_Avion);

#endif // HELICOPTERE_H_INCLUDED
