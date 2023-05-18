#include "noeud.h"
#include "lecture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Initialise & renvoie un noeud avec son 'nom' & son type 'is_dossier'. Affiche une ERREUR sinon et quitte le programme.
noeud *init_noeud(noeud **courant, bool is_dossier, char *nom)
{
    if (!word_check(nom) || strlen(nom) > 100) // On vérifie que le nom respecte bien les critères alpha-numériques.
    {
        free_noeud((*courant)->racine);
        free(courant);
        printf("Erreur dans 'init_noeud' (noeud.c:10) : La nom choisi '%s' n'est pas alpha-numérique.", nom);
        exit(EXIT_FAILURE);
    }
    noeud *node = malloc(sizeof(noeud));
    assert(node != NULL); // On vérifie que l'allocation s'est bien passée.
    node->est_dossier = is_dossier;
    memcpy(node->nom, nom, (strlen(nom) + 1) * sizeof(char)); // On complète le tableau contenant le nom (avec 99 caractères max).
    node->pere = NULL;
    node->racine = NULL;
    node->fils = NULL;
    return node; // On renvoie le noeud créé.
}

// Crée un noeud avec sa 'racine', son 'pere', son 'nom' & son type 'is_dossier'.
noeud *creer_noeud(noeud **courant, bool is_dossier, noeud *racine, noeud *pere, char *nom)
{
    noeud *n = init_noeud(courant, is_dossier, nom);
    n->pere = pere;
    n->racine = racine;
    return n;
}

// Ajoute le noeud 'newfils' comme fils du noeud 'courant'. Renvoie le noeud ajouté ou affiche une ERREUR et quitte le programme.
noeud *insert_noeud(noeud **courant, noeud *newfils)
{
    if (*courant == NULL)
    {
        free_noeud((*courant)->racine);
        free(courant);
        quit("Erreur dans 'insert_noeud' (noeud.c:38) : Le noeud 'courant' n'existe pas. Vous ne pouvez pas y ajouter 'newfils'.");
    }

    if (!(*courant)->est_dossier)
        quit("Erreur dans 'insert_noeud' (noeud.c:41) : Le noeud 'courant' n'est pas un dossier. Vous ne pouvez pas y ajouter 'newfils'.");

    if ((*courant)->fils == NULL) // Si le noeud courant n'a pas de fils.
    {
        (*courant)->fils = malloc(sizeof(liste_noeud)); // On crée simplement une structure de liste de noeuds.
        assert((*courant)->fils != NULL);               // On vérifie que l'allocation s'est bien passée.
        (*courant)->fils->no = newfils;                 // On initialise les valeures de cette liste de noeuds.
        (*courant)->fils->succ = NULL;
    }
    else // Sinon.
    {
        liste_noeud *tmp = (*courant)->fils;
        while (tmp->succ != NULL) // On cherche le dernier fils du noeud courant.
            tmp = tmp->succ;
        // Quand on arrive ici, tmp pointe vers le dernier fils du noeud courant.
        tmp->succ = malloc(sizeof(liste_noeud)); // On crée une structure de liste de noeuds.
        assert(tmp->succ != NULL);               // On vérifie que l'allocation s'est bien passée.
        tmp->succ->no = newfils;                 // On initialise les valeures de cette liste de noeuds.
        tmp->succ->succ = NULL;
        // free(tmp);
    }
    newfils->pere = *courant; // On met à jour le pere du fils inséré (parfois nécessa).
    return newfils;           // On renvoie le fils inséré.
}

// Cherche un noeud 'nom' dans une liste de fils 'ln'. // Renvoie le noeud trouvé ou NULL.
noeud *search_noeud_list(liste_noeud *ln, char *nom)
{
    if (ln == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;

    else if (strcmp(ln->no->nom, nom) == 0) // Cas 2 : On trouve le dossier/fichier.
        return ln->no;

    else
        return search_noeud_list(ln->succ, nom); // Sinon on regarde dans les autres fils.
}

// Cherche un noeud 'nom' dans une profondeur 1 dans le noeud 'courant'.
noeud *search_noeud_profondeur1(noeud *courant, char *nom) // Cherche un noeud "nom" dans une profondeur 1. Renvoie le noeud trouvé ou NULL.
{
    if (courant == NULL) // Cas 1 : Le dossier/fichier n'exite pas.
        return NULL;

    else if (courant->nom == nom) // Cas 2 : On trouve le dossier/fichier.
        return courant;

    else // Cas 3 : On cherche parmi les fils
        return search_noeud_list(courant->fils, nom);
}

// Cherche un noeud au bout du 'chemin' dans toute l'arborescence du noeud 'courant'. Renvoie le noeud trouvé ou NULL.
noeud *search_noeud(noeud *courant, char *chemin)
{
    if (strlen(chemin) == 1 && *chemin == '/') // Cas 0 : chemin = '/'. On renvoie la racine.
        return courant->racine;
    else if (*chemin == '/') // Cas 1 : On va dans la racine.
        return search_noeud(courant->racine, chemin + 1);
    else if (*chemin == '\0') // Cas 2 : On reste dans le dossier actuel. On est arrivé à la fin du chemin.
        return courant;
    else if (*chemin == '.') // Cas 3 : On reste dans le dossier actuel avec "." ou on remonte au père avec "..".
    {
        if (*(chemin + 1) == '\0') // Cas 3.1 : "."
            return courant;
        if (*(chemin + 1) == '/') // Cas 3.2 : ./blabla"
            return search_noeud(courant, chemin + 2);
        if ((*(chemin + 1) == '.')) // Cas 3.3 : ".." ou "../blabla"
        {
            return (*(chemin + 2) == '\0') ? courant->pere : search_noeud(courant->pere, chemin + 3);
        }
    }
    // On récupère le premier dossier dans le chemin.
    unsigned i = 0;
    while (*(chemin + i) != '/' && *(chemin + i) != '\0') // Tant qu'on est pas arrivé à la fin du chemin ou à un autre dossier ('/').
        ++i;                                              // On avance.
    char *chem = malloc((i + 1) * sizeof(char));          // On sauvegarde ici le nom du premier dossier.
    assert(chem != NULL);                                 // On vérifie que l'allocation s'est bien passée.
    memcpy(chem, chemin, i);                              // On récupère le nom de ce premier dossier.
    *(chem + i) = '\0';                                   // On respecte les propriétés des chaines de caractères.

    // Si on arrive ici, c'est que chemin + i pointe vers '/' ou vers la fin du chemin.
    noeud *dossier_suivant = search_noeud_profondeur1(courant, chem);
    free(chem); // On libère la zone mémoire allouée temporairement.

    if (dossier_suivant != NULL) // Cas 4.1 : On a trouvé le sous-chmein, on peut donc continuer à chercher à partir de ce sous-dossier.
    {
        chemin = *(chemin + i) == '/' ? chemin + i + 1 : chemin + i;
        return search_noeud(dossier_suivant, chemin);
    }
    // Cas 4.2 : On a pas trouvé de sous-chemin correspondant, donc il n'y en a pas.
    return NULL;
}

// Affiche les informations du noeud 'courant'. Affiche une ERREUR sinon et quitte le programme.
void print_noeud(noeud *courant)
{
    if (courant == NULL)
        return NULL;
    printf("Nom : %s\n", courant->nom);
    printf("Racine : %s\n", courant->racine->nom);
    printf("Pere : %s\n", courant->pere->nom);
}

// Libère toutes les zones allouées par le noeud 'courant'.
void free_noeud(noeud *courant)
{
    if (courant == NULL)
        return;
    if (courant->fils != NULL)          // S'il a des fils.
        free_noeud_list(courant->fils); // On free la liste des fils.
    free(courant);                      // On libère la zone mémoire allouée par le noeud.
}

// Libère toutes les zones allouées dans une liste de noeud 'ln'.
void free_noeud_list(liste_noeud *ln)
{
    if (ln->succ != NULL)          // S'il y'a d'autres fils.
        free_noeud_list(ln->succ); // On libère ces autres fils.
    free_noeud(ln->no);            // On libère le fils courant.
    free(ln);                      // On libère la structure de liste courante.
}

// Renvoie le chemin absolue du noeud 'courant'.
char *chemin_absolue(noeud *courant)
{
    // Deux variables temporaires pour naviguer dans l'arbre & revenir à la position d'origine si nécessaire.
    noeud n = *courant;
    noeud *pn = &n; // Deuxième pointeur vers le noeud 'courant'.

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

// Copie l'arborescence du noeud 'courant' avec un nouveau 'nom'.
noeud *copie_arbre(noeud **courant, char *nom)
{
    if (*courant == NULL) // Si le noeud 'courant' est NULL, on renvoie NULL.
        return NULL;
    char nvnom[99]; // Le nom du noeud copié.
    size_t len = strlen(nom);
    if (len > 100)
        quit("Le nouveau nom possède trop de caractères (maximum 100).");
    memcpy(nvnom, nom, len); // On copie le nouveau nom du fichier
    nvnom[len] = '\0';       // On respecte les règles des chaines de caractère.

    noeud *copie = creer_noeud(courant, (*courant)->est_dossier, (*courant)->racine, NULL, nvnom); // On crée le nouveau noeud.
    if ((*courant)->est_dossier)
        copie->fils = copie_fils((*courant)->fils, copie); // On copie le reste de l'arborescence si nécessaire.
    return copie;                                          // On renvoie le noeud copié.
}

// Copie les fils (récursivement) du noeud 'courant'. Utile pour "copie_arbre".
liste_noeud *copie_fils(liste_noeud **courant, noeud *pere)
{
    if (*courant == NULL) // Si le noeud est NULL, on renvoie NULL.
        return NULL;
    liste_noeud *ln = malloc(sizeof(liste_noeud));
    assert(ln != NULL); // On vérifie que l'allocation s'est bien passée.
    ln->no = copie_arbre((*courant)->no, (*courant)->no->nom);
    ln->no->pere = pere;
    ln->succ = copie_fils((*courant)->succ, pere);
    return ln;
}

// Affiche l'arborescance à partir du noeud 'courant', avec un 'nbSpace' espaces au début de ligne.
void tree(noeud *courant, int nbSpace)
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

// Renvoie 0 si 'n' est un parent du noeud 'courant'. Renvoie 1 sinon.
int is_parent(noeud *courant, noeud *n)
{
    if (n == courant->racine) // Cas 1 : [VRAI] Si 'n' est la racine, la racine est pere de tous les noeuds.
        return 0;
    if (courant == courant->pere) // Cas 2 : [FAUX] Si courant est la racine & que 'n' ne l'est pas, c'est faux.
        return 1;
    if (courant->pere == n) // Cas 3 : [VRAI] Si 'n' est le pere de courant, c'est vrai.
        return 0;
    else // Cas 4 : On regarde si 'n' est le pere du pere de courant, i.e 'n' est un parent.
        return is_parent(courant->pere, n);
}

// Renvoie TRUE si le caractère 'c' est bien alpha-numérique. Renvoie FALSE sinon.
bool alphanum(char c)
{
    return (c >= 48 && c <= 57) || (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}

// Renvoie TRUE si la chaine 'str' est bien alpha-numérique. Renvoie FALSE sinon.
bool word_check(char *str)
{
    if (str == NULL)
        // quit("Erreur dans 'word_check' (noeud:259) : La chaine à vérifier est nulle.");
        return false;
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
        if (!alphanum(str[i]))
            return false;
    }
    return true;
}

// Renvoie 0 s'il l'un des fils dans 'ln' porte le nom 'nom'. Renvoie 1 sinon.
int is_name_fils_exist(liste_noeud *ln, char *nom)
{
    if (ln == NULL) // Cas 1 : [FAUX] Il n'y a (plus) aucun fils, donc aucun fils ne porte ce nom.
        return 1;
    else if (strcmp(ln->no->nom, nom) == 0) // Cas 2 : [VRAI] Le fils courant porte le nom "nom".
        return 0;
    else // Cas 3 : On regarde dans les autres fils.
        return is_name_fils_exist(ln->succ, nom);
}