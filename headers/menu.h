#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

//initialise les images de victoire/defaite
void iniResult(imgMenu *vic,imgMenu *def,SDL_Surface *ecran);
//Met en pause le jeu
void menuPause(int* continuer,SDL_Event *even,TTF_Font *police,SDL_Surface *ecran,systEfSound *sound);
//Blitte le buton et renvois 1 si on a cliquer sinon renvois 0
int bouton(imgMenu *buton,imgMenu *butonSouriOn,int cordonneeX,int cordonneeY,SDL_Event even,SDL_Surface *texte);
#endif // MENU_H_INCLUDED
