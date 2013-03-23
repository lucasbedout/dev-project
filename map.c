/*
map.c
----------

Fait par : Mehdi

Rôle : Contient les fonctions de déclaration des tilesets et de l'affichage. Tout ce qui concerne la map ainsi que le scrolling.

Date de dernière modifiction : 04/03/2013

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/map.h"
#include "headers/erreur.h"

void declaration_tilesets(tilesets *tilesetsMap)
{
    //on chargement les image
    tilesetsMap->image[0]=IMG_Load(NOM_FICHIER_MAP_TERRE);
    if(tilesetsMap->image[0]==NULL)
    {
        erreur_image(NOM_FICHIER_MAP_TERRE);
    }
    tilesetsMap->image[1]=IMG_Load(NOM_FICHIER_MAP_CIEL);
    if(tilesetsMap->image[1]==NULL)
    {
        erreur_image(NOM_FICHIER_MAP_CIEL);
    }
    tilesetsMap->image[2]=IMG_Load(NOM_FICHIER_MAP_ROUGE);
    if(tilesetsMap->image[2]==NULL)
    {
        erreur_image(NOM_FICHIER_MAP_ROUGE);
    }
}

int** map_remplie(SDL_Surface *ecran)
{
    int **map=NULL;
    int n=0,m=0,i=0,j=0;

    //on choisie une surface
    SDL_Surface *map2=NULL;

    map2=IMG_Load(NOM_FICHIER_MAP_TERRE);
    if(map2==NULL)
    {
        erreur_image(NOM_FICHIER_MAP_TERRE);
    }

    //on met en place la hauteur/largeur de la map
    n=ecran->h/map2->h;
    m=50;


    //-------------------ALLOCATION TABLEAU DYNAMIQUE 2D--------------

    //allocation dynamique a 1 dimension
    map=malloc(n*sizeof(int *));
    //si cela echoue, on quitte
    if(map==NULL)
    {
        erreur_allocation("map remplissage auto :Dimention1:");
        exit(0);
    }

    //allocation dynamique a 2 dimensions
    for(i=0;i<n;i++)
    {
       map[i]=malloc(m*sizeof(int));
       //si cela echoue, on quitte
       if(map[i]==NULL)
       {
           erreur_allocation("map remplissage auto :Dimention2:");
           exit(0);
       }
    }

    //---------------------FIN ALLOCATION TABLEAU DYNAMIQUE 2D-----------

    //attribution de la map
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(i>(3.25*n/4))
            {
                map[i][j]=0;
            }
            else if(j*(map2->w)>(ecran->w))
            {
                map[i][j]=2;
            }
            else
            {
                map[i][j]=1;
            }
        }
    }

    //on libére l'image de test
    SDL_FreeSurface(map2);

    return map;

    //libération de la mémoire allouer
    for(i=0;i<n;i++)
    {
        free(map[i]);
    }
    free(map);


}

int taille_map()
{
    FILE* fichier=NULL;

    //ouverture du fichier
    fichier=fopen("map.txt","r");

    //si l'ouverture échoue
    if (fichier == NULL)
    {
        return 0;
    }

    //si l'ouverture réussi, on indique qu'il y a aucune erreur
    else
    {
        char caractere=NULL;
        int i=0;

        do
        {
            caractere=fgetc(fichier);
            i++;
        }while(caractere!='\n');
        i--;

        //on ferme le fichier
        fclose(fichier);

        return i;
    }
}

int** chargement_map(SDL_Surface *ecran)
{
    if(erreur_chargement_map()==0)
    {
        int **map=NULL;
        int n=0,m=0,i=0,j=0;

        //on choisie une surface
        SDL_Surface *map2=NULL;

        map2=IMG_Load(NOM_FICHIER_MAP_TERRE);
        if(map2==NULL)
        {
            erreur_image(NOM_FICHIER_MAP_TERRE);
        }

        //on met en place la hauteur/largeur de la map
        n=ecran->h/map2->h;
        m=taille_map();

        //-------------------ALLOCATION TABLEAU DYNAMIQUE 2D--------------

        //allocation dynamique a 1 dimension
        map=malloc(n*sizeof(int *));
        //si cela echoue, on quitte
        if(map==NULL)
        {
            erreur_allocation("map chargement :Dimention1:");
            exit(0);
        }

        //allocation dynamique a 2 dimensions
        for(i=0;i<n;i++)
        {
           map[i]=malloc(m*sizeof(int));
           //si cela echoue, on quitte
           if(map[i]==NULL)
           {
               erreur_allocation("map chargement :Dimention2:");
               exit(0);
           }
        }

        //---------------------FIN ALLOCATION TABLEAU DYNAMIQUE 2D-----------

        //chargement de la map
        FILE* fichierMap=NULL;
        fichierMap=fopen("map.txt","r");

        const int ascii=48;
        int caractereAscii=0;

        for(i=0;i<n;i++)
        {
            for(j=0;j<m;j++)
            {
                caractereAscii=fgetc(fichierMap);
                if(caractereAscii!=13 && caractereAscii!=10)
                {
                    map[i][j]=caractereAscii-ascii;
                }
                else
                {
                    j--;
                }
            }
        }

        //on libére l'image de test
        SDL_FreeSurface(map2);

        return map;

        //libération de la mémoir allouer
        for(i=0;i<n;i++)
        {
            free(map[i]);
        }
        free(map);
    }

    else
    {
        return map_remplie(ecran);
    }

}

void affiche_map(int **map,SDL_Surface *ecran,tilesets imageMap,int y)
{
    int i=0,j=0,n=0,m=0;

    //déclaration des variable
    SDL_Rect position;


    //on définie la largeur/hauteur a blitter celon la fenetre
    n=ecran->h/imageMap.image[0]->h;
    m=ecran->w/imageMap.image[0]->w;

    //on blitte celon la map
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(map[i][j+y]==0)
            {
                position.y=i*(imageMap.image[0]->h);
                position.x=j*(imageMap.image[0]->w);

                SDL_BlitSurface(imageMap.image[0],NULL,ecran,&position);
            }
            else if(map[i][j+y]==1)
            {
                position.y=i*(imageMap.image[1]->h);
                position.x=j*(imageMap.image[1]->w);

                SDL_BlitSurface(imageMap.image[1],NULL,ecran,&position);
            }
            else if(map[i][j+y]==2)
            {
                position.y=i*(imageMap.image[2]->h);
                position.x=j*(imageMap.image[2]->w);

                SDL_BlitSurface(imageMap.image[2],NULL,ecran,&position);
            }

            //on blitte le ciel si le numéro de l'image est incorrect
            else
            {
                position.y=i*(imageMap.image[1]->h);
                position.x=j*(imageMap.image[1]->w);

                SDL_BlitSurface(imageMap.image[1],NULL,ecran,&position);
            }

        }
    }

}

void liberation_tilesets(tilesets *tilesetsMap)
{
    int i=0;
    const int nombreImage=3;
    //on libére les images chargé
    for(i=0;i<(nombreImage);i++)
    {SDL_FreeSurface(tilesetsMap->image[0]);}
}

int limite_map(sprite *helico,tilesets *imageMap)
{
    int limite=0,largeurMap=50;

    if(taille_map()!=0)
    {
        largeurMap=taille_map();
    }

    limite=largeurMap-helico->imageUtilise.positionEcran->w/imageMap->image[0]->w;

    return limite;
}

int hauteur_sol(int** map,tilesets *tilesetsMap,SDL_Surface *ecran,int j)
{
    int i=0,n=0;
    n=ecran->h/tilesetsMap->image[0]->h;

    //on regarde toute les lignes et on voit si une des lignes contient de la terre
    //j+(n/2)=> le milieu de la map actuellement affiché a l'écran
    for(i=0;(map[i][j+(n/2)]!=0) && (i<(n-1));i++);

    /*Si on arrive a la dernière ligne, pour évité d'acceder a une zone mémoire non allouer par le programme
    on vérifie la dernière valeur de celle-ci. Si elle est différente du tilesets sol, on retourne la valeur
    de la hauteur de la map pour indiquer qu'il n'y a pas de sol*/
    if(i==(n-1) && map[i][j+(n/2)]!=0 )
    {
        i=n;
    }


    i=((ecran->h/tilesetsMap->image[0]->h)-i)*tilesetsMap->image[0]->h;

    return i;
}

void decallement_image_map(sprite *image,tilesets *tilesetsMap,int i,int j,int positionMap,SDL_Surface *numeroImage)
{
    //code ici
    //une image est placé de façon fixe a un endroit. Cette fonction permet de la déplacé celon le scrolling map
    int n=0,m=0;
    n=image->imageUtilise.positionEcran->h/tilesetsMap->image[0]->h;
    m=image->imageUtilise.positionEcran->w/tilesetsMap->image[0]->w;
    SDL_Rect position;

    //Si l'image a blitter se situe dans la fenetre, sinon on fait aucun calcul
    if( (0<((m/2)-(positionMap-j)) ) || ( (m/2)>(j-positionMap) ) )
    {
        //position de base de l'image ( image centré )
        position.x=(m/2)*(tilesetsMap->image[0]->w);
        position.y=i*(tilesetsMap->image[0]->h);

        //Si l'image se situe vers la gauche du centre de la map
        if(positionMap>j)
        {
            //calcul du décallement
            int decallement=(m/2)-(positionMap-j);

            //attribution des coordonnées
            position.x=decallement*(tilesetsMap->image[0]->w);
            position.y=i*(tilesetsMap->image[0]->h);
        }
        //Sinon si l'image se situe vers la droite du centre de la map
        else if(positionMap<j)
        {
            //calcul du décallement
            int decallement=(j-positionMap)+(m/2);

            //attribution des coordonnées
            position.x=decallement*(tilesetsMap->image[0]->w);
            position.y=i*(tilesetsMap->image[0]->h);
        }

        //blittage de l'image
        SDL_BlitSurface(numeroImage,NULL,image->imageUtilise.positionEcran,&position);
    }
}
