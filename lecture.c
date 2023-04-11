#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "lecture.h"
#include "commande.c"
#include "noeud.c"

char **split(char *ligne)
{
    char *nvligne = trim(ligne);
    // free(ligne);
    char **tmp = malloc(3 * sizeof(char *));
    int nbw = nbwords(nvligne);
    if (nbw > 3)
    {
        quit("too many arguments");
    }
    if (nvligne == NULL)
        quit("error argument is (null)");
    tmp[0] = next(nvligne);
    tmp[1] = nbw >= 2 ? next(nvligne + strlen(tmp[0]) + 1) : NULL;
    tmp[2] = nbw == 3 ? next(nvligne + strlen(tmp[0]) + strlen(tmp[1]) + 1 + 1) : NULL;
    return tmp;
}

void quit(char *message)
{
    printf("%s \n", message);
    exit(EXIT_FAILURE);
}

void execute(noeud *courant, char *command, char *arg1, char *arg2)
{

    if (command == NULL)
    {
        printf("command null");
        exit(EXIT_FAILURE);
    }
    int i = 0;
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
                cp(courant, arg1, arg2);
        }
        else
        {
            quit("command not recognized");
        }
        break;
        case 'm':
            if (command[i + 1] == 'v'){
                if(arg1==NULL||arg2==NULL)
                    quit("too few arguments");
                else mv(courant,arg1,arg2);
            }else if(command[i + 1] != 'k'){
                quit("command not recognized");
            }else if(equals("mkdir",command)){
                if(arg1==NULL)
                    quit("too few arguments");
                else if(arg2!=NULL)
                    quit("too many arguments");
                mkdir(courant,arg1);
            }
            break;
        case 'p':
            if(equals("pwd",command)){
                if(arg1!=NULL||arg2!=NULL)
                    quit("too many arguments");
                else print(courant);
            }else if(equals("print",command)){
                if(arg1!=NULL||arg2!=NULL)
                    quit("too many arguments for pwd");
                else print(courant,0);
            }else 
                quit("command not recognized");
        break;
    case 'r':
        if (command[i + 1] != 'm')
            quit("command not recognized");
        if (arg2 == NULL || arg1 == NULL)
            quit("too few arguments for rm");
        rm(courant, arg1);
        break;
    case 'l':
        if (command[i + 1] != 's')
            quit("command not recognized");
        if (arg2 != NULL || arg1 != NULL)
            quit("too many arguments for ls");
        ls(courant);
        break;
    case 't':
        if (equals(command, "touch"))
        {
            if (arg1 == NULL)
                quit("too few arguments for touch");
            if (arg2 != NULL)
                quit("too many arguments for touch");
            touch(courant, arg1);
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
            return false;
    }
    return true;
}

int nbwords(char *s)
{
    int i = 0;
    int c = 0;
    while (s[i] != '\0')
    {
        if (s[i] == ' ')
            ++i;
        else
        {
            c++;
            while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0')
            {
                ++i;
            }
        }
    }
    return c;
}

char *trim(char *string)
{
    int j = 0;
    for (; (string[j] == ' ' || string[j] == '\t') && string[j] != '\0'; ++j)
        ;
    int i = strlen(string) - 1;
    for (; string[i] == ' ' || string[i] == '\t'; --i)
        ;
    if (i == strlen(string) - 1 && j == 0)
        return string;
    int c = i - j + 1;
    // char tmp[c];
    char *tmp = malloc(sizeof(char) * (i - j) + 1);
    if (tmp == NULL)
        printf("erreur de memoire");
    memcpy(tmp, string + j, i - j);
    tmp[c] = '\0';
    return tmp;
}

char *next(char *w)
{
    if (w == NULL)
        return NULL;
    size_t i = 0;
    while (!(w[i] == ' ' || w[i] == '\0'))
    {
        ++i;
    }
    if (i == strlen(w))
        return w;
    char *t = malloc(sizeof(char) * i + 1);
    if (t == NULL)
        printf("erreur de memoire");
    memcpy(t, w, i);
    t[i] = '\0';
    return t;
}

void read(noeud *courant, char *filename)
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
        printf("erreur de memoire");
    while (fgets(string, 250, flux) != NULL)
    {
        char **tmp = split(string);
        execute(courant, tmp[0], tmp[1], tmp[2]);
        // free(*tmp);
        // free(*(tmp+1));
        // free(*(tmp+2));
        // free(tmp);
    }
    free(string);
    int fin = fclose(flux);
    if (fin != 0)
        perror("erreur de fermuture");
}

char *dupliquer(const char *s)
{
    char *tmp = malloc((strlen(s)) * sizeof(char));
    memcpy(tmp, s, strlen(s));
    return tmp;
}

int main()
{
    read(creer_noeud(true, NULL, NULL, "racine"), "coms.txt");
    return EXIT_SUCCESS;
}