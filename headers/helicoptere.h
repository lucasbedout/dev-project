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
//Gestion de la vie de l'h�licopt�re. Retourne 1 si l'h�lico est toucher, retourne 0 si il ne l'est pas.
int Gestion_Vie_helico(sprite *helico,sprite *ennemie,int positionMap,tilesets tilesetsMap);
//gestion de la vie de l'h�lico si quelque chose le touche. Retourne 1 si il y a colision sinon retourne 0.
int gestion_colision_helico(sprite *helico,sprite *ennemie,int positionMap,tilesets tilesetsMap);
//Permet de faire revenir la position de l'h�lico a son origine si il y a une colisition
void gestionColisionSprite(sprite *helico,sprite spriteCible,tilesets tilesetsMap,int *positionMap,SDL_Event even);

#endif // HELICOPTERE_H_INCLUDED
