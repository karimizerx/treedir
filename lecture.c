#include "noeud.h"
#include "lecture.h"
#include "commande.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// Affiche le message 'msg' et quitte le programme.
void quit(char *msg)
{
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

// Renvoie TRUE si le caractère 'c' est un espace.
bool espace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Supprime les espaces au début et à la fin de la chaine 'str' et renvoie cette nouvelle chaine.
char *del_space(char *str)
{
    char *nv = NULL;
    size_t len = strlen(str);
    size_t deb = 0;
    int fin = len - 1;

    while (espace(str[deb]) && str[deb] != '\0' && deb < len)
        deb++;
    while (fin >= 0 && espace(str[fin]))
        fin--;

    if (fin == -1)
    {
        nv = malloc(sizeof(char));
        assert(nv != NULL); // On vérifie que l'allocation s'est bien passée.
        nv[0] = '\0';
        return nv;
    }

    nv = malloc(sizeof(char) * (fin - deb + 2));
    assert(nv != NULL); // On vérifie que l'allocation s'est bien passée.
    int i;
    for (i = 0; i <= fin - deb; i++)
        nv[i] = str[deb + i];
    nv[i] = '\0';
    return nv;
}

// Renvoie le nombre de mots dans la chaine 'str'.
int nbwords(char *str)
{
    int c = 0; // Compteur du nombre de mots.
    bool mot = false;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!espace(str[i]))
        {
            if (!mot) // Si on ne regarde pas déjà un mot, alors on incrémente le compteur de mots.
            {
                mot = true;
                c++;
            }
        }
        else
            mot = false;
    }
    return c;
}

void split(char *ligne, char **tmp)
{
    char *nvligne = del_space(ligne);
    int nbw = nbwords(nvligne);
    if (nbw > 3)
    {
        quit("Erreur : Arguments trop nombreux.");
    }
    else if (nbw < 1)
    {
        quit("Erreur : Pas assez d'arguments.");
    }
    if (nvligne == NULL)
        quit("Erreur : L'argument est NULL.");

    int i = 0;
    if (nbw >= 0)
    {
        *tmp = next(nvligne);
        i = i + (strlen(*tmp) + 1); // taille du mot + l'espace qui separe le deuxieme mot
    }
    if (nbw >= 2)
    {
        *(tmp + 1) = next(nvligne + i);
        i = i + (strlen(*(tmp + 1)) + 1); // taille du mot + l'espace qui separe le deuxieme mot
    }
    if (nbw == 3)
        *(tmp + 2) = next(nvligne + i);

    free(nvligne);
}

// Renvoie un pointeur vers le prochain mot dans la chaine 'str'.
char *next(char *str)
{
    if (str == NULL) // Si la chaine 'str' est NULL
        return NULL;
    size_t i = 0;
    while (!(espace(str[i]) || str[i] == '\0'))
        ++i;
    char *t = malloc(sizeof(char) * i + 1);
    assert(t != NULL); // On vérifie que l'allocation s'est bien passée.
    if (i == strlen(str))
        return strcpy(t, str);
    memcpy(t, str, i);
    t[i] = '\0';
    return t;
}

// Exécute la commande 'command' sur le noeud 'courant' avec les arguments 'arg1' & 'arg2'.
void execute(noeud **courant, char *command, char *arg1, char *arg2)
{
    int i = 0;
    if (strlen(command) <= 1)
    {
        printf("La commande '%s' est inconnue.\n", command);
        exit(EXIT_FAILURE);
    }

    switch (command[i])
    {
    case 'c':
        if (command[i + 1] == 'd')
        {
            if (arg2 == NULL)
                cd(courant, arg1);
            else
                quit("Erreur : Arguments trop nombreux pour la commande 'cd'.");
        }
        else if (command[i + 1] == 'p')
        {
            if (arg1 == NULL || arg2 == NULL)
                quit("Erreur : Trop peux d'arguments pour la commande 'cp'.");
            else
                cp(*courant, arg1, arg2);
        }
        else
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        break;

    case 'i':
        if (strcmp(command, "info") == 0)
        {
            if (arg1 == NULL && arg2 == NULL)
                info(*courant);
            else
                quit("Erreur : Arguments trop nombreux pour la commande 'info'.");
        }
        else
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        break;

    case 'p':
        if (strcmp("pwd", command) == 0)
        {
            if (arg1 != NULL || arg2 != NULL)
                quit("Erreur : Arguments trop nombreux pour la commande 'pwd'.");
            else
                pwd(*courant);
        }
        else if (strcmp(command, "print") == 0)
        {
            if (arg1 == NULL && arg2 == NULL)
                print(*courant);
            else
                quit("Erreur : Arguments trop nombreux pour la commande 'print'.");
        }
        else
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        break;

    case 'l':
        if (command[i + 1] != 's')
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        if (arg2 != NULL || arg1 != NULL)
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        ls(*courant);
        puts("");
        break;

    case 'm':
        if (command[i + 1] == 'v')
        {
            if (arg1 == NULL || arg2 == NULL)
                quit("Erreur : Trop peux d'arguments pour la commande 'mv'.");
            else
                mv(*courant, arg1, arg2);
        }
        else if (command[i + 1] != 'k')
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        else if (strcmp("mkdir", command) == 0)
        {
            if (arg1 == NULL)
                quit("Erreur : Trop peux d'arguments pour la commande 'mkdir'.");
            else if (arg2 != NULL)
                quit("Erreur : Arguments trop nombreux pour la commande 'mkdir'.");
            mkdir(*courant, arg1);
        }
        break;

    case 'r':
        if (command[i + 1] != 'm')
        {
            printf("La commande '%s' est inconnue.\n", command);
            exit(EXIT_FAILURE);
        }
        if (arg2 != NULL)
            quit("Erreur : Arguments trop nombreux pour la commande 'rm'.");
        if (arg1 == NULL)
            quit("Erreur : Trop peux d'arguments pour la commande 'rm'.");
        rm(*courant, arg1);
        break;

    case 't':
        if (strcmp(command, "touch") == 0)
        {
            if (arg1 == NULL)
                quit("Erreur : Trop peux d'arguments pour la commande 'touch'.");
            if (arg2 != NULL)
                quit("Erreur : Arguments trop nombreux pour la commande 'touch'.");
            touch(*courant, arg1);
        }
        break;

    default:
        printf("La commande '%s' est inconnue.\n", command);
        quit("");
        break;
    }
}

// Lis le fichier de nom 'filename' & lance les exécutions.
void read(noeud **courant, char *filename)
{
    FILE *flux = fopen(filename, "r");
    if (flux == NULL)
    {
        perror("Problème pour ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    // 'c' est le caractère que l'on lit actuellement.
    int r = 0, debut_ligne = 0, dec = 0, c = 0; // 'debut_ligne' indique où commence une ligne.
    char *ligne = NULL;                         // Ligne contenant la commande & les arguments.

    while ((c = fgetc(flux)) != EOF)
    {
        if (c != '\n')
            ++dec;
        else
        {
            r = fseek(flux, debut_ligne, SEEK_SET);
            assert(r == 0);
            ligne = malloc((dec + 2) * sizeof(char));
            fgets(ligne, dec + 2, flux); // On récupère cette ligne.
            assert(ligne != NULL);       // On vérifie que tout s'est bien passé.

            char **list_arg = malloc(3 * sizeof(char *)); // Pointeur vers des chaines de caractères (cmd, argument1, argument2).
            *list_arg = NULL;
            *(list_arg + 1) = NULL;
            *(list_arg + 2) = NULL;
            split(ligne, list_arg);                                        // On remplit les strings de cmd, arg1 & arg2.
            execute(courant, *list_arg, *(list_arg + 1), *(list_arg + 2)); // On exécute cette commande.

            if (ligne != NULL)
                free(ligne);
            free(*list_arg);     // On libère la commande.
            if (*(list_arg + 1)) // S'il y a un argument, on le libère.
                free(*(list_arg + 1));
            if (*(list_arg + 2)) // S'il y a un deuxieme argument, on le libère.
                free(*(list_arg + 2));
            free(list_arg); // On libère le pointeur général.

            debut_ligne = debut_ligne + dec + 1; // On met à jour les variables pour la lecture.
            dec = 0;
        }
    }

    r = fclose(flux); // On ferme le flux.
    if (r != 0)       // On vérifie que tout s'est bien passé.
        perror("Erreur lors de la fermuture du fichier.");
}
