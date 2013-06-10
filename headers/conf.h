#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED

//Affiche les options possibles du jeu. Retourne 1 si le jeu doit restart, sinon retourne 0
int ecranConf(SDL_Surface *ecran,TTF_Font *police,imgMenu buton,imgMenu butonOn);
//retourne une chaine de caractère qui correspond au dimention de l'écran
char* valScreen(int ecranDim);
//retourne la structure de configuration du jeu
conf lec_conf();
//Retourne les dimensions de l'écran celon les flags
void dimEcran(int *largeur,int *hauteur,int *flag);
//Modifie le fichier configue en fonction de la structure de conf
void modif_conf(conf config);

#endif // CONF_H_INCLUDED
