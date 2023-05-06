#include "noeud.h"
#include "lecture.h"
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

noeud *insert_noeud(noeud *courant, noeud *newfils) // Ajout le noeud "newfils" comme fils du noeud "courant".
{
    if (courant->fils == NULL) // Si le noeud courant n'a pas de fils.
    {
        courant->fils = malloc(sizeof(liste_noeud)); // On crée simplement une structure de liste de noeuds.
        assert(courant->fils != NULL);               // On vérifie que l'allocation s'est bien passée.
        courant->fils->no = newfils;                 // On initialise les valeures de cette liste de noeuds.
        courant->fils->succ = NULL;
    }

    else // Sinon.
    {
        liste_noeud *tmp = courant->fils;
        while (tmp->succ != NULL) // On cherche le dernier fils du noeud courant.
            tmp = tmp->succ;
        // Quand on arrive ici, tmp pointe vers le dernier fils du noeud courant.
        tmp->succ = malloc(sizeof(liste_noeud)); // On crée une structure de liste de noeuds.
        assert(tmp->succ != NULL);               // On vérifie que l'allocation s'est bien passée.
        tmp->succ->no = newfils;                 // On initialise les valeures de cette liste de noeuds.
        tmp->succ->succ = NULL;
    }
    return newfils; // On renvoie le fils ajouté.
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
    char *tmp =trim(chemin);
    if(strlen(tmp)==1 && tmp[0]=='/')
        return n->racine;
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
        char *chem = malloc((i + 1) * sizeof(char));          // On sauvegarde ici le nom du premier dossier.
        assert(chem != NULL);                                 // On vérifie que l'allocation s'est bien passée.
        memcpy(chem, chemin, i);                              // On récupère le nom de ce premier dossier.
        *(chem + i) = '\0';                                   // On respecte les propriétés des chaines de caractères.

        // Si on arrive ici, c'est que chemin + i pointe vers / ou vers la fin du chemin.
        noeud *dossier_suivant = search_noeud_profondeur1(n, chem);
        free(chem); // On libère la zone mémoire allouée temporairement.

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

void free_noeud(noeud *noeud) // Libère toutes les zones allouées par un noeud.
{
    if (noeud->fils != NULL)          // S'il a des fils.
        free_noeud_list(noeud->fils); // On free la liste des fils.
    free(noeud);                      // On libère la zone mémoire allouée par le noeud.
}

void free_noeud_list(liste_noeud *ln) // Libère toutes les zones allouées dans une liste de noeud.
{
    if (ln->succ != NULL)          // S'il y'a d'autres fils.
        free_noeud_list(ln->succ); // On libère ces autres fils.
    free_noeud(ln->no);            // On libère le fils courant.
    free(ln);                      // On libère la structure de liste courante.
}

char *chemin_absolue(noeud *node) // Renvoie le chemin absolue du noeud node.
{
    // Deux variables temporaires pour naviguer dans l'arbre & revenir à la position d'origine si nécessaire.
    noeud n = *node;
    noeud *pn = &n; // Deuxième pointeur vers le noeud node.

    // Etape 1 : Déterminer la longueur de la chaine.
    size_t len = 0;        // Taille de la chaine de caractère du chemin absolue.
    while (pn != pn->pere) // Tant qu'on a pas atteint la racine.
    {
        len = len + strlen(pn->nom) + 1; // On ajoute la taille du nom du dossier + 1 (taille de "/").
        pn = pn->pere;                   // On passe au père.
    }

    // Etape 2 : Créer la chaine.
    if (len == 0) // Cas 1 : On cherche le chaine absolue de la racine.
    {
        char *chemin = malloc((len + 2) * sizeof(char)); // La chaine de cacatères du chemin absolue.
        assert(chemin != NULL);                          // On vérifie que l'allocation s'est bien passée.
        *chemin = '/';                                   // La racine ne contient qu'un "/".
        *(chemin + 1) = '\0';                            // On respecte les propriétés des chaines de caractères.
        return chemin;                                   // On renvoie le chemin.
    }
    else // Cas 2 : Sinon.
    {
        char *chemin = malloc((len + 1) * sizeof(char)); // La chaine de cacatères du chemin absolue.
        assert(chemin != NULL);                          // On vérifie que l'allocation s'est bien passée.
        *(chemin + len) = '\0';                          // On respecte les propriétés des chaines de caractères.

        // Etape 3 : Remplire la chaine.
        pn = &n;                          // On revient à la position d'origine;
        size_t s = len - strlen(pn->nom); // La position à laquelle on doit écrire dans la chaine.
        while (pn != pn->pere)            // Tant qu'on a pas atteint la racine.
        {
            memcpy(chemin + s, pn->nom, strlen(pn->nom)); // On copie le nom du dossier actuel à la bonne position.
            memcpy(chemin + s - 1, "/", 1);               // On le fait précéder d'un "/".
            s = s - 1 - strlen(pn->pere->nom);            // On met à jour les variables de noeud & de positions.
            pn = pn->pere;
        }
        return chemin; // On renvoie le chemin.
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


noeud *copie_arbre(noeud *cop){
    if(cop==NULL) return NULL;
    noeud *tmp=creer_noeud(cop->est_dossier,cop->racine,NULL,cop->nom);
    if(cop->est_dossier){
        tmp->fils=copie_fils(cop->fils,tmp);
    }
    return tmp;
}

liste_noeud* copie_fils(liste_noeud *cop,noeud *pere){
    if(cop==NULL) return NULL;
    liste_noeud* tmp=malloc(sizeof(liste_noeud));
    tmp->no=copie_arbre(cop->no);
    tmp->no->pere=pere;
    tmp->succ=copie_fils(cop->succ,pere);
    return tmp;
}
// on veut savoir si node est un fils(ou fils de fils...) de pere
bool is_subdirectory(noeud *node,noeud *pere){
    if(node==NULL) {
        quit("node est null");
    }
    if(node->pere==NULL) {
        printf("erreur, noeud %s n'a pas de parent",node->nom);
        quit("");
    }
    if(node==pere->racine) return false;//cas ou on arrive a la racine
    if(node->pere==pere) return true;// cas ou on a la meme adresse
    return is_subdirectory(node->pere,pere);
}

