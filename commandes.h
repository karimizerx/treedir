#include "noeud.h"

extern void ls(noeud *courant);

extern noeud *cd(noeud *courant, char *chem);

extern void pwd(noeud *courant);

extern void mkdir(noeud *courant, char *nom);

extern void touch(noeud *courant, char *nom);

extern void rm(noeud *courant, char *nom);

extern void cp(noeud *courant, char *chem1, char *chem2);

extern void mv(noeud *courant, char *chem1, char *chem2);

extern void print(noeud *courant);
