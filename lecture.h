// Fares

//renvoi un tableau de dim 3 
extern char **split(char *ligne); 

//execute la commande 
extern void execute(char* command, char* arg1, char* arg2);

//renvoi le prochain mot
extern char *next(char *ligne);

//lis le fichier
extern void read(char* filename);
