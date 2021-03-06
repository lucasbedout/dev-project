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
//permet de blitter le tir du sprite en fonction de la trajectoire. Retourne 1 si le tir est en cour, sinon 0 si il est fini
int tir(sprite *typeSprite,tilesets tilesetsMap,Mix_Chunk **soundTir);
//gerre la vie d'un sprite. renvois 0 si rien ne se passe, renvois 1 si la vie du sprite a diminuer
int Gestion_Vie_sprite(sprite *Sprite,sprite *helico,tilesets *tilesetsMap,int tempsActu);
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
//Retourne le pourcentage d'otage sauv� dans la base
int pourcentSavOtageBase(int nbOtageBase,int nbCaserne);
//Retourne si il reste des otages a sauver ou non. retourne 0 si il ne reste aucun otage, retourne 1 si il reste des otages
int restOtage(otage tabOtage[],int nbCaserne,int nbOtageBord);
//Retourne 1 si les sprites se touche, sinon retourne 0
int colisionSprite(sprite spriteMouvant,sprite spriteColision,tilesets tilesetsMap);
//Retourne 1 si l'h�lico touche le sprite, sinon retourne 0
int colisionSpriteHelico(sprite helico,sprite Sprite,tilesets tilesetsMap,int positionMap);
//Initialise l'explosion
void iniExplosion(imgMenu *explosion,SDL_Surface *ecran);
//d�clenche l'explosion
void declenchementExplosion(imgMenu *explosion,sprite *spriteToucher, sprite spriteTirreur,Mix_Chunk **soundTir);
//Affiche l'explosion
void afficheExplosion(sprite *Sprite,imgMenu explosion,tilesets tilesetsMap,int positionMap);
//On regarde si un otage a �t� �cras�, si c'est le cas on renvoit 1 sinon 0
int Helico_ecrase_otage(sprite helico,sprite Otage,int** map,tilesets tilesetsMap,int positionActu);
//test
void jaugeVie(sprite spriteCible,int vieTotal,int vieActu,int largueurTile,int positionMap);

#endif // ACTIONSCOMMUNES_H_INCLUDED
