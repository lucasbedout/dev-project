/*
erreur.h
----------

Fait par : Mehdi

R�le : Permet de gerer les erreurs ou de tester des valeurs

Date de derni�re modification : 28/02/2013

*/
#ifndef ERREUR_H_INCLUDED
#define ERREUR_H_INCLUDED

//verifie si il n'y a eu aucune erreur dans l'ouverture fichier d'image
void erreur_image(char nomFichier[]);
//verifie si il n'y eu aucune erreur dans l'allocation dynamique
void erreur_allocation(char endroitAllouer[]);
//Verifie si le fichier map, si il n'existe pas on l'indique et cr�e un fichier
int erreur_chargement_map();
//suprime le fichier d'erreur
void supr_fichier_erreur();
//Verifie si le fichier erreur a �t� ouvert ou cr�er
int verif_erreur();
//On cr�e un fichier qui �crit. Fonction de test
void test();
//Affiche une valeur pour le test
void test_valeur(double valeur,char information[]);

#endif // ERREUR_H_INCLUDED
