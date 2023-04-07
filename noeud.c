#include "noeud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

noeud *creer_noeud(bool b, char *nom)
{
    noeud *node = malloc(sizeof(noeud));
    node->est_dossier = b;
    memcpy(node->nom, nom, 99); // On complète le tableau contenant le nom (avec 99 caractères max).
    node->pere = NULL;
    node->racine = NULL;
    node->fils = NULL;
    return node;
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