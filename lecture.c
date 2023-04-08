#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "lecture.h"

char **split(char *ligne){
    char *nvligne=trim(ligne);
    // free(ligne);
    char **tmp=malloc(3*sizeof(char*));
    int nbw=nbwords(nvligne);
    if(nbw>3){
        printf("too many arguments \n");
        exit(EXIT_FAILURE);
    }
    tmp[0]= next(nvligne);
    tmp[1]= nbw>=2 ?next(nvligne+strlen(tmp[0])+1):NULL;
    tmp[2]= nbw==3 ?next(nvligne+strlen(tmp[0])+strlen(tmp[1])+1+1):NULL;
    return tmp;
}

void execute(char* command, char* arg1, char* arg2){
    if(command==NULL){
        printf("command null");
        exit(EXIT_FAILURE);
    }
    for(int i=0;command[i]!='\0';i++){
        switch (command[i])
        {
        case 'c':
            if(command[i+1]=='d')
                //commande cd
            break;

        default:
            printf("Commande %s non reconnu", command);
            break;
        }
    }
}



int nbwords(char *s){
    int i=0;
    int c=0;
    while(s[i]!='\0'){
        if(s[i]==' ') ++i;
        else { 
            c++;
            while(s[i]!=' ' && s[i]!='\t' && s[i]!='\0'){
                ++i;
            }
        }
    }
    return c;
}

char* trim(char *string){   
    int j=0;
    for (; (string[j] == ' '||string[j] == '\t') && string[j]!='\0'; ++j);
    int i = strlen(string)-1;
    for (; string[i] == ' '||string[i] == '\t'; --i);
    if(i==strlen(string)-1 && j==0) return string; 
    int c=i-j+1;
    // char tmp[c];
    char *tmp=malloc(sizeof(char)*(i-j)+1);
    if(tmp==NULL) printf("erreur de memoire");
    memcpy(tmp,string+j,i-j);
    tmp[c-1] = '\0';
    return tmp;
}


char *next(char *w){
    if(w==NULL) return NULL ;
    size_t i=0;
    while(!(w[i]==' ' || w[i]=='\0')){
        ++i;
    }
    if(i==strlen(w)) return w;
    char* t=malloc(sizeof(char)*i+1);
    if(t==NULL)printf("erreur de memoire");
    memcpy(t,w,i);
    t[i]='\0';
    return t;
}

void read(char* filename){
    // 200 poour les deux args, 25 pour ma commande et  
    FILE * flux=fopen(filename,"r");
    if(flux ==NULL){
        perror("Probleme ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    //adresse dans la quelle on stock la ligne 
    char * string=malloc(sizeof(char)*225); 
    if(string==NULL)printf("erreur de memoire");
    while(fgets(string,250,flux)!=NULL){
        char** tmp=split(string);
        execute(tmp[0],tmp[1],tmp[2]);
    }
    free(string);
    int fin =fclose(flux);
    if(fin !=0) perror("erreur de fermuture");
}

int main(){
    read("coms.txt");
    return EXIT_SUCCESS;
}