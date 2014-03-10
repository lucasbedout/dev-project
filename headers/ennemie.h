/*
ennemie.h
----------

Fait par : Mehdi

R�le : Contient les fonctions des sprites ennemies comme le Tank,l'Avion et la Soucoupe

Date de derni�re modification : 05/04/2013

*/
#ifndef ENNEMIE_H_INCLUDED
#define ENNEMIE_H_INCLUDED

//---------------------------------PARTIE TANK-----------------------------------------
//Initialise le sprite tank
void iniTank(SDL_Surface *ecran,sprite *tank,int level);
//Permet de d�plac� le Tank
void deplacementTank(sprite *tank,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsTank);
//Calcul la trajectoire du Tank
void calculTrajectoireTank(sprite *tank,sprite *helico,int positionMap,tilesets *tilesetsMap,int tempsActu,int *tempsPrece);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE AVION----------------------------------------
//Initialise le sprite avion
void iniAvion(SDL_Surface *ecran,sprite *avion,int level);
//Permet de d�plac� l'avion
void deplacementAvion(sprite *avion,sprite *helico,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsAvion);
//Calcul la trajectoire de l'avion de fa�on lin�aire
void calculTrajectoireAvion(sprite *avion,sprite *helico,int positionMap,tilesets *tilesetsMap,int tempsActu,int *tempsPrece,Mix_Chunk **soundTir);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE SOUCOUPE-------------------------------------
//Initialise le sprite Soucoupe
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe,int level);
//permet de d�placer la soucoupe sur la map
void deplacementSoucoupe(sprite *soucoupe,sprite *helico,int positionMap,tilesets *tilesetsMap,int** map,int tempsActu,int *tempsSoucoupe);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE GENERAL--------------------------------------
//Retourne une position al�atoire dans la war zone
int spawnAlea(sprite *Sprite,tilesets *tilesetsMap,int positionMap);
//retourne 1 si il est possible de respawn, sinon retourne 0
int autorisationRespawn(sprite *Sprite,int tempsActu,int tempsRespawn);
void respawn(sprite *Sprite,tilesets *tilesetsMap,int tempsActu,int tempsRespawn,int vie,int difficulte,int positionMap);
//-------------------------------------------------------------------------------------

#endif // ENNEMIE_H_INCLUDED
