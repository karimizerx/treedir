// Fares
#include "noeud.h"

// renvoi un tableau de dim 3
// extern char **split(char *);

// execute la commande
extern void execute(noeud *,char *, char *, char *);

// renvoi le prochain mot
extern char *next(char *);

// renvoi le nombre de mot par ligne
extern int nbwords(char *);

// lis le fichier
extern void read(noeud *,char *);

// Enleve les espaces au debut et a la fin du char
extern char *trim(char *);

//quitte le programme et affiche un message d'erreur
extern void quit(char* );

//teste si deux strings sont egaux
extern bool equals(char*,char*);

//copie un char[]
extern char * dupliquer(const char * s);