#ifndef CLASSEMENT_H_INCLUDED
#define CLASSEMENT_H_INCLUDED

//regarde si un fichier classement est créer, si il n'est pas créer alors on le crée puis on le remplie
void verif_clmt();
//affiche le classement
void affClmt(SDL_Surface *ecran,TTF_Font *police,int flagCheat);
//regarde si le score rentre dans le top 5, si le score rentre dans le top 5 alors on retourne 1 sinon 0
int newScoreClmt(int score);
//Inserre le nouveau score dans le classement.sav
void insertNewScore(char nomJoueur[],int scoreJoueur);
//Lis le caractère rentré, convertie qwerty en azerty et renvois le caractère, retourne le caractère 0 si le caractère est non accepter ou indéfinie
char lectureCara(char *touche);
//Permet de rentrer le nom du joueur et retourne ce nom
char* enterName(int score,TTF_Font *police,SDL_Surface *ecran);

#endif // CLASSEMENT_H_INCLUDED
