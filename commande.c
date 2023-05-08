#include "noeud.h"
#include "commande.h"
#include "lecture.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void ls(noeud *courant) // Affiche la liste des fils du noeud courant.s
{
    if (courant == NULL)
    {
        puts("[ERREUR] dans 'ls' (ligne 10) : Courant = NULL");
        exit(EXIT_FAILURE);
    }

    liste_noeud *tmp = courant->fils;
    while (tmp != NULL)
    {
        printf(" %s \n", tmp->no->nom);
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

void rm(noeud *courant, char *chemin)
{
    noeud *del = search_noeud(courant, chemin); // On cherche le noeud pointé par chemin dans le dossier courant.

    if (del == NULL) // Cas 1 : [ERREUR] Il n'y a pas de noeud pointé par ce chemin.
    {
        printf("Erreur dans 'rm' (ligne 141) : Aucun noeud pointé par le chemin '%s' .\n", chemin);
        exit(EXIT_FAILURE);
    }
    else // Cas 2 : On trouve le noeud.
    {
        if (del == courant) // Cas 2.1 : [ERREUR] Le noeud renvoyé est le noeud courant.
        {
            printf("Erreur dans 'rm' (ligne 150) : Le noeud pointé par le chemin '%s' est le noeud courant.\n", chemin);
            exit(EXIT_FAILURE);
        }
        else if (is_parent(courant, del) == 0) // Cas 2.2 : [ERREUR] Le noeud renvoyé est un parent du noeud courant.
        {
            printf("Erreur dans 'rm' (ligne 157) : Le noeud pointé par le chemin '%s' est un parent du noeud courant.\n", chemin);
            exit(EXIT_FAILURE);
        }
        else // Cas 2.3 : On trouve le noeud et on peut le supprimer.
        {
            liste_noeud *ln = del->pere->fils;
            if (ln->no == del) // Si le noeud a supprimé est le premier fils.
                del->pere->fils = del->pere->fils->succ;
            else // Sinon, on cherche le noeud à supprimer dans la liste des fils.
            {
                while (ln->succ->no != del)
                    ln = ln->succ;
                ln->succ = ln->succ->succ; // On change les liaisons pour isoler le noeud à supprimer.
            }
            free_noeud(del); // On supprime le noeud demandé.
        }
    }
}

void cp(noeud *courant, char *chemin1, char *chemin2)
{
    noeud *n1 = search_noeud(courant, chemin1);
    if (n1 == NULL) // Cas 1 : [ERREUR] Le chemin a copié n'existe pas.
    {
        printf("Erreur dans 'cp' (ligne 179) : Aucun noeud pointé par le chemin '%s' .\n", chemin1);
        exit(EXIT_FAILURE);
    }

    // Cas 2 : Le chemin1 existe. On va séparer le nom du noeud copié et le chemin où l'on doit le copier.
    size_t len2 = strlen(chemin2);
    unsigned i = len2;
    while (*(chemin2 + i) != '/' && i != 0) // Tant qu'on a pas atteint un dossier ("/") ou le début de "chemin2".
        --i;                                // On avance (en sens inverse).
    // Si on est ici, c'est que *(chemin2 + i) pointe vers "/" ou que l'on a parcouru l'ensemble du "chemin2".

    char *dest;    // "dest" est le chemin du noeud où l'on doit copier n1.
    char *new_nom; // "new_nom" est le nom de la copie de n1.

    if (i == 0) // Cas 2.1 : On copie dans le dossier courant ou la racine.
    {
        if (*(chemin2 + i) != '/') // Cas 2.1.1 : On copie dans le dossier courant.
        {
            dest = malloc(sizeof(char));
            assert(dest != NULL); // On vérifie que l'allocation s'est bien passée.
            *(dest) = '\0';       // dest pointe vers la chaine vide.
            new_nom = malloc((len2 + 1) * sizeof(char));
            assert(new_nom != NULL);        // On vérifie que l'allocation s'est bien passée.
            memcpy(new_nom, chemin2, len2); // On copie le nom.
            *(new_nom + len2) = '\0';       // On respecte les propriétés des chaines de caractères.
        }
        else // Cas 2.1.2 : On copie dans la racine.
        {
            dest = malloc(2 * sizeof(char));
            assert(dest != NULL); // On vérifie que l'allocation s'est bien passée.
            *(dest) = '/';        // dest pointe vers la racine.
            *(dest + 1) = '\0';   // On respecte les propriétés des chaines de caractères.
            new_nom = malloc(len2 * sizeof(char));
            assert(new_nom != NULL);                // On vérifie que l'allocation s'est bien passée.
            memcpy(new_nom, chemin2 + 1, len2 - 1); // On copie le nom.
            *(new_nom + len2 - 1) = '\0';           // On respecte les propriétés des chaines de caractères.
        }
    }
    else // Cas 2.2 : On copie dans un autre dossier de l'arborescence.
    {
        dest = malloc((i + 1) * sizeof(char));
        assert(dest != NULL);     // On vérifie que l'allocation s'est bien passée.
        memcpy(dest, chemin2, i); // On copie le chemin de la destination.
        *(dest + i + 1) = '\0';   // On respecte les propriétés des chaines de caractères.
        new_nom = malloc((len2 - i) * sizeof(char));
        assert(new_nom != NULL);                        // On vérifie que l'allocation s'est bien passée.
        memcpy(new_nom, chemin2 + i + 1, len2 - i - 1); // On copie le nom du nouveau noeud.
        *(new_nom + len2 - i - 1) = '\0';               // On respecte les propriétés des chaines de caractères.
    }

    noeud *n2 = search_noeud(courant, dest); // On essaye d'atteindre la destination.

    if (n2 == NULL) // Cas 3 : [ERREUR] Le noeud pointé par la destination n'existe pas.
    {
        printf("Erreur dans 'cp' (ligne 231) : Aucun noeud pointé par le chemin '%s'.\n", dest);
        exit(EXIT_FAILURE);
    }
    else if (!n2->est_dossier) // Cas 4 : [ERREUR] Le noeud pointé par la destination n'est pas un dossier.
    {
        printf("Erreur dans 'cp' (ligne 231) : Le noeud pointé par le chemin '%s' n'est pas un dossier.\n", dest);
        exit(EXIT_FAILURE);
    }
    else if (is_parent(n2, n1) == 0) // Cas 5 : [ERREUR] Le noeud à copier est parent du noeud destination.
    {
        printf("Erreur dans 'cp' (ligne 231) : Le noeud à copier '%s' est parent du noeud destination '%s'\n.", n1->nom, new_nom);
        exit(EXIT_FAILURE);
    }
    else if (is_name_fils_exist(n2->fils, new_nom) == 0) // Cas 6 : [ERREUR] Il existe déjà un noeud portant le nom "new_nom" dans la destination pointée par chemin2.
    {
        printf("Erreur dans 'cp' (ligne 231) : Il existe déjà un noeud portant le nom '%s' parmi les fils du noeud destination.\n", new_nom);
        exit(EXIT_FAILURE);
    }
    else // Cas 7 : On peut copier le noeud.
    {
        noeud *copie = copie_arbre(n1);
        //*(copie)->nom = "";
        memcpy(copie->nom, new_nom, strlen(new_nom));
        insert_noeud(n2, copie);
    }

    free(dest);
    free(new_nom);
}

// void cp(noeud *courant, char *src, char *dst)
// {
//     int newchem = getDernierMotIndex(dst); // renvoi l'indice a partir du quelle ya le nom du fichier
//     if (newchem == -1)
//         quit("erreur");
//     if (newchem == strlen(dst))
//         quit("il y a un / la fin du chemin source, commande invalide");

//     char *chem;       // chemin vers le dossier ou on copie
//     if (newchem == 0) // le dst est le dossier courant
//         chem = chemin_absolue(courant);
//     else
//     {
//         chem = malloc(sizeof(char) * (newchem));
//         memcpy(chem, dst, newchem + 1);
//     }
//     printf("%s\n", chem);
//     noeud *copier = search_noeud(courant, src);
//     if (copier == NULL)
//     {
//         printf("dossier %s fichier n'existe pas", src);
//         quit("");
//     }

//     noeud *dest = search_noeud(courant, chem);

//     if (dest == NULL)
//         quit("Dossier destination n'existe pas");

//     if (is_parent(copier, dest))
//         quit("destination est parent du dossier courant");

//     if (!dest->est_dossier)
//         quit("la destination n'est pas un dossier");

//     if (search_noeud_profondeur1(dest, dst + newchem) != NULL)
//     {
//         exit(EXIT_FAILURE);
//         quit("fichier de meme nom existant");
//     }
//     noeud *tmp = copie_arbre(copier);
//     memcpy(tmp->nom, dst + newchem, strlen(dst) - newchem);

//     insert_noeud(dest, tmp);
// }

void mv(noeud *courant, char *src, char *dst)
{
    cp(courant, src, dst);
    rm(courant, src);
}

void info(noeud *courant)
{
    print_noeud(courant);
    puts("");
}