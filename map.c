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

//Si on active le mode designer, on charge de nouvelle blibliothèque et fonction


#include "headers/constantes.h"
#include "headers/map.h"
#include "headers/erreur.h"

#if MODE_DESIGNER==1

#include <io.h>
#include <direct.h>

extern char* strcpy(char* copieDeLaChaine,const char* chaineACopier);

int chargTileAuto(char string[],tilesets *strucTile)
{
    //on met a zero le nombre de Tilesets
    strucTile->nbTilesets=0;

    struct _finddata_t D;
    int done=0;
    int hd;

    //On obtient le chemin actuel où on est
    char path[100];

    _getcwd(path,100);

    //permet d'acceder au dossier
    char nameDir[50]="";
    sprintf(nameDir,"./%s",string);
    if(0==_chdir(nameDir) ){
        hd = _findfirst("*.*",&D);

        if (hd==-1)
            return;

        //on parcour tout le dossier
        while (!done)
        {
            if(D.name[1]!='.' && D.name[0]!='.'){
                //information tilesets
                strcpy(strucTile->infoImage[strucTile->nbTilesets].nom,D.name);
                sprintf(strucTile->infoImage[strucTile->nbTilesets].cheminImage,"%s/%s",string,D.name);
                strucTile->infoImage[strucTile->nbTilesets].image=IMG_Load(D.name);

                strucTile->nbTilesets++;

                done = _findnext(hd,&D);
            }
            else
                done = _findnext(hd,&D);
        }
        //on revient au fichier principal
        _chdir(path);

        return 1;
    }
    else
        return 0;
}

#endif

extern char* strcpy(char* copieDeLaChaine,const char*chaineACopier);

void declaration_tilesets(tilesets *tilesetsMap)
{
    //on initialise le nombre de Tileset
    tilesetsMap->nbTilesets=0;

    //on chargement les image
    strcpy(tilesetsMap->infoImage[0].cheminImage,NOM_FICHIER_MAP_TERRE);
    tilesetsMap->infoImage[0].image=IMG_Load(tilesetsMap->infoImage[0].cheminImage);
    if(tilesetsMap->infoImage[0].image==NULL)
    {
        erreur_image(tilesetsMap->infoImage[0].cheminImage);
    }
    tilesetsMap->nbTilesets++;

    strcpy(tilesetsMap->infoImage[1].cheminImage,NOM_FICHIER_MAP_CIEL);
    tilesetsMap->infoImage[1].image=IMG_Load(tilesetsMap->infoImage[1].cheminImage);
    if(tilesetsMap->infoImage[1].image==NULL)
    {
        erreur_image(tilesetsMap->infoImage[1].cheminImage);
    }
    tilesetsMap->nbTilesets++;

    strcpy(tilesetsMap->infoImage[2].cheminImage,NOM_FICHIER_MAP_ROUGE);
    tilesetsMap->infoImage[2].image=IMG_Load(tilesetsMap->infoImage[2].cheminImage);
    if(tilesetsMap->infoImage[2].image==NULL)
    {
        erreur_image(tilesetsMap->infoImage[2].cheminImage);
    }
    tilesetsMap->nbTilesets++;
}

int** map_remplie(SDL_Surface *ecran,tilesets tilesetMap)
{
    int **map=NULL;
    int n=0,m=0,i=0,j=0;

    //on choisie une surface
    SDL_Surface *map2=NULL;

    #if MODE_DESIGNER==1
    map2=tilesetMap.infoImage[IMAGE1].image;
    #else
    map2=IMG_Load(NOM_FICHIER_MAP_TERRE);
    if(map2==NULL)
        erreur_image(NOM_FICHIER_MAP_TERRE);
    #endif

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

int** chargement_map(SDL_Surface *ecran,tilesets tilesetMap)
{
    if(erreur_chargement_map()==0)
    {
        int **map=NULL;
        int n=0,m=0,i=0,j=0;

        //on choisie une surface
        SDL_Surface *map2=NULL;

        #if MODE_DESIGNER==1
        map2=tilesetMap.infoImage[IMAGE1].image;
        #else
        map2=IMG_Load(NOM_FICHIER_MAP_TERRE);
        if(map2==NULL)
            erreur_image(NOM_FICHIER_MAP_TERRE);
        #endif

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
        return map_remplie(ecran,tilesetMap);
    }

}

void affiche_map(int **map,SDL_Surface *ecran,tilesets imageMap,int y)
{
    int i=0,j=0,n=0,m=0;

    //déclaration des variable
    SDL_Rect position;


    //on définie la largeur/hauteur a blitter celon la fenetre
    n=ecran->h/imageMap.infoImage[0].image->h;
    m=ecran->w/imageMap.infoImage[0].image->w;

    //on blitte celon la map
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(map[i][j+y]<imageMap.nbTilesets)
            {
                position.y=i*(imageMap.infoImage[map[i][j+y]].image->h);
                position.x=j*(imageMap.infoImage[map[i][j+y]].image->w);

                SDL_BlitSurface(imageMap.infoImage[map[i][j+y]].image,NULL,ecran,&position);
            }
            //on blitte le ciel si le numéro de l'image est incorrect
            else
            {
                position.y=i*(imageMap.infoImage[1].image->h);
                position.x=j*(imageMap.infoImage[1].image->w);

                SDL_BlitSurface(imageMap.infoImage[1].image,NULL,ecran,&position);
            }

        }
    }

}

void liberation_tilesets(tilesets *tilesetsMap)
{
    int i=0;
    //on libére les images chargé
    for(i=0;i<(tilesetsMap->nbTilesets);i++)
        SDL_FreeSurface(tilesetsMap->infoImage[i].image);
}

int limite_map(sprite *helico,tilesets *imageMap)
{
    int limite=0,largeurMap=50;

    if(taille_map()!=0)
    {
        largeurMap=taille_map();
    }

    limite=largeurMap-helico->imageUtilise.positionEcran->w/imageMap->infoImage[0].image->w;

    return limite;
}

int hauteur_sol(int** map,tilesets *tilesetsMap,SDL_Surface *ecran,int j)
{
    int i=0,n=0,m=0;
    n=ecran->h/tilesetsMap->infoImage[0].image->h;
    m=ecran->w/tilesetsMap->infoImage[0].image->w;

    //on regarde toute les lignes et on voit si une des lignes contient de la terre
    //j+(n/2)=> le milieu de la map actuellement affiché a l'écran
    for(i=0;(map[i][j+(m/2)]!=0) && (i<(n-1));i++);

    /*Si on arrive a la dernière ligne, pour évité d'acceder a une zone mémoire non allouer par le programme
    on vérifie la dernière valeur de celle-ci. Si elle est différente du tilesets sol, on retourne la valeur
    de la hauteur de la map pour indiquer qu'il n'y a pas de sol*/
    if(i==(n-1) && map[i][j+(m/2)]!=0 )
    {
        i=n;
    }


    i=((ecran->h/tilesetsMap->infoImage[0].image->h)-i)*tilesetsMap->infoImage[0].image->h;

    return i;
}

void decallement_image_map_hauteurPixel(sprite *image,tilesets *tilesetsMap,int hauteurPixel,int j,int positionMap,SDL_Surface *numeroImage)
{
    //une image est placé de façon fixe a un endroit. Cette fonction permet de la déplacé celon le scrolling map
    int n=0,m=0;
    n=image->imageUtilise.positionEcran->h/tilesetsMap->infoImage[0].image->h;
    m=image->imageUtilise.positionEcran->w/tilesetsMap->infoImage[0].image->w;
    SDL_Rect position;

    //Si l'image a blitter se situe dans la fenetre, sinon on fait aucun calcul
    if( (0<((m/2)-(positionMap-j)) ) || ( (m/2)>(j-positionMap) ) )
    {
        //position de base de l'image ( image centré )
        position.x=(m/2)*(tilesetsMap->infoImage[0].image->w);
        position.y=hauteurPixel;

        //Si l'image se situe vers la gauche du centre de la map
        if(positionMap>j)
        {
            //calcul du décallement
            int decallement=(m/2)-(positionMap-j);

            //attribution des coordonnées
            position.x=decallement*(tilesetsMap->infoImage[0].image->w);
        }
        //Sinon si l'image se situe vers la droite du centre de la map
        else if(positionMap<j)
        {
            //calcul du décallement
            int decallement=(j-positionMap)+(m/2);

            //attribution des coordonnées
            position.x=decallement*(tilesetsMap->infoImage[0].image->w);
        }

        //blittage de l'image
        SDL_BlitSurface(numeroImage,NULL,image->imageUtilise.positionEcran,&position);
    }
}

int hauteur_sol_max(int** map,tilesets *tilesetsMap,SDL_Surface *ecran,int positionIni,int positionFin)
{
    int interval=positionFin-positionIni,max=0,i=0;

    for(i=0;i<=interval;i++)
    {
        if( hauteur_sol(map,tilesetsMap,ecran,(positionIni+i) )>max )
        {
            max=hauteur_sol(map,tilesetsMap,ecran,(positionIni+i) );
        }
    }

    return max;
}
