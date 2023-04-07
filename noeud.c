#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

struct noeud;

struct liste_noeud;

struct noeud
{
    bool est_dossier;
    char nom[100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};
struct liste_noeud
{
    struct noeud *no;
    struct liste_noeud *succ;
};
typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

int main(int argc, char *argv[])
{
    if (argc != 2)
        return EXIT_FAILURE;
    noeud actuel;
    // sscanf(fichier);
}
