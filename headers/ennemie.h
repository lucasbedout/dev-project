#ifndef ENNEMIE_H_INCLUDED
#define ENNEMIE_H_INCLUDED

//---------------------------------PARTIE TANK-----------------------------------------
void iniTank(SDL_Surface *ecran,sprite *tank);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE AVION----------------------------------------
void iniAvion(SDL_Surface *ecran,sprite *avion);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE SOUCOUPE-------------------------------------
void iniSoucoupe(SDL_Surface *ecran,sprite *soucoupe);
//-------------------------------------------------------------------------------------

//---------------------------------PARTIE GENERAL--------------------------------------
int animationEnnemie(int image,int tempsActu,int tempsPrece,SDL_Surface *ecran,sprite *ennemie);
//-------------------------------------------------------------------------------------

#endif // ENNEMIE_H_INCLUDED
