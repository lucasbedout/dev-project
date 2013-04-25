/*
actionsCommune.h
----------

Fait par : Mehdi

R�le : Contient les fonctions utilis� par les sprites ou en rapport entre eux.

Date de derni�re modification : 26/03/2013

*/
#ifndef ACTIONSCOMMUNES_H_INCLUDED
#define ACTIONSCOMMUNES_H_INCLUDED

//Permet d'anim� le sprite
int animationSprite(int image,int tempsActu,int tempsPrece,sprite *spriteAnime,tilesets *tilesetsMap,int positionMap);
//Permet d'anim� le tir
void animationTir (sprite *spriteAnime,tilesets *tilesetsMap,int positionMap);
//permet de blitter le tir du sprite en fonction de la trajectoire
void tir(sprite *typeSprite);
//gerre la vie d'un sprite
void Gestion_Vie_sprite(sprite *Sprite,sprite *helico,tilesets *tilesetsMap,int tempsActu);
//Regarde si un otage monte dans l'h�lico et retourne 1 si c'est le cas sinon 0
int hotage_monte_helico(sprite *helico,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu);
//Fait dessendre les otages dans la base
void hotage_dessend_helico(int positionMap,int *nbOtageBord,int *nbOtageBase,otage *Otage,sprite *helico,sprite *base, int** map,tilesets *tilesetsMap);
//Retourne 1 si l'otage est rentr� dans la base sinon retourne 0
int hotage_rentre_base(sprite *base,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu);
//retourne le debus de la position de la saveZone ( unit� : tilesets et le sprite est utilis� juste pour les propri�t� �cran )
int saveZone(sprite *Sprite,tilesets *tilesetsMap);
//Calcul automatiquement le nombre de caserne celon la map, le debus de la premiere caserne et l'espacement entre celles-ci. Return le nombre de caserne
int nbCaserneMap(sprite *Sprite,tilesets *tilesetsMap);
//Retourne le pourcentage d'otage sauv� et ceux qui sont a bord de l'h�licopt�re
int pourcentSavOtage(int nbOtageBord,int nbOtageBase,int nbCaserne);
//On regarde si un otage a �t� �cras�, si c'est le cas on renvoit 1 sinon 0
int Helico_ecrase_otage(sprite *helico,sprite *otage,int** map,tilesets *tilesetsMap,int positionActu);

#endif // ACTIONSCOMMUNES_H_INCLUDED
