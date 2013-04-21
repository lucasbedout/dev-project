/*
editeur.c
----------

Fait par : Mehdi

Rôle : Permet de creer ces propres map sous fichier : map.txt

Date de dernière modifiction : 10/03/2013

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>

#include "headers/constantes.h"
#include "headers/editeur.h"
#include "headers/map.h"
#include "headers/erreur.h"

void editeur (SDL_Surface *ecran)
{
    //-------------------DECLARATION MAP+TILESETS----------------
    tilesets tilesetsMap;
    int **map=NULL;

    #if MODE_DESIGNER==1
    chargTileAuto("img/Tilesets",&tilesetsMap);
    #else
    declaration_tilesets(&tilesetsMap);
    #endif

    map=declaration_tableau_map(ecran,tilesetsMap);

    int positionMap=0;
    int typeImage=IMAGE1;
    //-----------------------------------------------------------

    //-------------------VARIABLE GLOBAL-------------------------
    SDL_Event even;
    int continuer=verif_erreur();
    //Partie du temps
    temps tempsJeu;
    initilisationTemps(tempsJeu.tempsPrecedent);
    tempsJeu.tempsActuel=SDL_GetTicks(); //temps actuel du jeu
    //partie position
    SDL_Rect positionSouri;
    int positionActu=0,positionMax=0;
    //partie liste
    Liste *liste=initilisation_liste();
    //permet de voir la position de la molette pour attribuer le type d'image
    int molette=0;
    //-----------------------------------------------------------

    while(continuer)
    {
        selection(&continuer,&even,&typeImage,&molette,tilesetsMap.nbTilesets);

        affiche_map_editeur(map,ecran,tilesetsMap,positionActu,liste);
        surface_map (&even,typeImage,tilesetsMap,ecran,&positionSouri);
        map=coller_surface(typeImage,tilesetsMap,map,&positionSouri,&even,liste,&positionActu,ecran);
        positionActu=positionActu+position_map(&even,&positionActu);
        aggrandissement_map(&positionActu,&positionMax,liste);
        save_map(map,ecran,tilesetsMap,&even,liste,positionMax);

        SDL_Flip(ecran);


        tempsJeu.tempsActuel=SDL_GetTicks();
        if((tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0])<(TEMPS_POSE))
        {
            SDL_Delay(TEMPS_POSE-(tempsJeu.tempsActuel-tempsJeu.tempsPrecedent[0]));
        }

    }
    liberation_tilesets(&tilesetsMap);
    suppression(liste);

}

int** declaration_tableau_map(SDL_Surface *ecran,tilesets imageMap)
{
    int** map=NULL;
    int n=0,m=0,i=0;

    n=ecran->h/imageMap.infoImage[0].image->h;
    m=ecran->w/imageMap.infoImage[0].image->w;

    //-------------------ALLOCATION TABLEAU DYNAMIQUE 2D--------------

        //allocation dynamique a 1 dimension
        map=malloc(n*sizeof(int *));
        //si cela echoue, on quitte
        if(map==NULL)
        {
            erreur_allocation("map editeur :Dimention1:");
            exit(0);
        }

        //allocation dynamique a 2 dimensions
        for(i=0;i<n;i++)
        {
           map[i]=malloc(m*sizeof(int));
           //si cela echoue, on quitte
           if(map[i]==NULL)
           {
               erreur_allocation("map editeur :Dimention2:");
               exit(0);
           }
        }

    //---------------------FIN ALLOCATION TABLEAU DYNAMIQUE 2D-----------

    initilisationMap(map,n,m);

    return map;

    for(i=0;i<n;i++)
    {
        free(map[i]);
    }
    free(map);
}

int** initilisationMap(int** map,int n,int m)
{
    int i=0,j=0;

    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            map[i][j]=1;
        }
    }

    return map;
}

void selection(int* continuer,SDL_Event *even,int *typeImage,int *molette,int imageMax)
{
    SDL_PollEvent(even);

    //on retire un tileset car le tableau commence a 0 et pas a 1
    imageMax--;

    switch(even->type)
    {
        case SDL_QUIT:
            *continuer=0;
            break;

        case SDL_KEYDOWN:
                switch(even->key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        *continuer=0;
                        break;
                    case SDLK_KP1:
                    case SDLK_1:
                        *typeImage=IMAGE1;
                        break;
                    case SDLK_KP2:
                    case SDLK_2:
                        *typeImage=IMAGE2;
                        break;
                    case SDLK_KP3:
                    case SDLK_3:
                        *typeImage=IMAGE3;
                        break;
                }
                break;

        case SDL_MOUSEBUTTONDOWN :
            switch(even->button.button)
            {
                case SDL_BUTTON_WHEELDOWN:
                    if( (*molette-1)>=0 )
                    {
                        *molette-=1;
                        *typeImage=*molette;
                    }
                    else
                    {
                        *molette=imageMax;
                        *typeImage=*molette;
                    }
                    //permet de mettre a "NULL" la touche enfoncé
                    even->key.keysym.sym=SDLK_EURO;

                    break;

                case SDL_BUTTON_WHEELUP:
                    if( (*molette+1)<=imageMax )
                    {
                        *molette+=1;
                        *typeImage=*molette;
                    }
                    else
                    {
                        *molette=IMAGE1;
                        *typeImage=*molette;
                    }
                    //permet de mettre a "NULL" la touche enfoncé
                    even->key.keysym.sym=SDLK_EURO;
                    break;

                default:
                    break;
            }
            break;
    }

}

void surface_map (SDL_Event *even,int typeImage,tilesets tilesetsMap,SDL_Surface *ecran,SDL_Rect *position)
{
    //Surface qui suit la sourri
    if(even->type==SDL_MOUSEMOTION)
    {
        position->x=even->motion.x;
        position->y=even->motion.y;
    }

    SDL_BlitSurface(tilesetsMap.infoImage[typeImage].image,NULL,ecran,position);
}

int** coller_surface(int typeImage,tilesets tilesetsMap,int** map,SDL_Rect *position,SDL_Event *even,Liste *liste,int *positionActu,SDL_Surface *ecran)
{
    //Si l'utilisateur a le clique enfoncé droit
    if( (even->type==SDL_MOUSEBUTTONDOWN || even->type==SDL_MOUSEMOTION) && even->button.button==SDL_BUTTON_LEFT)
    {
       int i=0,j=0,m=0,c=0;
       //on blitte la surface en fonction de la map si aucune liste chainnée est présente
       if(*positionActu==0)
       {
            i=position->y/tilesetsMap.infoImage[0].image->h;
            j=position->x/tilesetsMap.infoImage[0].image->w;

            map[i][j]=typeImage;
       }
       //sinon on blitte la surface en prenant en compte les listes chaînées
       else
       {
           i=position->y/tilesetsMap.infoImage[0].image->h;
           j=position->x/tilesetsMap.infoImage[0].image->w;
           m=ecran->w/tilesetsMap.infoImage[0].image->w;

            //Si on blitte encore sur la map, on blitte les surfaces en prenant en compte les listes chainnées sur l'écran
           if( 0>(j+(*positionActu)-m) )
           {
               j=j+(*positionActu);
               map[i][j]=typeImage;

           }
           //Sinon si on blitte en dehors de la map mais sur une liste chaînée
           else
           {
               Element *cursor=liste->premier;
               //On va jusqu'a l'ellement clqiuer
               for(c=0;c!=((j+(*positionActu))-m);c++)
               {
                   cursor=cursor->suivant;
               }
                //Et on lui attribue le type d'image
               cursor->colonne[i]=typeImage;
           }

       }

    }

    return map;
}

void save_map(int** map,SDL_Surface *ecran,tilesets imageMap,SDL_Event *even,Liste* liste,int positionMax)
{
    if(even->type==SDL_KEYDOWN && even->key.keysym.sym==SDLK_s)
    {
         FILE* fichier=NULL;

        //ouverture du fichier en écriture/lecture et écrasement au préalable du fichier
        fichier=fopen("map.txt","w+");

        //si l'ouverture réussi
        if (fichier != NULL)
        {
            int i=0,j=0,n=0,m=0;
            Element* cursor=liste->premier;

            n=ecran->h/imageMap.infoImage[0].image->h;
            m=ecran->w/imageMap.infoImage[0].image->w;

            //parcour de la map
            for(i=0;i<n;i++)
            {
                for(j=0;j<m;j++)
                {
                    fprintf(fichier,"%d",map[i][j]);
                }
                //Si la position max de la map est supérieur a 0
                if(positionMax>0)
                {
                    //Alors on parcour les listes de colonne i
                    for(j=0;j<(positionMax-1);j++)
                    {
                        fprintf(fichier,"%d",cursor->colonne[i]);
                        cursor=cursor->suivant;
                    }
                    //on revient au debus de la liste
                    cursor=liste->premier;
                }

                fputc('\n',fichier);
            }

            //on ferme le fichier
            fclose(fichier);
        }
        //si l'ouverture échoue
        else
        {
            printf("Erreur ouverture du fichier erreur_allocation.txt");
        }
    }
}

Liste* initilisation_liste()
{
    //on crée une liste et un nouvelle ellement
    Liste *liste=malloc(sizeof(Liste));
    Element *element=malloc(sizeof(Element));

    //Si l'allocation échoue, on l'indique
    if(liste==NULL || element==NULL)
    {
        erreur_allocation("Initialisation liste, allocation\n");
    }

    //On initialise le tableau du nouvelle ellement
    initilisation_tab(element->colonne, NB_LIGNE_MAP_HAUTEUR);
    //On indique au nouvelle ellement la fin de la liste en attribuant un NULL pour l'ellement suivant
    element->suivant=NULL;
    /*on attribue a la liste l'ellement premier et dernier, comme il y a un seul ellement dans notre liste,
    le dernier et premier ellement sera le même*/
    liste->premier=element;
    liste->dernier = element;

    return liste;
}

Liste* insertion_liste(Liste *liste)
{
    //on fait une allocation dynamique pour le nouveau ellement
    Element* n = malloc(sizeof(Element));
    Element* cursor;
    //On parcour toute la liste
    for(cursor=liste->premier; cursor != liste->dernier; cursor=cursor->suivant);
    //On inserre le nouvel ellement a la fin de la liste
    cursor->suivant=n;
    liste->dernier=n;

    //Initialisation du tableau
    initilisation_tab(n->colonne, NB_LIGNE_MAP_HAUTEUR);
    //Et on pointe vers NULL pour indiqué la fin de la liste
    n->suivant=NULL;
    return liste;
}

void initilisation_tab(int tab[],int limite)
{
    int i=0;

    //On parcour tout le tableau est on l'initialise a 2 pour montrer que la map est scroller
    for(i=0;i<limite;i++)
    {
        tab[i]=2;
    }
}

double position_map(SDL_Event *even,int* positionActu)
{
    //Si l'utilisateur va vers la droite, on renvoit 1, si il va a gauche on renvoit -1 sinon on renvoit 0
    switch(even->type)
    {

        case SDL_KEYDOWN:
                switch(even->key.keysym.sym)
                {
                    case SDLK_LEFT:
                        if(*positionActu>0)
                        {
                            //permet de mettre a "NULL" la touche enfoncé
                            even->key.keysym.sym=SDLK_EURO;
                           return -1;
                        }
                        else
                        {
                             //permet de mettre a "NULL" la touche enfoncé
                            even->key.keysym.sym=SDLK_EURO;
                            return 0;
                        }
                        break;
                    case SDLK_RIGHT:
                        //permet de mettre a "NULL" la touche enfoncé
                        even->key.keysym.sym=SDLK_EURO;
                        return 1;
                        break;
                    default:
                        return 0;
                        break;
                }
                break;
            default:
                return 0;
                break;
    }
}

void aggrandissement_map(int* positionActu,int* positionMax,Liste *liste)
{
    /*Si la position actuel de la map est plus grande que la position max de la map,
    on aggrandie cette position max et on crée un nouvellement ellement dans la liste.
    Note : positionActu+1 => le +1 est dù a l'initialisation de la liste où on a déjà inserrer un ellement*/
    if( (*positionActu+1)>(*positionMax) )
    {
        insertion_liste(liste);
        *positionMax+=1;
    }
}

void affiche_map_editeur(int **map,SDL_Surface *ecran,tilesets imageMap,int y,Liste *liste)
{
    int i=0,j=0,n=0,m=0;

    //déclaration des variable
    SDL_Rect position;


    //on définie la largeur/hauteur a blitter celon la fenetre
    n=ecran->h/imageMap.infoImage[0].image->h;
    m=ecran->w/imageMap.infoImage[0].image->w;

    //on blitte si on fait appelle a la map
    if(y<m)
    {
        for(i=0;i<n;i++)
        {
            for(j=y;j<m;j++)
            {
                //blitte la surface qui correspond a la partie de la map
                if(map[i][j]<imageMap.nbTilesets)
                {
                    position.y=i*(imageMap.infoImage[map[i][j]].image->h);
                    position.x=(j-y)*(imageMap.infoImage[map[i][j]].image->w);

                    SDL_BlitSurface(imageMap.infoImage[map[i][j]].image,NULL,ecran,&position);
                }
                //on blitte le ciel si le numéro de l'image est incorrect
                else
                {
                    position.y=i*(imageMap.infoImage[1].image->h);
                    position.x=(j-y)*(imageMap.infoImage[1].image->w);

                    SDL_BlitSurface(imageMap.infoImage[1].image,NULL,ecran,&position);
                }

            }
        }
    }

    //Si la position de la map actuel est supérieur a 0 et qu'on appelle les liste chaînée
    if(y>0)
    {
        //On vérifie avant tout si une liste est bien présente
        if(liste==NULL)
        {
            erreur_allocation("Allocation de la liste dans l'affiche map");
        }

        //On pointe sur le premier ellement
        Element *actuel=liste->premier;

        //x parcour la liste, z les colonnes, le décallement correspond au décallement entre la map et les listes
        int x=0,z=0,decallement=0;

        //boucle qui parcour les listes chaînées
        for(x=0;x<y;x++)
        {
            //On regard si les listes a blittés sont dans le cadre de la fenetre
            if(m>=(y-x))
            {
                //On calcul ici le décallement en fonction si on va blitté que la liste chainnée
                if(y>=m)
                {
                    decallement=(m-(y-x));
                }
                //sinon on calcul le décallement en prenons en compte aussi la map
                else
                {
                    decallement=(j-y+x);
                }

                //boucle qui parcour les lignes
                for(i=0;i<n;i++)
                {
                    //boucle qui parcour les colonnes
                    for(z=decallement;z<m;z++)
                    {
                        if(actuel->colonne[i]<imageMap.nbTilesets)
                        {
                            position.y=i*(imageMap.infoImage[actuel->colonne[i]].image->h);
                            position.x=z*(imageMap.infoImage[actuel->colonne[i]].image->w);

                            SDL_BlitSurface(imageMap.infoImage[actuel->colonne[i]].image,NULL,ecran,&position);
                        }
                        //on blitte le ciel si le numéro de l'image est incorrect
                        else
                        {
                            position.y=i*(imageMap.infoImage[1].image->h);
                            position.x=z*(imageMap.infoImage[1].image->w);

                            SDL_BlitSurface(imageMap.infoImage[1].image,NULL,ecran,&position);
                        }


                    }
                }
            }

            //on passe a la liste suivante
            actuel=actuel->suivant;
        }
    }

}

void suppression(Liste *liste)
{
    //Si la liste n'exite pas, on précise l'erreur
    if (liste == NULL)
    {
        erreur_allocation("Supression liste, editeur");
    }

    //si elle exite, on parcour toute la liste et on suprime chaque ellement un par un
    if (liste->premier != NULL)
    {
         Element *aSupprimer=liste->premier;

        do{
            aSupprimer= liste->premier;
            liste->premier = liste->premier->suivant;
            free(aSupprimer);
        }while(liste->premier!=NULL);
    }
}
