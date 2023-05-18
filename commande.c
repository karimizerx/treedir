#include "noeud.h"
#include "commande.h"
#include "lecture.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Affiche la liste des fils du noeud 'courant'.
void ls(noeud **courant)
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'ls' (commande.c:13) : Le noeud courant est NULL.");
    }
    liste_noeud *tmp = (*courant)->fils;
    while (tmp != NULL)
    {
        printf("%s ", tmp->no->nom);
        tmp = tmp->succ;
    }
    puts("");
}

// Affiche les informations du noeud 'courant'. Affiche une ERREUR sinon et quitte le programme.
void info(noeud **courant)
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'info' (commande.c:31) : Le noeud courant est NULL.");
    }
    print_noeud(courant);
    puts("");
}

// Crée un dossier de nom 'nom' dans le dossier 'courant'.
void mkdir(noeud **courant, char *nom)
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'mkdir' (commande.c:44) : Le noeud courant est NULL.");
    }
    if (!(*courant)->est_dossier)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'mkdir' (commande.c:50) : Le noeud courant n'est pas un dossier.");
    }
    noeud *n = creer_noeud(courant, true, (*courant)->racine, *courant, nom);
    insert_noeud(courant, n);
}

// Crée un fichier de nom 'nom' dans le dossier 'courant'.
void touch(noeud **courant, char *nom) // Créer un fichier dans le dossier courant.
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'touch' (commande.c:63) : Le noeud courant est NULL.");
    }
    if (!(*courant)->est_dossier)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'touch' (commande.c:68) : Le noeud courant n'est pas un dossier.");
    }
    noeud *n = creer_noeud(courant, false, (*courant)->racine, *courant, nom);
    insert_noeud(courant, n);
}

// Affiche le chemin absolue du noeud 'courant'.
void pwd(noeud **courant)
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'pwd' (commande.c:82) : Le noeud courant est NULL.");
    }
    else
    {
        char *chemin = chemin_absolue(*courant); // On récupère le chemin absolue.
        printf("%s\n", chemin);                  // On l'affiche.
        free(chemin);                            // On libère la zone mémoire allouée temporairement.
    }
}

// Déplace le pointeur du noeud 'courant' au bout du 'chemin'. Affiche une ERREUR sinon et quitte le programme.
void cd(noeud **courant, char *chemin)
{
    noeud *tmp;         // Pointeur vers le noeud 'destination'.
    if (chemin == NULL) // Cas 0 : Le dossier demandé est la racine.
        *courant = (*courant)->racine;
    else
    {
        tmp = search_noeud(*courant, chemin); // On cherche le noeud.
        if (tmp != NULL && tmp->est_dossier)  // Cas 1 : On trouve le noeud demandé & c'est bien un dossier.
            *courant = tmp;
        else
        {
            if (tmp == NULL) // Cas 2.1 : Le chemin n'existe pas.
            {
                free_noeud((*courant)->racine);
                free(courant);
                printf("Erreur dans 'cd' (commande.c:104) : Le chemin '%s' n'existe pas.\n", chemin);
                exit(EXIT_FAILURE); // On arrête donc le programme.
            }
            else // Cas 2.2 : Le chemin renvoie vers un fichier.
            {
                free_noeud((*courant)->racine);
                free(courant);
                printf("Erreur dans 'cd' (commande.c:104) : Le chemin '%s' pointe vers un fichier.\n", chemin);
                exit(EXIT_FAILURE); // On arrête donc le programme.
            }
        }
    }
}

// Affiche l'arborescence à partir du noeud 'courant'.
void print(noeud **courant)
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'print' (commande.c:130) : Le noeud courant est NULL.");
    }
    tree(*courant, 0); // On affiche l'arborescence à partir de courant.
    puts("");
}

// Supprime le noeud 'destination' se trouvant au bout du 'chemin', ainsi que toute son arborescence. Affiche une ERREUR sinon et quitte le programme.
void rm(noeud **courant, char *chemin)
{
    noeud *del = search_noeud(*courant, chemin); // On cherche le noeud pointé par chemin dans le dossier courant.

    if (del == NULL) // Cas 1 : [ERREUR] Il n'y a pas de noeud pointé par ce chemin.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'rm' (commande.c:143) : Aucun noeud pointé par le chemin '%s' .\n", chemin);
        exit(EXIT_FAILURE);
    }
    else // Cas 2 : On trouve le noeud.
    {
        if (del == *courant) // Cas 2.1 : [ERREUR] Le noeud renvoyé est le noeud 'courant'.
        {
            free_noeud((*courant)->racine);
            free(courant);
            printf("Erreur dans 'rm' (commande.c:143) : Le noeud pointé par le chemin '%s' est le noeud courant.\n", chemin);
            exit(EXIT_FAILURE);
        }
        else if (is_parent(*courant, del) == 0) // Cas 2.2 : [ERREUR] Le noeud renvoyé est un parent du noeud courant.
        {
            free_noeud((*courant)->racine);
            free(courant);
            printf("Erreur dans 'rm' (commande.c:143) : Le noeud pointé par le chemin '%s' est un parent du noeud courant.\n", chemin);
            exit(EXIT_FAILURE);
        }
        else // Cas 2.3 : On trouve le noeud et on peut le supprimer.
        {
            liste_noeud *ln = del->pere->fils;
            liste_noeud *l;    // Pointeur vers la structure 'liste_noeud' du noeud à supprimer.
            if (ln->no == del) // Si le noeud a supprimé est le premier fils.
            {
                l = ln;                                  // On garde un pointeur vers cette structure.
                del->pere->fils = del->pere->fils->succ; // On change les liaisons pour isoler le noeud à supprimer.
            }
            else // Sinon, on cherche le noeud à supprimer dans la liste des fils.
            {
                while (ln->succ->no != del)
                    ln = ln->succ;
                l = ln->succ;              // On garde un pointeur vers cette structure.
                ln->succ = ln->succ->succ; // On change les liaisons pour isoler le noeud à supprimer.
            }
            free(l);         // On supprime la strucutre 'list_noeud' du noeud supprimé.
            free_noeud(del); // On supprime le noeud demandé.
        }
    }
}

// Copie le noeud 'src' pointé par 'chemin1' dans le noeud 'destination' pointé par 'chemin2' avec son nouveau nom au bout de 'chemin2'. Affiche une ERREUR sinon et quitte le programme.
void cp(noeud **courant, char *chemin1, char *chemin2)
{
    noeud *n1 = search_noeud(*courant, chemin1);
    if (n1 == NULL) // Cas 1 : [ERREUR] Le chemin a copié n'existe pas.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'cp' (commande.c:193) : Aucun noeud pointé par le chemin '%s' .\n", chemin1);
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
            assert(new_nom != NULL);                       // On vérifie que l'allocation s'est bien passée.
            memcpy(new_nom, chemin2, len2 * sizeof(char)); // On copie le nom.
            *(new_nom + len2) = '\0';                      // On respecte les propriétés des chaines de caractères.
        }
        else // Cas 2.1.2 : On copie dans la racine.
        {
            dest = malloc(2 * sizeof(char));
            assert(dest != NULL); // On vérifie que l'allocation s'est bien passée.
            *(dest) = '/';        // dest pointe vers la racine.
            *(dest + 1) = '\0';   // On respecte les propriétés des chaines de caractères.
            new_nom = malloc(len2 * sizeof(char));
            assert(new_nom != NULL);                                 // On vérifie que l'allocation s'est bien passée.
            memcpy(new_nom, chemin2 + 1, (len2 - 1) * sizeof(char)); // On copie le nom.
            *(new_nom + len2 - 1) = '\0';                            // On respecte les propriétés des chaines de caractères.
        }
    }
    else // Cas 2.2 : On copie dans un autre dossier de l'arborescence.
    {
        dest = malloc((i + 1) * sizeof(char));
        assert(dest != NULL);                    // On vérifie que l'allocation s'est bien passée.
        memcpy(dest, chemin2, i * sizeof(char)); // On copie le chemin de la destination.
        *(dest + i) = '\0';                      // On respecte les propriétés des chaines de caractères.
        new_nom = malloc((len2 - i) * sizeof(char));
        assert(new_nom != NULL);                                         // On vérifie que l'allocation s'est bien passée.
        memcpy(new_nom, chemin2 + i + 1, (len2 - i - 1) * sizeof(char)); // On copie le nom du nouveau noeud.
        *(new_nom + len2 - i - 1) = '\0';                                // On respecte les propriétés des chaines de caractères.
    }

    noeud *n2 = search_noeud(*courant, dest); // On essaye d'atteindre la destination.

    if (n2 == NULL) // Cas 3 : [ERREUR] Le noeud pointé par la destination n'existe pas.
    {
        free_noeud((*courant)->racine);
        free(courant);
        free(new_nom);
        free(dest);
        printf("Erreur dans 'cp' (commande.c:248) : Aucun noeud pointé par le chemin '%s'.\n", dest);
        exit(EXIT_FAILURE);
    }
    else if (!n2->est_dossier) // Cas 4 : [ERREUR] Le noeud pointé par la destination n'est pas un dossier.
    {
        free_noeud((*courant)->racine);
        free(courant);
        free(new_nom);
        free(dest);
        printf("Erreur dans 'cp' (commande.c:248) : Le noeud pointé par le chemin '%s' n'est pas un dossier.\n", dest);
        exit(EXIT_FAILURE);
    }
    else if (is_parent(n2, n1) == 0) // Cas 5 : [ERREUR] Le noeud à copier est parent du noeud destination.
    {
        free_noeud((*courant)->racine);
        free(courant);
        free(new_nom);
        free(dest);
        printf("Erreur dans 'cp' (commande.c:248) : Le noeud à copier '%s' est parent du noeud destination '%s'\n.", n1->nom, new_nom);
        exit(EXIT_FAILURE);
    }
    else if (is_name_fils_exist(n2->fils, new_nom) == 0) // Cas 6 : [ERREUR] Il existe déjà un noeud portant le nom "new_nom" dans la destination pointée par chemin2.
    {
        free_noeud((*courant)->racine);
        free(courant);
        free(new_nom);
        free(dest);
        printf("Erreur dans 'cp' (commande.c:248) : Il existe déjà un noeud portant le nom '%s' parmi les fils du noeud destination.\n", new_nom);
        exit(EXIT_FAILURE);
    }
    else // Cas 7 : On peut copier le noeud.
    {
        noeud *copie = copie_arbre(n1, new_nom);
        insert_noeud(n2, copie);
    }
    free(dest);
    free(new_nom);
}

// Déplace le noeud pointé par le chemin 'src' dans le noeud pointé par le chemin 'dst'.
void mv(noeud **courant, char *src, char *dst)
{
    if (src == NULL || dst == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'mv' (commande.c:296) : Il n'y a pas assez d'arguments.");
    }

    noeud *source = search_noeud(*courant, src);
    if (source == NULL) // Cas 1 : [ERREUR] Le chemin a copié n'existe pas.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'mv' (commande.c:305) : Aucun noeud pointé par le chemin '%s'.\n", src);
        exit(EXIT_FAILURE);
    }

    noeud *destination = search_noeud(*courant, dst);
    if (destination == NULL) // Cas 2 : [ERREUR] Le noeud pointé par la destination n'existe pas.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'mv' (commande.c:314) : Aucun noeud pointé par le chemin '%s'.\n", dst);
        exit(EXIT_FAILURE);
    }
    if (!destination->est_dossier) // Cas 3 : [ERREUR] Le noeud pointé par la destination n'est pas un dossier.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'mv' (commande.c:314) : Le noeud pointé par le chemin '%s' n'est pas un dossier.\n", dst);
        exit(EXIT_FAILURE);
    }
    else if (is_parent(destination, source) == 0) // Cas 4 : [ERREUR] Le noeud à déplacer est parent du noeud destination.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'mv' (commande.c:305) : Le noeud à copier '%s' est parent du noeud destination '%s'\n.", source->nom, destination->nom);
        exit(EXIT_FAILURE);
    }
    else if (is_name_fils_exist(destination->fils, source->nom) == 0) // Cas 5 : [ERREUR] Il existe déjà un noeud portant le nom du noeud à déplacer dans la destination.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'mv' (commande.c:305) : Il existe déjà un noeud portant le nom '%s' parmi les fils du noeud destination.\n", source->nom);
        exit(EXIT_FAILURE);
    }
    else // Cas 7 : On peut déplacer le noeud.
    {
        // On supprime la connection entre le noeud source et son père.
        noeud *src_pere = source->pere;
        liste_noeud *ln = src_pere->fils;
        liste_noeud *l;       // Pointeur vers la structure 'liste_noeud' du noeud à déplacer.
        if (ln->no == source) // Si le noeud a déplacer est le premier fils.
        {
            l = ln;                                // On garde un pointeur vers cette structure.
            src_pere->fils = src_pere->fils->succ; // On change les liaisons pour isoler le noeud à déplacer.
        }
        else // Sinon, on cherche le noeud à déplacer dans la liste des fils.
        {
            while (ln->succ->no != source)
                ln = ln->succ;
            l = ln->succ;              // On garde un pointeur vers cette structure.
            ln->succ = ln->succ->succ; // On change les liaisons pour isoler le noeud à supprimer.
        }
        free(l); // On supprime la strucutre 'list_noeud' du noeud déplacé.

        // On relie le noeud à son nouveau père.
        insert_noeud(destination, source);
    }
}
