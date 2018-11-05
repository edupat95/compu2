CC = gcc
CFLAGS = -g -Wall
LDLIBS = -pthread
TARGETS = servidor
all: $(TARGETS)
#tp2: utils.c gestion_argumentos.c gestion_partes.c gestion_hijos.c
server	: servidor.c
	$(CC) $(CFLAGS) $(LDLIBS) -o  $@ $^

clean:
	$(RM) $(TARGETS) *.o *~ *.txt
