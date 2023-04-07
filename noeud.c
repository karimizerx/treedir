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
    if (ln == NULL)
        return NULL;
    else if (ln->no->nom == nom && !(ln->no->est_dossier))
    {
        printf("Erreur : %s n'est pas un dossier.", nom);
        exit(EXIT_FAILURE);
    }
    else if (ln->no->nom == nom)
        return ln->no;
    else
        return search_noeud_list(ln->succ, nom);
}

noeud *search_noeud(noeud *n, char *nom)
{
    if (n == NULL)
        return NULL;
    else if (n->nom == nom)
        return n;
    else if (search_noeud_list(n->fils, nom) != NULL)
        return search_noeud_list(n->fils, nom);
    else
    {
    }
}

void print_noeud(noeud *node)
{
}