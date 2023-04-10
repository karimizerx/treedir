#include "noeud.h"
#include "commandes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void ls(noeud *courant) // Affiche la liste des fils du noeud courant.
{
    liste_noeud *tmp = courant->fils;
    while (tmp != NULL)
    {
        printf("%s ", tmp->no->nom);
        tmp = tmp->succ;
    }
}

noeud *cd(noeud *courant, char *chemin)
{
    if (chemin == NULL) // Cas 0 : Le dossier demandé est la racine.
    {
        courant = courant->racine;
        return courant; // On renvoie ce dossier.
    }
    else if (*chemin == '/') // Cas 1 : Le chemin est absolue.
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
    noeud *n = creer_noeud(true, courant->racine, courant, nom);
    insert_fils(courant, n);
    return n;
}

noeud *touch(noeud *courant, char *nom) // Créer un fichier dans le dossier courant & renvoie le fichier créé.
{
    noeud *n = creer_noeud(false, courant->racine, courant, nom);
    insert_fils(courant, n);
    return n;
}

char *pwd(noeud *courant) // Affiche le chemin absolue du noeud n.
{
    char *tmp = chemin_noeud(courant, NULL); // On récupère la valeure absolue du noeud.
    char *chemin = malloc(strlen(tmp) - 1);
    memcpy(chemin, tmp, strlen(tmp) - 1); // On retire le "/" à la fin.
    free(tmp);                            // On libère les ressources temporaires.
    printf("%s\n", chemin);               // On affiche le chemin absolue.
    return chemin;
}

void print(noeud *courant, int nbSpace)
{
    // On affiche le nom du noeud.
    if (courant->pere == courant)
        puts(".");
    else
    {
        int i = 0;
        printf("└");
        while (i != nbSpace)
        {
            printf("─");
            ++i;
        }
        printf("─ %s", courant->nom);
        if (courant->est_dossier)
            printf("/\n");
        else
            puts("");
    }

    if (courant->fils == NULL)
        return;
    else
    {
        liste_noeud *ln = courant->fils;
        while (ln != NULL)
        {
            print(ln->no, nbSpace + 1);
            ln = ln->succ;
        }
        return;
    }
}