#include "noeud.h"
#include "commandes.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // Création de la racine.
    char *nom0 = "";
    noeud *n0 = init_noeud(true, nom0);
    n0->pere = n0;
    n0->racine = n0;

    // Création des fils de la racine.
    noeud *n1 = creer_noeud(true, n0, n0, "Cours");
    noeud *n2 = creer_noeud(true, n0, n0, "TD");
    noeud *n3 = creer_noeud(true, n0, n0, "TP");
    noeud *n4 = creer_noeud(false, n0, n0, "edt");

    // Set fils de la racine.
    n0->fils = malloc(sizeof(liste_noeud));
    n0->fils->no = n1;
    n0->fils->succ = malloc(sizeof(liste_noeud));
    n0->fils->succ->no = n2;
    n0->fils->succ->succ = malloc(sizeof(liste_noeud));
    n0->fils->succ->succ->no = n3;
    n0->fils->succ->succ->succ = malloc(sizeof(liste_noeud));
    n0->fils->succ->succ->succ->no = n4;
    n0->fils->succ->succ->succ->succ = NULL;

    // Création des fils de n1.
    noeud *n5 = creer_noeud(true, n0, n1, "ProjetC");
    noeud *n6 = creer_noeud(true, n0, n1, "CM");

    // Set fils de n1.
    n1->fils = malloc(sizeof(liste_noeud));
    n1->fils->no = n5;
    n1->fils->succ = malloc(sizeof(liste_noeud));
    n1->fils->succ->no = n6;
    n1->fils->succ->succ = NULL;

    // Création des fils de n2.
    noeud *n7 = creer_noeud(false, n0, n2, "td1");
    noeud *n8 = creer_noeud(false, n0, n2, "td2");

    // Set fils de n2.
    n2->fils = malloc(sizeof(liste_noeud));
    n2->fils->no = n7;
    n2->fils->succ = malloc(sizeof(liste_noeud));
    n2->fils->succ->no = n8;
    n2->fils->succ->succ = NULL;

    // Affichage de chaque noeud.
    puts("Noeud n0 :");
    print_noeud(n0);
    puts("\nNoeud n1 :");
    print_noeud(n1);
    puts("\nNoeud n2 :");
    print_noeud(n2);
    puts("\nNoeud n3 :");
    print_noeud(n3);
    puts("\nNoeud n4 :");
    print_noeud(n4);
    puts("\nNoeud n5 :");
    print_noeud(n5);
    puts("\nNoeud n6 :");
    print_noeud(n6);
    puts("\nNoeud n7 :");
    print_noeud(n7);
    puts("\nNoeud n8 :");
    print_noeud(n8);
    puts("\n");

    // Affichage des chemins absolues de tous les noeuds.
    printf("pwd(n0) : ");
    pwd(n0);
    printf("pwd(n1) : ");
    pwd(n1);
    printf("pwd(n2) : ");
    pwd(n2);
    printf("pwd(n3) : ");
    pwd(n3);
    printf("pwd(n4) : ");
    pwd(n4);
    printf("pwd(n5) : ");
    pwd(n5);
    printf("pwd(n6) : ");
    pwd(n6);
    printf("pwd(n7) : ");
    pwd(n7);
    printf("pwd(n8) : ");
    pwd(n8);

    puts("mkdir(n0, 'Cours')");
    noeud *n = mkdir(n0, "Cours");
    printf("%s\n", n->nom);

    return 0;
}
