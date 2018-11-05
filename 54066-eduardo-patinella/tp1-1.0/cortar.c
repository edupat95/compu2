#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

int tamnio_archivo(){
}

int main (int argc, char * argv []){
	extern char *optarg;
	extern int optind;
	int cantidad_archivos=0,tamanio_entrada=0, i=0,flagi=0,flags=0,flagn=0,flago=0,flagh=0, tamanio=0, cant=0;
	char *archivo_aux, *archivo_entrada, *archivo_salida;
	archivo_salida=(malloc(sizeof(char)));
	archivo_entrada=(malloc(sizeof(char)));
	archivo_aux=(malloc(sizeof(char)));
	char opt;
	char buff[1024];
 	int fdo,fdd,leido;

	while ((opt = getopt(argc, argv, "i:s:n:o:h")) != -1){
		switch (opt){
			case 'i':
			flagi = 1;
			archivo_entrada=optarg;
			break;
			case 's':
			tamanio = atoi(optarg);
			flags = 1;
			break;
			case 'n':
			cant = atoi(optarg);
			flagn=1;
			break;
			case 'o':
			flago = 1;
			archivo_salida=optarg;
			break;
			case 'h':
			flagh = 1;
		    break;
			}
		}



if(flagh == 1){
		printf("forma de uso -s [tamaño] -b n [hasta n argumentos] -h ayuda \n");
}
else if((flagi==1 && (flags==1 || flagn==1) && flago==1)){

				printf("El archivo de entrada es %s \n",archivo_entrada);
				char buffer[1024];
				int leido;
				while((leido = read(fdo,&buffer,sizeof(char))) > 0){
					tamanio_entrada++;
				}
				if (flags==1 && flagn==1){
					perror("Error de ingreso de parametros: \n Funcionamiento: -i [archivo_entrada] (-n [cant_archivos] or -s [tamanio_archivos_byte]) -o [archivo_salida]");
					return 0;
					exit(1);
				}

				fdo = open(archivo_entrada,O_RDONLY,0666); //abrimos el archivo
				if (fdo==-1)
					{
					perror("Error al abrir fichero:");
					exit(1);
					}




				printf ("El archivo de salidas es %s \n",archivo_salida);

				if(flags==1){
					printf("desea partir el archivo en varios archivo de tamanio %d \n",tamanio);
					cantidad_archivos=tamanio_entrada/tamanio;
				}else if(flagn==1){
					printf("desea partir el archivo en %d archivos \n",cant);
					cantidad_archivos=cant;
				}




				strcpy(archivo_aux,archivo_salida);
				for(i=1; i<=cantidad_archivos ;i++){
					sprintf(archivo_aux,"%s%d",archivo_aux,i);
      		fdd = open(archivo_aux, O_CREAT|O_RDWR,0666);
      		leido = read(fdo, buff, tamanio);
      		write(fdd,buff,leido);
      		close(fdd);
					//memset(&archivo_aux,0,largo);
					strcpy(archivo_aux,archivo_salida);
				}


				/*fdd = open("dest2",O_CREAT|O_RDWR,0666);
      	leido = read(fdo, buff, tamanio);
      	write (fdd,buff,leido);
      	close (fdd);
      	fdd = open("dest3", O_CREAT|O_RDWR,0666);
      	leido = read (fdo, buff, tamanio);
      	write (fdd,buff,leido);
      	close(fdd);
				}*/

				close(fdo);

	}else if((flagi==1 && (flags==1 || flagn==1) && flago==0)){
		printf("En este caso no se define flago \n");
		fdo = open(archivo_entrada,O_RDONLY,0666); //abrimos el archivo
		if (fdo==-1)
			{
			perror("Error al abrir fichero:");
			exit(1);
			}
		for(i=1; i<=cantidad_archivos ;i++){
			sprintf(archivo_salida,"%s%d",archivo_salida,i);
			//fdd = open(archivo_salida, O_CREAT|O_RDWR,0666);
			leido = read(fdo, buff, tamanio);
			write(STDOUT_FILENO,buff,leido);
			//memset(&aux,0,strlen(aux));
			//strcpy(aux,archivo_salida);
		}
	}else{
		perror("Los parametros no son correctos \n Funcionamiento: -i [archivo_entrada] (-n [cant] or -s [tamanio]) -o [archivo_salida] \n help only -h parameter");
	}
			return 0;
}
