/*
jeu.h
----------

Fait par : Mehdi

Rôle : Prototypes des fonctions lier au jeu

Date de dernière modification : 18/01/2013

*/
#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>
#include <SDL_ttf.h>

//lance le jeu
void jeu (SDL_Surface *ecran,TTF_Font *police,conf config);
//permet de sortir, mettre le jeu en pose, etc...
void option(int* continuer,SDL_Event *even,TTF_Font *police,SDL_Surface *ecran);
//Initialise le temps du jeu
void initilisationTemps(int tab[TAILLE_TABLEAU_TEMPS]);

#endif // JEU_H_INCLUDED
