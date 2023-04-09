#ifndef NOEUD_H
#define NOEUD_H
#include <stdbool.h>

struct noeud
{
    bool est_dossier;
    char nom[100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};

struct liste_noeud
{
    struct noeud *no;
    struct liste_noeud *succ;
};

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

extern char *reverse_cat(char *, char *);
extern noeud *init_noeud(bool, char *);
noeud *creer_noeud(bool, noeud *, noeud *, char *);
extern noeud *insert_noeud(noeud *, char *);
extern noeud *delete_noeud(noeud *);
extern noeud *search_noeud_list(liste_noeud *, char *);
extern noeud *search_noeud_profondeur1(noeud *, char *);
extern noeud *search_noeud(noeud *, char *);
extern char *chemin_noeud(noeud *, char *);
extern void print_noeud(noeud *);
#endif