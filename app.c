#include "noeud.h"
#include "commande.h"
#include "lecture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
    if(argc!=2)
        return EXIT_FAILURE;
    puts("\n");
    char *nom0 = "";                      // Nom du noeud racine.
    noeud **n0 = malloc(sizeof(noeud *)); // On crée un pointeur vers le noeud racine.
    *n0 = init_noeud(n0, true, nom0);     // On crée le noeud racine.
    (*n0)->pere = *n0;
    (*n0)->racine = *n0;
    read(n0, argv[1]);      // On lit le fichier contenant la liste des commandes & on exécute celles-ci.
    free_noeud((*n0)->racine); // On libère la mémoire alouée.
    free(n0);
    puts("Exécution terminée.");
    return EXIT_SUCCESS;
}
