#include "noeud.h"
#include <stddef.h>
#include <stdio.h>

extern void quit(char *msg);

extern bool espace(char c);

extern char *del_space(char *str);

extern int nbwords(char *str);

extern char *next(char *str);

extern void execute(noeud **courant, char *command, char *arg1, char *arg2);

extern void read(noeud **courant, char *filename);

extern void split(char *ligne, char **tmp);
