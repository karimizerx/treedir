#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lecture.h"
#include "commande.h"
#include "noeud.h"

void split(char *ligne, char **tmp)
{
    char *nvligne = trim(ligne);
    int nbw = nbwords(nvligne);
    if (nbw > 3)
    {
        quit("too many arguments");
    }
    else if (nbw < 1)
    {
        quit("too few arguments");
    }
    if (nvligne == NULL)
        quit("error argument is (null)");
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

void quit(char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

void execute(noeud **courant, char *command, char *arg1, char *arg2)
{
    int i = 0;
    if (strlen(command) <= 1)
        quit("command not recognized");
    switch (command[i])
    {
    case 'c':
        if (command[i + 1] == 'd')
        {
            if (arg2 == NULL)
                cd(courant, arg1);
            else
                quit("too many arguments for cd");
        }
        else if (command[i + 1] == 'p')
        {
            if (arg1 == NULL || arg2 == NULL)
                quit("too few arguments for cp");
            else
                cp(*courant, arg1, arg2);
        }
        else
        {
            quit("command not recognized");
        }
        break;

    case 'i':
        if (equals(command, "info"))
        {
            if (arg1 == NULL && arg2 == NULL)
                info(*courant);
            else
                quit("too many arguments for info");
        }
        else
        {
            quit("command not recognized");
        }
        break;

    case 'p':
        if (equals("pwd", command))
        {
            if (arg1 != NULL || arg2 != NULL)
                quit("too many arguments for pwd");
            else
                pwd(*courant);
        }
        else if (equals(command, "print"))
        {
            if (arg1 == NULL && arg2 == NULL)
                print(*courant);
            else
                quit("too many arguments for print");
        }
        else
            quit("command not recognized");
        break;

    case 'l':
        if (command[i + 1] != 's')
            quit("command not recognized");
        if (arg2 != NULL || arg1 != NULL)
            quit("too many arguments for ls");
        ls(*courant);
        puts("");
        break;

    case 'm':
        if (command[i + 1] == 'v')
        {
            if (arg1 == NULL || arg2 == NULL)
                quit("too few arguments for mv");
            else
                mv(*courant, arg1, arg2);
        }
        else if (command[i + 1] != 'k')
        {
            quit("command not recognized");
        }
        else if (equals("mkdir", command))
        {
            if (arg1 == NULL)
                quit("too few arguments mkdir");
            else if (arg2 != NULL)
                quit("too many arguments for mkdir");
            mkdir(*courant, arg1);
        }
        break;

    case 'r':
        if (command[i + 1] != 'm')
            quit("command not recognized");
        if (arg2 != NULL)
            quit("too many arguments for rm");
        if (arg1 == NULL)
            quit("too few arguments for rm");
        rm(*courant, arg1);
        break;

    case 't':
        if (equals(command, "touch"))
        {
            if (arg1 == NULL)
                quit("too few arguments for touch");
            if (arg2 != NULL)
                quit("too many arguments for touch");
            touch(*courant, arg1);
        }
        else if (strcmp("tree", command) == 0)
        {
            if (arg1 != NULL || arg2 != NULL)
                quit("too many arguments for pwd");
            else
            {
                tree(*courant, 0);
                puts("");
            }
        }
        break;

    default:
        printf("Commande %s non reconnu", command);
        quit("");
        break;
    }
}

bool equals(char *a, char *b)
{
    if (strlen(a) != strlen(b))
        return false;
    for (size_t i = 0; i < strlen(a); ++i)
    {
        if (*(a + i) != *(b + i))
        {
            puts("faux chef");
            return false;
        }
    }
    return true;
}

int nbwords(char *str)
{
    int c = 0;
    bool mot = false;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!espace(str[i]))
        {
            if (!mot)
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
        nv = malloc(sizeof(char *));
        nv[0] = '\0';
        return nv;
    }
    nv = malloc(sizeof(char) * (fin - deb + 2));
    if (nv == NULL)
        return NULL;
    int i;
    for (i = 0; i <= fin - deb; i++)
        nv[i] = str[deb + i];
    nv[i] = '\0';
    return nv;
}

char *next(char *w)
{
    if (w == NULL)
        return NULL;
    size_t i = 0;
    while (!(espace(w[i]) || w[i] == '\0'))
    {
        ++i;
    }
    char *t = malloc(sizeof(char) * i + 1);
    if (i == strlen(w))
        return strcpy(t, w);
    if (t == NULL)
        printf("erreur de memoire");
    memcpy(t, w, i);
    t[i] = '\0';
    return t;
}

void read(noeud **courant, char *filename)
{
    // 200 pour les deux args, 25 pour ma commande et space
    FILE *flux = fopen(filename, "r");
    if (flux == NULL)
    {
        perror("Probleme ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    // adresse dans la quelle on stock la ligne

    char *string = malloc(sizeof(char) * 225);
    if (string == NULL)
    {
        printf("erreur de memoire");
        exit(EXIT_FAILURE);
    }
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
        perror("erreur de fermuture");
}

// renvoie l'indice du dernier / du path
int getDernierMotIndex(char *src)
{
    if (src == NULL)
        return -1;
    int len = strlen(src);
    int i = len - 1;
    for (; src[i] != '/' && i > 0; i--)
    {
        printf("i= %d, val %d , pour %d", i, isalnum(src[i]), src[i]);
        puts("");
        // if(isalnum(src[i])==0)
        // {
        //     quit("le nom doit etre alpha-numerique");
        // }
    }
    printf("%d\n", i);
    return i;
}

bool espace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}