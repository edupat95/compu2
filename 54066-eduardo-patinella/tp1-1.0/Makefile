CC = gcc
cortar:cortar.c
	$(CC) -o $@ -I/usr/include/mysql  $^ -Wl,-Bsymbolic-functions -rdynamic -L/usr/lib/mysql
