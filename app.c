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
    // puts("Noeud n0 :");
    // print_noeud(n0);
    // puts("\nNoeud n1 :");
    // print_noeud(n1);
    // puts("\nNoeud n2 :");
    // print_noeud(n2);
    // puts("\nNoeud n3 :");
    // print_noeud(n3);
    // puts("\nNoeud n4 :");
    // print_noeud(n4);
    // puts("\nNoeud n5 :");
    // print_noeud(n5);
    // puts("\nNoeud n6 :");
    // print_noeud(n6);
    // puts("\nNoeud n7 :");
    // print_noeud(n7);
    // puts("\nNoeud n8 :");
    // print_noeud(n8);
    // puts("\n");

    // Affichage des chemins absolues de tous les noeuds.
    printf("pwd(n0) : ");
    char *pwdn0 = pwd(n0);
    printf("pwd(n1) : ");
    char *pwdn1 = pwd(n1);
    printf("pwd(n2) : ");
    char *pwdn2 = pwd(n2);
    printf("pwd(n3) : ");
    char *pwdn3 = pwd(n3);
    printf("pwd(n4) : ");
    char *pwdn4 = pwd(n4);
    printf("pwd(n5) : ");
    char *pwdn5 = pwd(n5);
    printf("pwd(n6) : ");
    char *pwdn6 = pwd(n6);
    printf("pwd(n7) : ");
    char *pwdn7 = pwd(n7);
    printf("pwd(n8) : ");
    char *pwdn8 = pwd(n8);
    puts("");

    // Affichage des enfants de chaque noeud.
    printf("ls(n0) : ");
    ls(n0);
    printf("\nls(n1) : ");
    ls(n1);
    printf("\nls(n2) : ");
    ls(n2);
    printf("\nls(n3) : ");
    ls(n3);
    printf("\nls(n4) : ");
    ls(n4);
    printf("\nls(n5) : ");
    ls(n5);
    printf("\nls(n6) : ");
    ls(n6);
    printf("\nls(n7) : ");
    ls(n7);
    printf("\nls(n8) : ");
    ls(n8);
    puts("\n");

    // Ajout d'un dossier/fichier.
    mkdir(n5, "Mickey");
    printf("ls(n5) : ");
    ls(n5);
    puts("");
    touch(n0, "DRAKEN");
    printf("ls(n0) : ");
    ls(n0);
    puts("");
    mkdir(n1, "Baji");
    printf("ls(n1) : ");
    ls(n1);
    puts("");

    // Déplacement dans un autre noeud.
    noeud *courant = n1;
    pwd(courant);
    cd(courant, "ProjetC");
    pwd(courant);

    // On libère toutes les ressources.
    free(pwdn0);
    free(pwdn1);
    free(pwdn2);
    free(pwdn3);
    free(pwdn4);
    free(pwdn5);
    free(pwdn6);
    free(pwdn7);
    free(pwdn8);
    return 0;
}
