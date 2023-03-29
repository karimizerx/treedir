#include <commandes.h>
#include <noeud.c>



void ls(noeud courant){
    liste_noeud *liste =courant.fils;
    while (liste->no != NULL){
        printf("%s   ",liste->no->nom);
        liste=liste->succ;
    }
    printf("\n");
}

void cd(noeud courant){
    courant=*(courant.racine);
}

