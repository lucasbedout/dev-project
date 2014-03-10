#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

//Initialise les boutons
void iniBouton(imgMenu *buton,imgMenu *butonOn,SDL_Surface *ecran);
//initialise la musique et les effets spéciaux
void iniSound(conf config,Mix_Music **music,systEfSound *sound);

#endif // MAIN_H_INCLUDED
