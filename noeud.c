#include "noeud.h"
#include <stdio.h>
#include <stdlib.h>

noeud *creer_noeud(bool b, char *nom)
{
    return NULL;
}

noeud *insert_noeud(noeud *node)
{
    return NULL;
}

noeud *delete_noeud(noeud *node)
{
    return NULL;
}

noeud *search_noeud_list(liste_noeud *ln, char *nom)
{
    if (ln == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;
    else if (ln->no->nom == nom) // Cas 2 : On trouve le dossier/fichier.
        return ln->no;
    else
        return search_noeud_list(ln->succ, nom); // Sinon on regarde dans les autres fils.
}

noeud *search_noeud(noeud *n, char *nom)
{
    if (n == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;
    /*     else if (nom == '/')
            return n->racine; */
    else if (n->nom == nom) // Cas 2 : On trouve le dossier/fichier.
        return n;
    else // Cas 3 : On cherche parmi les fils.
        return search_noeud_list(n->fils, nom);
}

void print_noeud(noeud *node)
{
}