CC = gcc
CFLAGS = -g -Wall
DEPS = app.h commande.h noeud.h 
EXEC = app

all : $(EXEC)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< 

app : app.o commande.o noeud.o
	$(CC) $(CFLAGS) -o $@ $^

run :
	./$(EXEC)
	
clean :
	rm -rf $(EXEC) *.o

leak :
	valgrind --leak-check=full ./$(EXEC) 