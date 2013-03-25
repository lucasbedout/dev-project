/*
map.h
----------

Fait par : Mehdi

Rôle : Contient les fonctions de gestion de map

Date de dernière modification : 26/03/2013

*/
#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

//Déclare tout les  tilesets
void declaration_tilesets(tilesets *tilesetsMap);
//Remplie automatiquement la map ( map statique et prédéfinie )
int** map_remplie(SDL_Surface *ecran);
//Calcul la taille de la map si le fichier map est présent
int taille_map();
//Charge la map si le fichier map est présent
int** chargement_map(SDL_Surface *ecran);
//Affiche la map
void affiche_map(int **map,SDL_Surface *ecran,tilesets imageMap,int y);
//libére la mémoire allouer par les tilesets
void liberation_tilesets(tilesets *tilesetsMap);
//On cherche la limite de l'axe des x
int limite_map(sprite *helico,tilesets *imageMap);
//retourne le nombre de pixel la hauteur du sol
int hauteur_sol(int** map,tilesets *tilesetsMap,SDL_Surface *ecran,int j);
//Permet a une image de se "fixé sur la map" et non se "fixé par rapport a la fenetre" sauf que l'unité de valeur de l'hauteur est en pixel
void decallement_image_map_hauteurPixel(sprite *image,tilesets *tilesetsMap,int hauteurPixel,int j,int positionMap,SDL_Surface *numeroImage);

#endif // MAP_H_INCLUDED
