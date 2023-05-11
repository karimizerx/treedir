#include "noeud.h"

extern void quit(char *msg);

extern bool espace(char c);

extern char *trim(char *str);

extern int nbwords(char *str);

extern char *next(char *str);

extern void execute(noeud **courant, char *command, char *arg1, char *arg2);

extern void read(noeud **courant, char *filename);

// extern int getDernierMotIndex(char *);
