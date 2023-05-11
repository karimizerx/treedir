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
char *trim(char *str)
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
    char *nvligne = trim(ligne);
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

// Lis le fichier de nom 'filename'.
void read(noeud **courant, char *filename)
{
    // 200 pour les deux args, 25 pour ma commande et space
    FILE *flux = fopen(filename, "r");
    if (flux == NULL)
    {
        perror("Problème pour ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    char *string = malloc(sizeof(char) * 225); // Adresse dans la quelle on stock la ligne.
    assert(string != NULL);                    // On vérifie que l'allocation s'est bien passé.

    while (fgets(string, 250, flux) != NULL)
    {
        char **tmp = malloc(3 * sizeof(char *));
        *(tmp + 2) = NULL;
        *(tmp + 1) = NULL;
        *tmp = NULL;
        split(string, tmp);
        free(string);
        string = malloc(sizeof(char) * 225);
        execute(courant, *(tmp), *(tmp + 1), *(tmp + 2));
        if (tmp != NULL)
        {
            if (*(tmp + 2) != NULL)
            {
                free(*(tmp + 2));
                *(tmp + 2) = NULL;
            }
            if (*(tmp + 1))
            {
                free(*(tmp + 1));
                *(tmp + 1) = NULL;
            }
            if (*(tmp))
            {
                free(*(tmp));
                *(tmp) = NULL;
            }
            free(tmp);
            tmp = NULL;
        }
    }
    free(string);
    int fin = fclose(flux);
    if (fin != 0)
        perror("Erreur lors de la fermuture du fichier.");
}

void read(noeud **courant, char *filename)
{
    // 200 pour les deux args, 25 pour ma commande et space
    FILE *flux = fopen(filename, "r");
    if (flux == NULL)
    {
        perror("Problème pour ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    // char *ligne = malloc(sizeof(char) * 225); // Adresse dans la quelle on stock la ligne.
    // assert(ligne != NULL); // On vérifie que l'allocation s'est bien passé.
    char *ligne = NULL;
    size_t len = 0;

    while (getline(&ligne, &len, flux) != -1)
    {
        char *cmd = malloc(sizeof(char));
        char *arg1 = malloc(sizeof(char));
        char *arg2 = malloc(sizeof(char));
        char *str = malloc((strlen(ligne) + 1) * sizeof(char));
        strcpy(str, ligne);
        cmd = strtok(str, " \n \t \r ");
        arg1 = strtok(str, " \n \t \r ");
        arg2 = strtok(str, " \n \t \r ");
        free(str);
        free(ligne);

        execute(courant, cmd, arg1, arg2);

        if (cmd != NULL)
            free(cmd);
        if (arg1 != NULL)
            free(arg1);
        if (arg2 != NULL)
            free(arg2);
    }

    free(ligne);
    int fin = fclose(flux);
    if (fin != 0)
        perror("Erreur lors de la fermuture du fichier.");
}

// Renvoie l'indice du dernier "/" dans le chemin 'str'. Renvoie -1 sinon
// int getDernierMotIndex(char *str)
// {
//     if (str == NULL)
//         return -1;

//     int len = strlen(str);
//     size_t i = len - 1;
//     while (*(str + i) != '/' && i > 0)
//         --i;
//     return i;
// }
