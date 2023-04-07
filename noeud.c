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
    if (ln == NULL) // Cas 1 : le dossier n'exite pas.
        return NULL;
    else if (ln->no->nom == nom && !(ln->no->est_dossier)) // Cas 2 : [Erreur] Le nom est un fichier.
    {
        printf("Erreur : %s n'est pas un dossier.", nom);
        exit(EXIT_FAILURE);
    }
    else if (ln->no->nom == nom) // Cas 3 : On trouve le nom.
        return ln->no;
    else
        return search_noeud_list(ln->succ, nom); // Sinon on regarde dans les autres fils.
}

noeud *search_noeud(noeud *n, char *nom)
{
    if (n == NULL) // Cas 1 : Le dossier n'exite pas.
        return NULL;
    else if (n->nom == nom && !(n->est_dossier)) // Cas 2 : [Erreur] Le nom est un fichier.
    {
        printf("Erreur : %s n'est pas un dossier.", nom);
        exit(EXIT_FAILURE);
    }
    else if (n->nom == nom) // Cas 3 : On trouve le dossier.
        return n;
    else if (search_noeud_list(n->fils, nom) != NULL) // Cas 4 : Le dossier est un des fils.
        return search_noeud_list(n->fils, nom);
    else // Le dossier n'est ni le dossier courant, ni parmi les fils.
    {
    }
}

void print_noeud(noeud *node)
{
}