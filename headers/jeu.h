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
#include <SDL_mixer.h>

//lance le jeu
void jeu (SDL_Surface *ecran,TTF_Font *police,conf config,systEfSound *sound);
//permet de sortir, mettre le jeu en pose, etc...
void option(int* continuer,SDL_Event *even,TTF_Font *police,SDL_Surface *ecran,systEfSound *sound,conf config,Mix_Chunk **helicoSound);
//Initialise le temps du jeu
void initilisationTemps(int tab[TAILLE_TABLEAU_TEMPS]);
//initialise les effets de son du jeu
void iniSoundJeu(conf config,Mix_Chunk **avionEffect,Mix_Chunk **soucoupeEffect,Mix_Chunk **tankEffect,Mix_Chunk **helicoEffect,Mix_Chunk **explosionEffect,Mix_Chunk **shootEffect);
//Libere les sons en mémoire
void libSoundJeu(conf config,Mix_Chunk **avionEffect,Mix_Chunk **soucoupeEffect,Mix_Chunk **tankEffect,Mix_Chunk **helicoEffect,Mix_Chunk **explosionEffect,Mix_Chunk **shootEffect);
//Active le son et régle le volume
void activSoundJeu(conf config,Mix_Chunk **avionEffect,Mix_Chunk **soucoupeEffect,Mix_Chunk **tankEffect,Mix_Chunk **helicoEffect);
//Stop le sound d'un jeu
void stopSoundJeu(conf config,Mix_Chunk **sound);
//Active le sound d'un jeu
void repriseSoundJeu(conf config,Mix_Chunk **sound);
#endif // JEU_H_INCLUDED
