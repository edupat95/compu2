CC = gcc
CFLAGS = -g -Wall
LDLIBS = -pthread
TARGETS = proceso

all: $(TARGETS)

#tp2: utils.c gestion_argumentos.c gestion_partes.c gestion_hijos.c
proceso	: proceso.c
	$(CC) $(CFLAGS) $(LDLIBS) -o  $@ $^

test: proceso
		./proceso

clean:
	$(RM) $(TARGETS) *.o *~ *.txt
