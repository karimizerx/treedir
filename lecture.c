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

void split(char *ligne,char **tmp)
{
     char *nvligne = trim(ligne);
    int nbw = nbwords(nvligne);
    if (nbw > 3)
    {
        quit("too many arguments");
    }else if (nbw <1)
    {
        quit("too few arguments");
    }
    if (nvligne == NULL)
        quit("error argument is (null)");
    int i=0;
    if(nbw>=0){
        *tmp = next(nvligne);
        i=+(strlen(*tmp)+1);//taille du mot + l'espace qui separe le deuxieme mot
    }
    if(nbw >= 2){
        *(tmp+1) =next(nvligne + i); 
        i=+(strlen(*(tmp+1))+1);//taille du mot + l'espace qui separe le deuxieme mot
    }
    if(nbw == 3)
        *(tmp+2) =next(nvligne + i); 

    free(nvligne);
}

void quit(char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

void execute(noeud *courant, char *command, char *arg1, char *arg2)
{

    int i = 0;
    switch (command[i])
    {
    case 'c':
        if (command[i + 1] == 'd')
        {
            if (arg2 == NULL)
                cd(&courant, arg1);
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
        
    case 'i':
        if(strcmp(command,"info")){
            if(arg1==NULL && arg2==NULL)
                info(courant);       
            else 
                quit("too many arguments for info");
        }
        else
        {
            quit("command not recognized");
        }
        break;

    case 'p':
        if (strcmp("pwd", command))
        {
            if (arg1 != NULL || arg2 != NULL)
                quit("too many arguments for pwd");
            else
                pwd(courant);
        }
        else if (strcmp("print", command))
        {
            if (arg1 != NULL || arg2 != NULL)
                quit("too many arguments for pwd");
            else
            {    
                print(courant);
                puts("");
            }
        }
        else
            quit("command not recognized");
        break;

    case 'l':
        if (command[i + 1] != 's')
            quit("command not recognized");
        if (arg2 != NULL || arg1 != NULL)
            quit("too many arguments for ls");
        ls(courant);
        puts("");
        break;

    case 'm':
        if (command[i + 1] == 'v')
        {
            if (arg1 == NULL || arg2 == NULL)
                quit("too few arguments for mv");
            else
                mv(courant, arg1, arg2);
        }
        else if (command[i + 1] != 'k')
        {
            quit("command not recognized");
        }
        else if (strcmp("mkdir", command))
        {
            if (arg1 == NULL)
                quit("too few arguments mkdir");
            else if (arg2 != NULL)
                quit("too many arguments for mkdir");
            mkdir(courant, arg1);
        }
        break;

    case 'r':
        if (command[i + 1] != 'm')
            quit("command not recognized");
        if (arg2 != NULL)
            quit("too many arguments for rm");
        if(arg1==NULL)  
            quit("too few arguments for rm");
        rm(courant, arg1);
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

int nbwords(char *str)
{
    int c = 0;
    bool mot = false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace(str[i])) {
            if (!mot) {
                mot = true;
                c++;
            }
        }
        else mot = false;
    }
    return c;
}

char *trim(char *str) {
    size_t len = strlen(str);
    char *nv = NULL;
    size_t deb = 0;
    size_t fin = len - 1;
    while (str[deb] == ' ' || str[deb] == '\t' || str[deb] == '\n')deb++;
    while (fin >= 0 && (str[fin] == ' ' || str[fin] == '\t' || str[fin] == '\n'))fin--;
    nv =malloc(sizeof(char)*(fin-deb+2));
    if (nv == NULL)
        return NULL;
    int i;
    for (i = 0; i <= fin - deb; i++)nv[i] = str[deb + i];
    nv[i] = '\0';
    return nv;
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
    char *t = malloc(sizeof(char) * i + 1);
    if (i == strlen(w))
        return strcpy(t,w);
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
    if (string == NULL){
        printf("erreur de memoire");
        exit(EXIT_FAILURE);
    }
    while (fgets(string, 250, flux) != NULL){
        char **tmp=malloc(3*sizeof(char*));
        *(tmp+2)=NULL;
        *(tmp+1)=NULL;
        *tmp=NULL;
        split(string,tmp);
        free(string);
        string = malloc(sizeof(char) * 225);
        execute(courant, *(tmp), *(tmp+1), *(tmp+2));
        if(tmp!=NULL){
            if(*(tmp+2)!=NULL){
                free(*(tmp+2));
                *(tmp+2)=NULL;
            }
            if(*(tmp+1)){
                free(*(tmp+1));
                *(tmp+1)=NULL;
            }
            if(*(tmp)){
                free(*(tmp));
                *(tmp)=NULL;
            }
            free(tmp);
            tmp=NULL;
        }
    }
    free(string);
    int fin = fclose(flux);
    if (fin != 0)
        perror("erreur de fermuture");
}

int getDernierMotIndex(char *src){
    if(src==NULL) return -1;
    int len=strlen(src);
    size_t i;
    for(i=len-1;src[i]!='/' && i<=0;--i)
        if(!isalnum(src[i])) 
            quit("le nom doit etre alpha-numerique");
    return i;
}