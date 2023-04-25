#include "noeud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

noeud *init_noeud(bool b, char *nom) // Initialise & renvoie un noeud avec son nom & son type.
{
    noeud *node = malloc(sizeof(noeud));
    assert(node != NULL); // On vérifie que l'allocation s'est bien passée.
    node->est_dossier = b;
    memcpy(node->nom, nom, 99); // On complète le tableau contenant le nom (avec 99 caractères max).
    node->pere = NULL;
    node->racine = NULL;
    node->fils = NULL;
    return node;
}

noeud *creer_noeud(bool dossier, noeud *racine, noeud *pere, char *nom) // Crée & renvoie un noeud avec toutes ses valeurs.
{
    noeud *n = init_noeud(dossier, nom);
    n->pere = pere;
    n->racine = racine;
    return n;
}

noeud *insert_fils(noeud *courant, noeud *newfils) // Ajout le noeud "newfils" comme fils du noeud "courant".
{
    if (courant->fils == NULL)
    {
        courant->fils = malloc(sizeof(liste_noeud));
        courant->fils->no = newfils;
        courant->fils->succ = NULL;
    }
    else
    {
        liste_noeud *tmp = courant->fils;
        while (tmp->succ != NULL)
            tmp = tmp->succ;
        // Quand on arrive ici, tmp pointe vers le dernier fils de courant.
        tmp->succ = malloc(sizeof(liste_noeud));
        tmp->succ->no = newfils;
        tmp->succ->succ = NULL;
    }
    return newfils;
}

noeud *delete_noeud(noeud *node)
{
    return NULL;
}

noeud *search_noeud_list(liste_noeud *ln, char *nom) // Cherche un noeud "nom" dans une liste de fils.
{
    if (ln == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;

    else if (strcmp(ln->no->nom, nom) == 0) // Cas 2 : On trouve le dossier/fichier.
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

    else // Cas 3 : On cherche parmi les fils
        return search_noeud_list(n->fils, nom);
}

noeud *search_noeud(noeud *n, char *chemin) // Cherche un noeud au boud du "chem" dans toute l'arborescence.
{

    if (*chemin == '\0') // Cas 1 : On reste dans le dossier actuel (On est arrivé à la fin du chemin ou ".").
        return n;
    else if (*chemin == '.') // Cas 2 : On reste dans le dossier actuel avec "." ou on remonte au père avec "..".
        return (*(chemin + 1) == '.') ? n->pere : n;

    else
    {
        // On récupère le premier dossier dans le chemin.
        unsigned i = 0;
        while (*(chemin + i) != '/' && *(chemin + i) != '\0') // Tant qu'on est pas arrivé à la fin du chemin ou à un autre dossier ('/').
            ++i;                                              // On avance.
        char *chem = malloc((i + 1) * sizeof(char));
        memcpy(chem, chemin, i); // On récupère le nom de ce premier dossier.

        // Si on arrive ici, c'est que chemin + i pointe vers / ou vers la fin du chemin.
        noeud *dossier_suivant = search_noeud_profondeur1(n, chem);
        free(chem);

        if (dossier_suivant != NULL) // Cas 4.1 : On a trouvé le sous-chmein, on peut donc continuer à chercher à partir de ce sous-dossier.
        {
            chemin = *(chemin + i) == '/' ? chemin + i + 1 : chemin + i;
            return search_noeud(dossier_suivant, chemin);
        }
        else // Cas 4.2 : On a pas trouvé de sous-chemin correspondant, donc il n'y en a pas.
            return NULL;
    }
}

void print_noeud(noeud *n) // Affiche les informations concernant un noeud.
{
    printf("Nom : %s\n", n->nom);
    printf("Racine : %s\n", n->racine->nom);
    printf("Pere : %s\n", n->pere->nom);
}

void free_noeud(noeud *noeud)
{
    if (noeud->fils != NULL)
        free_noeud_list(noeud->fils);
    free(noeud);
}

void free_noeud_list(liste_noeud *ln)
{
    if (ln->succ != NULL)
        free_noeud_list(ln->succ);
    free_noeud(ln->no);
    free(ln);
}

char *chemin_absolue(noeud *node) // Renvoie le chemin absolue du noeud node.
{
    // Deux variables temporaires pour naviguer dans l'arbre & revenir à la position d'origine si nécessaire.
    noeud n = *node;
    noeud *pn = &n; // Deuxième pointeur vers le noeud courant.

    // Etape 1 : Déterminer la longueur de la chaine.
    size_t len = 0;        // Taille de la chaine de caractère du chemin absolue.
    while (pn != pn->pere) // Tant qu'on a pas atteint la racine.
    {
        len = len + strlen(pn->nom) + 1; // On ajoute la taille du nom du dossier + 1 (taille de "/").
        pn = pn->pere;                   // On passe au père.
    }

    // Etape 2 : Créer la chaine.
    if (len == 0)
    {
        char *chemin = malloc((len + 2) * sizeof(char)); // La chaine de cacatères du chemin absolue.
        *chemin = '/';
        *(chemin + 1) = '\0'; // On respecte les propriétés des chaines de caractères.
        return chemin;
    }
    else
    {
        char *chemin = malloc((len + 1) * sizeof(char)); // La chaine de cacatères du chemin absolue.
        *(chemin + len) = '\0';                          // On respecte les propriétés des chaines de caractères.

        // Etape 3 : Remplire la chaine.
        pn = &n;                          // On revient à la position d'origine;
        size_t s = len - strlen(pn->nom); // La position à laquelle on doit écrire dans la chaine.
        while (pn != pn->pere)            // Tant qu'on a pas atteint la racine.
        {
            memcpy(chemin + s, pn->nom, strlen(pn->nom)); // On copie le nom du dossier actuel au bon emplacement.
            memcpy(chemin + s - 1, "/", 1);               // On le fait précéder d'un "/".
            s = s - 1 - strlen(pn->pere->nom);            // On met à jour les variables de noeud & de positions.
            pn = pn->pere;
        }
        return chemin;
    }
}

// Fonctions utiles.

char *reverse_cat(char *s1, char *s2) // Effectue une opération de concaténation à l'envers.
{
    size_t len = strlen(s1) + strlen(s2);         // Taille de la nouvelle chaine.
    char *res = malloc((len + 1) * sizeof(char)); // On crée la nouvelle chaine.
    assert(res != NULL);                          // On vérifie que l'allocation s'est bien passée.
    memcpy(res, s2, strlen(s2));                  // On copie la valeure de la chaine s2 au début.
    memcpy(res + strlen(s2), s1, strlen(s1));     // On copie la valeure de la chaine s1 juste après.
    *(res + len) = '\0';                          // On respecte les propriétés des chaines de caractère.
    return res;                                   // On renvoie la chaine finale.
}
