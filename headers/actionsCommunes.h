#ifndef ACTIONSCOMMUNES_H_INCLUDED
#define ACTIONSCOMMUNES_H_INCLUDED

//Permet d'animé le sprite
int animationSprite(int image,int tempsActu,int tempsPrece,sprite *spriteAnime,tilesets *tilesetsMap,int positionMap);
//Regarde si un otage monte dans l'hélico et retourne 1 si c'est le cas sinon 0
int hotage_monte_helico(sprite *helico,sprite *otage, int** map,tilesets *tilesetsMap,int positionActu);
//Fait dessendre les otages dans la base
void hotage_dessend_helico(int positionMap,int *nbOtageHelico,int file,sprite *helico, int** map,tilesets *tilesetsMap,int positionActu);
//On regarde si un otage a été écrasé, si c'est le cas on renvoit 1 sinon 0
int Helico_ecrase_otage(sprite *helico,sprite *otage,int** map,tilesets *tilesetsMap,int positionActu);

#endif // ACTIONSCOMMUNES_H_INCLUDED
