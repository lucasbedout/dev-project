#ifndef ENNEMIE_H_INCLUDED
#define ENNEMIE_H_INCLUDED

//---------------------------------PARTIE TANK-----------------------------------------
void iniTank(SDL_Surface *ecran,sprite *tank);
void deplacementTank(sprite *tank,int positionMap,tilesets *tilesetsMap,int** map);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE AVION----------------------------------------
void iniAvion(SDL_Surface *ecran,sprite *avion);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE SOUCOUPE-------------------------------------
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE GENERAL--------------------------------------

//-------------------------------------------------------------------------------------

#endif // ENNEMIE_H_INCLUDED
