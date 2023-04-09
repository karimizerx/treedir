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
    if (chemin == NULL) // Cas 0 : Le dossier demandé est la racine.
    {
        courant = courant->racine;
        return courant; // On renvoie ce dossier.
    }
    else if (*chemin == '/') // Cas 1 : Le chemain est absolue.
    {
        courant = courant->racine;
        ++chemin;
    }
    if (search_noeud(courant, chemin) != NULL && search_noeud(courant, chemin)->est_dossier) // Cas 2 : On trouve le noeud demandé & c'est bien un dossier.
    {
        courant = search_noeud(courant, chemin);
        return courant; // On renvoie ce dossier.
    }
    else
    {
        if (!search_noeud(courant, chemin)->est_dossier) // Cas 3.1 (ERREUR): Le chemin renvoie vers un fichier.
            printf("Erreur dans 'cd' : Le chemin '%s' pointe vers un fichier.\n", chemin);
        else // Cas 3.2 (ERREUR) : Le chemin n'existe pas.
            printf("Erreur dans 'cd' : Le chemin '%s' n'existe pas.\n", chemin);
        exit(EXIT_FAILURE); // On arrête donc le programme.
    }
}

noeud *mkdir(noeud *courant, char *nom) // Créer un dossier dans le dossier courant & renvoie le dossier créé.
{
    noeud *n = creer_noeud(true, nom);
    n->pere = courant;
    n->racine = courant->racine;
    liste_noeud *ln = courant->fils;
    while (ln != NULL)
        ln = ln->succ;
    ln = malloc(sizeof(liste_noeud));
    ln->no = n;
    ln->succ = NULL;
    return n;
}

noeud *touch(noeud *courant, char *nom) // Créer un fichier dans le dossier courant & renvoie le fichier créé.
{
    noeud *n = creer_noeud(false, nom);
    n->pere = courant;
    n->racine = courant->racine;
    liste_noeud *ln = courant->fils;
    while (ln != NULL)
        ln = ln->succ;
    ln = malloc(sizeof(liste_noeud));
    ln->no = n;
    ln->succ = NULL;
    return n;
}

char *pwd(noeud *courant) // Affiche le chemin absolue du noeud n.
{
    char *chemin = chemin_noeud(courant, NULL);
    printf("%s\n", chemin);
    return chemin;
}
