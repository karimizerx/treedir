#include "noeud.h"
#include "commandes.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char *nom0 = "";
    noeud *n0 = creer_noeud(true, nom0);
    n0->pere = n0;
    n0->racine = n0;

    puts("pwd(n0)");
    pwd(n0);

    puts("mkdir(n0, 'Cours')");
    noeud *n = mkdir(n0, "Cours");
    printf("%s\n", n->nom);

    return 0;
}
