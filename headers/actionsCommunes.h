/*
actionsCommune.h
----------

Fait par : Mehdi

Rôle : Contient les fonctions utilisé par les sprites ou en rapport entre eux.

Date de dernière modification : 26/03/2013

*/
#ifndef ACTIONSCOMMUNES_H_INCLUDED
#define ACTIONSCOMMUNES_H_INCLUDED

//Permet d'animé le sprite
int animationSprite(int image,int tempsActu,int tempsPrece,sprite *spriteAnime,tilesets *tilesetsMap,int positionMap);
//Permet d'animé le tir
void animationTir (sprite *spriteAnime,tilesets *tilesetsMap,int positionMap);
//permet de blitter le tir du sprite en fonction de la trajectoire
void tir(sprite *typeSprite);
//gerre la vie d'un sprite
void Gestion_Vie_sprite(sprite *Sprite,sprite *helico,tilesets *tilesetsMap,int tempsActu);
//Regarde si un otage monte dans l'hélico et retourne 1 si c'est le cas sinon 0
int hotage_monte_helico(sprite *helico,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu);
//Fait dessendre les otages dans la base
void hotage_dessend_helico(int positionMap,int *nbOtageBord,int *nbOtageBase,otage *Otage,sprite *helico,sprite *base, int** map,tilesets *tilesetsMap);
//Retourne 1 si l'otage est rentré dans la base sinon retourne 0
int hotage_rentre_base(sprite *base,otage *Otage, int** map,tilesets *tilesetsMap,int positionActu);
//retourne le debus de la position de la saveZone ( unité : tilesets et le sprite est utilisé juste pour les propriété écran )
int saveZone(sprite *Sprite,tilesets *tilesetsMap);
//Calcul automatiquement le nombre de caserne celon la map, le debus de la premiere caserne et l'espacement entre celles-ci. Return le nombre de caserne
int nbCaserneMap(sprite *Sprite,tilesets *tilesetsMap);
//Retourne le pourcentage d'otage sauvé et ceux qui sont a bord de l'hélicoptère
int pourcentSavOtage(int nbOtageBord,int nbOtageBase,int nbCaserne);
//On regarde si un otage a été écrasé, si c'est le cas on renvoit 1 sinon 0
int Helico_ecrase_otage(sprite *helico,sprite *otage,int** map,tilesets *tilesetsMap,int positionActu);

#endif // ACTIONSCOMMUNES_H_INCLUDED
