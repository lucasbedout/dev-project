/*
Erreur.c
----------

Fait par : Mehdi

Rôle : Gestion des erreurs quelquonques

Date de dernière modifiction : 16/02/2013

*/
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#include "erreur.h"

//Pour avoir les informations horaire complette de l'erreur
char *datetime(void)
{
  time_t t = time(0);
  static char buf[32];
  strftime(buf, sizeof buf, "%d/%m/%Y %H:%M:%S", localtime(&t));
  return buf;
}

void erreur_image(char nomFichier[])
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("erreur_image.txt","a");

    //si l'ouverture réussi
    if (fichier != NULL)
    {
        fprintf(fichier,"---------------------ERREUR DU : %s ",datetime());
        fputs("-----------------------\n",fichier);

        fprintf(fichier,"Fichier manquant : %s \n ",nomFichier);


        //on ferme le fichier
        fclose(fichier);
    }
    //si l'ouverture échoue
    else
    {
        printf("Erreur ouverture du fichier erreur_image.txt");
    }
}

void erreur_allocation(char endroitAllouer[])
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("erreur_allocation.txt","a");

    //si l'ouverture réussi
    if (fichier != NULL)
    {
        fprintf(fichier,"---------------------ERREUR DU : %s ",datetime());
        fputs("-----------------------\n",fichier);

        fprintf(fichier,"Allocation échouer : %s \n ",endroitAllouer);


        //on ferme le fichier
        fclose(fichier);
    }
    //si l'ouverture échoue
    else
    {
        printf("Erreur ouverture du fichier erreur_allocation.txt");
    }
}

int erreur_chargement_map()
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("map.txt","r");

    //si l'ouverture échoue
    if (fichier == NULL)
    {
        FILE* erreur=NULL;

        //ouverture du fichier
        erreur=fopen("erreur_map.txt","a");

        fputs("Le chargement de la map a échouer. Initialisation d'une map prédéfinie.\n",erreur);

        //on ferme le fichier
        fclose(erreur);

        return 1;
    }

    //si l'ouverture réussi, on indique qu'il y a aucune erreur
    else
    {
        //on ferme le fichier
        fclose(fichier);

        return 0;
    }
}

void supr_fichier_erreur()
{
    FILE* fichier=NULL;

    //ouverture du fichier pour le crée ou simplement l'ouvrir pour le suprimer
    fichier=fopen("erreur_image.txt","a");

    //si l'ouverture réussi
    if (fichier != NULL)
    {
        //on ferme le fichier
        fclose(fichier);

        //on suprime le fichier
        remove("erreur_image.txt");
    }
    //si l'ouverture échoue
    else
    {
        printf("Erreur ouverture du fichier erreur_image.txt");
    }
}

int verif_erreur()
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("erreur_image.txt","r");

    //si l'ouverture réussi
    if (fichier != NULL)
    {
        //on ferme le fichier
        fclose(fichier);

        //le fichier a été créer, nous devons donc terminer le programme proprement
        return 0;
    }
    //si l'ouverture échoue
    else
    {
        //Aucune erreur
        return 1;
    }
}

void test()
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("test.txt","a");

    //si l'ouverture réussi
    if (fichier != NULL)
    {
        fprintf(fichier,"---------------------Test réussi a : %s ",datetime());
        fputs("-----------------------\n",fichier);


        //on ferme le fichier
        fclose(fichier);
    }
    //si l'ouverture échoue
    else
    {
        printf("Erreur ouverture du fichier erreur_image.txt");
    }
}

void test_valeur(double valeur,char information[])
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("test.txt","a");

    //si l'ouverture réussi
    if (fichier != NULL)
    {
        fprintf(fichier,"---------------------Test de -%s- réussi de la valeur : %f ",information,valeur);
        fputs("-----------------------\n",fichier);


        //on ferme le fichier
        fclose(fichier);
    }
    //si l'ouverture échoue
    else
    {
        printf("Erreur ouverture du fichier erreur_image.txt");
    }
}
