#include "noeud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

noeud *creer_noeud(bool b, char *nom) // Créer un noeud.
{
    noeud *node = malloc(sizeof(noeud));
    node->est_dossier = b;
    memcpy(node->nom, nom, 99); // On complète le tableau contenant le nom (avec 99 caractères max).
    node->pere = NULL;
    node->racine = NULL;
    node->fils = NULL;
    return node;
}

noeud *insert_noeud(noeud *n, char *chemin) // Insère un noeud en prenant un chemain en argument.
{
    noeud *tmp = search_noeud(n, chemin);
    if (tmp != NULL && tmp->est_dossier)
    {
        n->pere = tmp;
        n->racine = tmp->racine;
        liste_noeud *ln = tmp->fils;
        while (ln != NULL)
            ln = ln->succ;
        ln = malloc(sizeof(liste_noeud));
        ln->no = n;
        ln->succ = NULL;
    }
    else
    {
        if (!tmp->est_dossier) // Cas 3.1 (ERREUR): Le chemin renvoie vers un fichier.
            printf("Erreur dans 'insert_noeud' : Le chemin '%d' pointe vers un fichier.\n", *chemin);
        else // Cas 3.2 (ERREUR) : Le chemin n'existe pas.
            printf("Erreur dans 'insert_noeud' : Le chemin '%d' n'existe pas.\n", *chemin);
        exit(EXIT_FAILURE); // On arrête donc le programme.
    }
    return NULL;
}

noeud *delete_noeud(noeud *node)
{
    return NULL;
}

noeud *search_noeud_list(liste_noeud *ln, char *nom) // Cherche un noeud "nom" dans une liste de fils.
{
    if (ln == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;
    else if (ln->no->nom == nom) // Cas 2 : On trouve le dossier/fichier.
        return ln->no;
    else
        return search_noeud_list(ln->succ, nom); // Sinon on regarde dans les autres fils.
}

noeud *search_noeud_profondeur1(noeud *n, char *nom) // Cherche un noeud "nom" dans une profondeur 1.
{
    if (n == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;
    else if (n->nom == nom) // Cas 2 : On trouve le dossier/fichier.
        return n;
    else // Cas 3 : On cherche parmi les fils.
        return search_noeud_list(n->fils, nom);
}

noeud *search_noeud(noeud *n, char *chem) // Cherche un noeud au boud du "chem" dans toute l'arborescence.
{
    if (*chem == '\0') // Cas 1 : On reste dans le dossier actuel (On est arrivé à la fin du chemin ou ".").
        return n;
    else if (*chem == '.') // Cas 2 : On reste dans le dossier actuel avec "." ou on remonte au père avec "..".
    {
        return (*(chem + 1) == '.') ? n->pere : n;
    }
    else
    {
        // On récupère le premier dossier dans le chemin.
        char *chemin = malloc(sizeof(char));
        unsigned i = 0;
        while (*(chem + i) != '/' && *(chem + i) != '\0') // Tant qu'on est pas arrivé à la fin du chemin ou à un autre dossier ("/")
        {
            *chemin += *(chem + i);
            ++i;
        }

        // Si on arrive ici, c'est que chem + i pointe vers / ou vers la fin.

        if (search_noeud_profondeur1(n, chemin) != NULL) // Cas 4.1 : On a trouvé le sous-chmein, on peut donc continuer à chercher.
        {
            return search_noeud(search_noeud_profondeur1(n, chemin), chemin);
        }
        else // Cas 4.2 : On a pas trouvé de sous-chemin correspondant, donc il n'y en a pas.
        {
            return NULL;
        }
    }
}

char *chemin_noeud(noeud *n, char *chemin) // Prend un pointeur vers une chaine & écrit dans cette chaine le chemain absolue du noeud n.
{
    if (chemin == NULL)
        chemin = malloc(sizeof(char));
    if (n->pere == n->racine)
    {
        *chemin = '/' + *chemin;
        return chemin;
    }
    else
    {
        *chemin = *(n->nom) + '/' + *chemin;
        return chemin_noeud(n->pere, chemin);
    }
}

void print_noeud(noeud *node)
{
}