/*
main.c
----------

Fait par : Mehdi

Rôle : Contient uniquement la fenetre

Date de dernière modification : 16/01/2013

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
//si l'erreur "undefined reference to `IMG_Load' " ajouter le fichier "SDL_image.lib" dans l'éditeur de lien
#include <SDL_image.h>

#include "fenetre.h"
#include "erreur.h"

int main ( int argc, char** argv )
{
    //on suprime le fichier erreur qui a put être créer
    supr_fichier_erreur();

    fenetreJeu();

    return 0;
}
