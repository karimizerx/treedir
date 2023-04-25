#include "noeud.h"
#include "commandes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void ls(noeud *courant) // Affiche la liste des fils du noeud courant.
{
    if (courant == NULL)
    {
        puts("[ERREUR] dans 'ls' (ligne 10) : Courant = NULL");
        exit(EXIT_FAILURE);
    }

    liste_noeud *tmp = courant->fils;
    while (tmp != NULL)
    {
        printf("%s ", tmp->no->nom);
        tmp = tmp->succ;
    }
}

void cd(noeud **courant, char *chemin)
{
    noeud *tmp;
    if (chemin == NULL) // Cas 0 : Le dossier demandé est la racine.
        *courant = (*courant)->racine;
    else
    {
        if (*chemin == '/') // Cas 1 : Le chemin est absolue.
        {
            *courant = (*courant)->racine;
            ++chemin;
        }
        tmp = search_noeud(*courant, chemin);
        if (tmp != NULL && tmp->est_dossier) // Cas 2 : On trouve le noeud demandé & c'est bien un dossier.
        {
            *courant = tmp;
        }
        else
        {
            if (tmp == NULL) // Cas 3.1 (ERREUR): Le chemin n'existe pas.
                printf("Erreur dans 'cd' (ligne 39) : Le chemin '%s' n'existe pas.\n", chemin);
            else // Cas 3.2 (ERREUR) : Le chemin renvoie vers un fichier.
            {
                printf("Erreur dans 'cd' (ligne 39) : Le chemin '%s' pointe vers un fichier.\n", chemin);
                exit(EXIT_FAILURE); // On arrête donc le programme.
            }
        }
    }
}

void mkdir(noeud *courant, char *nom) // Créer un dossier dans le dossier courant.
{
    if (courant == NULL)
    {
        puts("[ERREUR] dans 'mkdir' (ligne 58) : Courant = NULL");
        exit(EXIT_FAILURE);
    }
    noeud *n = creer_noeud(true, courant->racine, courant, nom);
    insert_noeud(courant, n);
}

void touch(noeud *courant, char *nom) // Créer un fichier dans le dossier courant.
{
    if (courant == NULL)
    {
        puts("[ERREUR] dans 'touch' (ligne 69) : Courant = NULL");
        exit(EXIT_FAILURE);
    }
    noeud *n = creer_noeud(false, courant->racine, courant, nom);
    insert_noeud(courant, n);
}

void pwd(noeud *courant) // Affiche le chemin absolue du noeud n.
{
    if (courant == NULL) // Cas d'Erreur.
    {
        puts("[ERREUR] dans 'pwd' (ligne 80) : Courant = NULL");
        exit(EXIT_FAILURE);
    }
    else
    {
        char *chemin = chemin_absolue(courant); // On récupère le chemin absolue.
        printf("%s\n", chemin);                 // On l'affiche.
        free(chemin);                           // On libère la zone mémoire allouée temporairement.
    }
}

void print(noeud *courant) // Affiche l'arborescence à partir du noeud courant.
{
    if (courant == NULL)
    {
        puts("[ERREUR] dans 'print' (ligne 95) : Courant = NULL");
        exit(EXIT_FAILURE);
    }
    tree(courant, 0);
}

void tree(noeud *courant, int nbSpace) // Affiche l'arborescance à partir du noeud courant (fonction auxiliaire à print, qui prend en argument le décalage).
{
    // On affiche le nom du noeud.
    if (courant->pere == courant) // Si on est à la racine.
        puts(".");
    else
    {
        int i = 0;
        printf("└");
        while (i != nbSpace) // nbSpace est le nb de "-" à afficher.
        {
            printf("─");
            ++i;
        }
        printf("─ %s", courant->nom); // Puis on affiche le nom du noeud.

        if (courant->est_dossier) // Si c'est un dossier, on ajoute "/".
            printf("/\n");
        else
            puts(""); // Sinon, on passe directement à la ligne.
    }

    if (courant->fils == NULL) // Si le noeud courant n'a pas d'enfant, on arrête le programme.
        return;

    else // Sinon, on affiche chacun de ses fils.
    {
        liste_noeud *ln = courant->fils;
        while (ln != NULL)
        {
            tree(ln->no, nbSpace + 1); // Les enfants sont décalés de +1 par rapport au noeud courant.
            ln = ln->succ;
        }
        return; // On s'arrête.
    }
}