/*
ennemie.h
----------

Fait par : Mehdi

Rôle : Contient les fonctions des sprites ennemies comme le Tank,l'Avion et la Soucoupe

Date de dernière modification : 05/04/2013

*/
#ifndef ENNEMIE_H_INCLUDED
#define ENNEMIE_H_INCLUDED

//---------------------------------PARTIE TANK-----------------------------------------
//Initialise le sprite tank
void iniTank(SDL_Surface *ecran,sprite *tank);
//Permet de déplacé le Tank
void deplacementTank(sprite *tank,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsTank);
//Calcul la trajectoire du Tank
void calculTrajectoireTank(sprite *tank,sprite *helico,int positionMap,tilesets *tilesetsMap,int tempsActu,int *tempsPrece);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE AVION----------------------------------------
//Initialise le sprite Avion
void iniAvion(SDL_Surface *ecran,sprite *avion);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE SOUCOUPE-------------------------------------
//Initialise le sprite Soucoupe
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE GENERAL--------------------------------------

//-------------------------------------------------------------------------------------

#endif // ENNEMIE_H_INCLUDED
