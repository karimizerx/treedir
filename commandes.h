#include <stdbool.h>

void ls(noeud courant);

void cd(noeud courant, char *chem);

void cd(noeud courant);

void pwd(noeud courant);

void mkdir(noeud courant, char *nom);

void touch(noeud courant, char *nom);

void rm(noeud courant, char *nom);

void cp(noeud courant, char *chem1, char *chem2);

void mv(noeud courant, char *chem1, char *chem2);

void print(noeud courant);

bool verif(noeud courant);
