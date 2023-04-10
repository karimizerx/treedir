#include "noeud.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

noeud *init_noeud(bool b, char *nom) // Initialise & renvoie un noeud avec son nom & son type.
{
    noeud *node = malloc(sizeof(noeud));
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

noeud *insert_noeud(noeud *n, char *chemin) // Insère un noeud en prenant un chemain en argument.
{
    /*     printf("insert : n->nom : %s\n", n->nom);
        noeud *tmp = search_noeud(n, chemin);
        printf("insert : n->nom : %s\n", n->nom);
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
                printf("Erreur dans 'insert_noeud' : Le chemin '%s' pointe vers un fichier.\n", chemin);
            else // Cas 3.2 (ERREUR) : Le chemin n'existe pas.
                printf("Erreur dans 'insert_noeud' : Le chemin '%s' n'existe pas.\n", chemin);
            exit(EXIT_FAILURE); // On arrête donc le programme.
        }
        return NULL; */
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
    if (n->pere == n) // Si on arrive à la racine.
    {
        char *tmp = reverse_cat(chemin, "/"); // On ajoute le "/" de la racine.
        chemin = malloc(strlen(tmp) * sizeof(char));
        memcpy(chemin, tmp, strlen(tmp));
        return chemin; // On s'arrête & on renvoie le chemin absolue.
    }
    else
    {
        char *tmp = reverse_cat(chemin, "/");
        chemin = malloc(strlen(tmp) * sizeof(char));
        memcpy(chemin, tmp, strlen(tmp));
        tmp = reverse_cat(chemin, n->nom);
        chemin = malloc(strlen(tmp) * sizeof(char));
        memcpy(chemin, tmp, strlen(tmp));
        free(tmp); // On libère les ressources temporaires.
        return chemin_noeud(n->pere, chemin);
    }
}

void print_noeud(noeud *n) // Affiche les informations concernant un noeud.
{
    printf("Nom : %s\n", n->nom);
    printf("Racine : %s\n", n->racine->nom);
    printf("Pere : %s\n", n->pere->nom);
}

// Fonctions utiles.
char *reverse_cat(char *s1, char *s2) // Effectue une opération de concaténation à l'envers.
{
    size_t len = strlen(s1) + strlen(s2); // Taille de la nouvelle chaine.
    char *tmp = malloc(strlen(s1) * sizeof(char));
    memcpy(tmp, s1, strlen(s1));           // On copie la valeure de la chaine s1.
    s1 = malloc((len + 1) * sizeof(char)); // s1 devient mtn vide.
    memcpy(s1, s2, strlen(s2));            // On y met s2.
    strcat(s1, tmp);                       // Puis on concatène s1 (valeur = s2) & tmp (valeur = s1).
    free(tmp);                             // On libère les ressources temporaires.
    return s1;                             // On renvoie la chaine concaténée.
}