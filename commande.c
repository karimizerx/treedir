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

noeud *cd(noeud *courant, char *chem)
{
    if (chem == NULL) // Cas 1 : Retour à la racine.
    {
        courant = courant->racine;
        return courant;
    }
    else if (*chem == '\0' || *chem == '.') // Cas 2 : On reste dans le dossier actuel (On est arrivé à la fin du chemin ou ".").
        return courant;
    else if (*chem == '..') // Cas 3 : On remonte au père avec "..".
    {
        courant = courant->pere;
        return courant;
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

        if (search_noeud(courant, chemin) != NULL) // Cas 4.1 : On a trouvé le sous-chmein, on peut donc continuer à chercher.
        {
            courant = search_noeud(courant, chemin);
            return cd(courant, chem + i + 1);
        }
        else // Cas 4.2 : On a pas trouvé de sous-chemin correspondant, donc il n'y en a pas.
        {
            puts("Erreur dans 'cd' : le chemin n'existe pas.");
            exit(EXIT_FAILURE);
        }
    }
}

void pwd(noeud *courant)
{
}