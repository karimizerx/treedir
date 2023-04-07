#include "noeud.h"
#include "commandes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void ls(noeud *courant)
{
    liste_noeud *liste = courant->fils;
    while (liste->no != NULL)
    {
        printf("%s   ", liste->no->nom);
        liste = liste->succ;
    }
    puts("");
}

noeud *cd(noeud *courant, char *chemin)
{
    if (chemin == NULL) // Cas 1 : Le dossier demandé est la racine.
    {
        courant = courant->racine;
        return courant; // On renvoie ce dossier.
    }
    else if (search_noeud(courant, chemin) != NULL && search_noeud(courant, chemin)->est_dossier) // Cas 2 : On trouve le noeud demandé & c'est bien un dossier.
    {
        courant = search_noeud(courant, chemin);
        return courant; // On renvoie ce dossier.
    }
    else
    {
        if (!search_noeud(courant, chemin)->est_dossier) // Cas 3.1 (ERREUR): Le chemin renvoie vers un fichier.
            printf("Erreur dans 'cd' : Le chemin '%s' pointe vers un fichier.\n", *chemin);
        else // Cas 3.2 (ERREUR) : Le chemin n'existe pas.
            printf("Erreur dans 'cd' : Le chemin '%s' n'existe pas.\n", *chemin);
        exit(EXIT_FAILURE); // On arrête donc le programme.
    }
}

void pwd(noeud *courant)
{
}