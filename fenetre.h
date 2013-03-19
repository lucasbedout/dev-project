/*
fenetre.h
----------

Fait par : Mehdi

R�le : Contient les prototypes de fonction propre a la fenetre

Date de derni�re modification : 16/01/2013

*/
#ifndef FENETRE_H_INCLUDED
#define FENETRE_H_INCLUDED

#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'�diteur de lien
#include <SDL_image.h>

//ouvre la fenetre du jeu
void fenetreJeu();
//pour sortir de la fenetre du jeu
void sortir(int* continuer,SDL_Surface *ecran);

#endif // FENETRE_H_INCLUDED
