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

// Signatures des fonctions.
extern noeud *init_noeud(noeud **courant, bool is_dossier, char *nom);

extern noeud *creer_noeud(noeud **courant, bool is_dossier, noeud *racine, noeud *pere, char *nom);

extern noeud *insert_noeud(noeud **courant, noeud *n);

extern noeud *search_noeud_list(liste_noeud *ln, char *nom);

extern noeud *search_noeud_profondeur1(noeud *courant, char *nom);

extern noeud *search_noeud(noeud *courant, char *chemin);

extern void print_noeud(noeud *courant);

extern void free_noeud(noeud *courant);

extern void free_noeud_list(liste_noeud *ln);

extern char *chemin_absolue(noeud *courant);

extern noeud *copie_arbre(noeud **courant, char *nom);

extern liste_noeud *copie_fils(liste_noeud *courant, noeud *nom);

extern void tree(noeud *courant, int nbSpace);

extern int is_parent(noeud *courant, noeud *n);

extern bool alphanum(char c);

extern bool word_check(char *str);

extern int is_name_fils_exist(liste_noeud *ln, char *nom);
#endif