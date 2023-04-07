#include <commandes.h>
#include <noeud.c>

void ls(noeud courant)
{
    liste_noeud *liste = courant.fils;
    while (liste->no != NULL)
    {
        printf("%s   ", liste->no->nom);
        liste = liste->succ;
    }
    printf("\n");
}

void cd(noeud courant)
{
    courant = *(courant.racine);
}

bool dansArb(noeud courant, noeud racine)
{
    if (&racine == NULL)
        return false;
    if (equal(courant, (racine)))
        return true;
    liste_noeud *tmp = racine.fils;
    while (tmp->succ == NULL)
    {
        if (equal(courant, *(tmp->no)) || dansArb(courant, *(tmp->no)))
            return true;
        tmp = tmp->succ;
    }
    return false;
}

noeud *dansList(noeud courant, liste_noeud tete)
{
    if (tete.no == NULL)
        return NULL;
    if (equal(courant, *(tete.no)))
        return (tete.no);
    return dansList(courant, *(tete.succ));
}

bool equal(noeud o1, noeud o2)
{
    return o1.est_dossier == o2.est_dossier && o1.pere == o2.pere && o1.racine == o2.racine && o1.fils == o2.fils && equals(o1.nom, o2.nom);
}

bool equals(char *a, char *b)
{
    if (strlen(a) != strlen(b))
        return false;
    for (size_t i = 0; i < strlen(a); ++i)
    {
        if (*(a + i) != *(b + i))
            return false;
    }
    return true;
}