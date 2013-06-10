#ifndef CLASSEMENT_H_INCLUDED
#define CLASSEMENT_H_INCLUDED

//regarde si un fichier classement est cr�er, si il n'est pas cr�er alors on le cr�e puis on le remplie
void verif_clmt();
//affiche le classement
void affClmt(SDL_Surface *ecran,TTF_Font *police,int flagCheat);
//regarde si le score rentre dans le top 5, si le score rentre dans le top 5 alors on retourne 1 sinon 0
int newScoreClmt(int score);
//Inserre le nouveau score dans le classement.sav
void insertNewScore(char nomJoueur[],int scoreJoueur);
//Lis le caract�re rentr�, convertie qwerty en azerty et renvois le caract�re, retourne le caract�re 0 si le caract�re est non accepter ou ind�finie
char lectureCara(char *touche);
//Permet de rentrer le nom du joueur et retourne ce nom
char* enterName(int score,TTF_Font *police,SDL_Surface *ecran);

#endif // CLASSEMENT_H_INCLUDED
