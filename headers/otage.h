/*
otage.h
----------

Fait par : Mehdi

Rôle :  Contient les fonctions des casernes, otage et juste une fonction
        d'initialisation du sprite de Base

Date de dernière modification : 05/04/2013

*/
#ifndef OTAGE_H_INCLUDED
#define OTAGE_H_INCLUDED

//initialise la caserne a une position précise
void iniCaserne(SDL_Surface *ecran,sprite *caserne,int** map,tilesets* tilesetsMap,int positionX);
//initialise les otages en fonction de x et y ( coordonnées donnée )
void iniOtage(SDL_Surface *ecran,otage *otage,int** map,tilesets* tilesetsMap,int positionX);
//Permet de déplacé l'otage vers l'hélico si l'hélico est attérie
void deplacementOtageVersHelico(otage *Otage,sprite *helico,sprite *bariere,int** map,tilesets *tilesetsMap,int positionMap,int tempsActu,int *tempsOtage);
//Permet de déplacé l'otage vers la base si l'hélico est attérie
void deplacementOtageVersBase(otage *Otage,sprite *helico,sprite *base,int** map,tilesets *tilesetsMap,int positionMap,int tempsActu,int *tempsOtage);
//Gerre la file d'otage
void gestionFileOtage(otage *Otage,sprite *helico,int positionCaserneX,int positionCaserneY,int **map,tilesets *tilesetsMap,int positionActu,int *nbOtageBord);
//Initialise la base
void iniBase(SDL_Surface *ecran,sprite *base,int** map,tilesets* tilesetsMap);
//initialisation de la bariere qui délimite la saveZone et la warZone
void iniBariere(SDL_Surface *ecran,sprite *bariere,int** map,tilesets* tilesetsMap);
//retourne 1 si il y a colision, sinon retourne 0
int colisionBariere(sprite *typeSprite,sprite *bariere,tilesets* tilesetsMap);

#endif // OTAGE_H_INCLUDED
