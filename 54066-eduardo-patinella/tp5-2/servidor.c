#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>

/*struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};*/
char *armar_protocolo(int error,char *tipo_archivo,int tamanio){
	char *part1=NULL;
	int n=0,n1=0;
	if(error==1){
		part1="HTTP/1.1 404 ERROR\n";
		n=19;
	} else {
		part1="HTTP/1.1 200 OK\n";
		n=16;
	}
	char stamanio[100];
	sprintf(stamanio,"%d",tamanio);
	n1=strlen(stamanio);
	int tam_tipo=strlen(tipo_archivo);
	int tam_protocolo=n+14+tam_tipo+n1+17;
	char protocolo[tam_protocolo];
	//itoa(tamanio, stamanio, 10);
	char part2[]="Content-Type: ";
	char part3[]="\nContent-Length: ";
	strcpy(protocolo,part1);
	strcat(protocolo,part2);
	strcat(protocolo,"text/");
	strcat(protocolo,tipo_archivo);
	strcat(protocolo,part3);
	strcat(protocolo,stamanio);
	char *ptr;
	ptr=protocolo;
	//HTTP/1.1 200 OK
	//Content-Type: text/html
	//Content-Length: 1221
	//write(fds,"HTTP/1.1 200 OK\n",16);
	return ptr;
	//return protocolo;
}
char *buscar_droot(char buff[1024]){
	char *error="NO DROOT";
	char *ptr=NULL;
	ptr = strtok( buff, "=: <\n;\t,>\0." );    // Primera llamada => Primer token
	if(strcmp(ptr,"droot")==0){
		//printf("%s",ptr);
		if ((ptr = strtok( NULL, "=: <\n;\t,>\0." )) != NULL){
			//printf(" = %s\n",ptr);
		}else{
			perror("error: no se encontro droot\n");
			return error;
		}
		printf("\n");
	}else{
		while((ptr = strtok( NULL, "=: <\n;\t,>\0." )) != NULL){
			if(strcmp(ptr,"droot")==0){
				//printf("%s",ptr);
				if ((ptr = strtok( NULL, "=: <\n;\t,>\0." )) != NULL){
					//printf(" = %s\n",ptr);
				}else{
					perror("error: no se encontro droot\n");
					return error;
				}
				return ptr;
			}
		}
	}
	return ptr;
}
char *tipoArchivo(char *nombre_archivo){
	char *ptr=NULL;
	ptr = strtok( nombre_archivo, "." );    // Primera llamada => Primer token
	ptr = strtok( NULL, " \n\t\0/");
	return ptr;
}
char *nombreArchivo(char buff[100]){
	char *ptr=NULL;
	ptr = strtok( buff, " \n\t\0/" );    // Primera llamada => Primer token
	ptr = strtok( NULL, " \n\t\0/");
	return ptr;
}

void *servicio (void * connfd){
  int leido,leido2,fds,fdc,fdd;
  char buff[1024],buffer[1024],*nombre_archivo=NULL,*tipo_archivo=NULL,*droot=NULL;
  fds = (intptr_t)connfd;
  printf("------------>ahora connfd vale %d\n",fds);
  printf("\n----------LO QUE MANDA EL CLIENTE----------\n");
  leido = read (fds,buff ,sizeof buff);
	write (1 ,buff ,leido);
	//write (connfd ,buff ,leido);
	printf("----------NOMBRE Y TIPO DE ARHCIVO QUE BUSCA EL CLIENTE----------\n");
	nombre_archivo=nombreArchivo(buff); // guardamos el nombre del archivo que desea ver el cliente
	tipo_archivo=tipoArchivo(nombre_archivo);
	printf("archivo: %s\n",nombre_archivo);
	printf("tipo: %s\n",tipo_archivo);
	printf("\n");

  printf("----------NOMBRE DE DROOT SACADO DE TP5.CONF----------");
	fdc = open("tp5.conf",O_RDONLY,0666); //abro archivo de configuracion
	while((leido2 = read (fdc,buffer ,sizeof (buffer))) > 0){ // muestro lo que tieen el archivo configuracion
		//write(1 ,buffer ,leido2);
		droot=buscar_droot(buffer);
		printf("droot= %s\n",droot);
	}
	close(fdc);
  printf("----------BUSCANDO LA DIRECCION DEL ARCHIVO PEDIDO----------\n");
	int lna=0, ldr=0;
	lna=strlen(nombre_archivo)+strlen(tipo_archivo);
	ldr=strlen(droot);
  printf("---> largo nombre archivo %d ----> largo nombre droot %d \n",lna,ldr);
	int n=lna+ldr;
  printf ("---> n = %d\n",n);
	char direccion[n];
	strcpy(direccion,droot);
	strcat(direccion,nombre_archivo);
	strcat(direccion,".");
	strcat(direccion,tipo_archivo);
	printf("direccion: %s\n\n",direccion);

  printf("----------ABRIENDO ARCHIVO Y ENVIANDO LOS DATOS AL CLIENTE----------\n");
  int leido3;
  memset(buffer,'\0',sizeof(buffer)); // limpiamos buffer
	fdd=open(direccion,O_RDONLY,0666); //ariendo archivo
	struct stat infoRecursoHttp; // par calcular el tamaño del archivo
	fstat(fdd,&infoRecursoHttp);
	printf("infoRecursoHttp.st_size (tamanio del archivo)-> %d\n",(int)infoRecursoHttp.st_size );
	char *protocolo=NULL;
  char *aux=NULL;
  s=(char*)malloc(n*sizeof(char));
  //strcpy(aux,protocolo);
  protocolo=armar_protocolo(0,tipo_archivo,(int)infoRecursoHttp.st_size);
  printf("-------> fds = %d\n",fds);
  printf("\n----->fdd = %d\n",fdd);
  printf("<<<PROTOCOLO>>>\n");
  write(fds,protocolo,strlen(protocolo));
  printf("\n<<<CONTENIDO ARCHIVO>>>\n");
  while((leido3=read(fdd,buffer,sizeof(buffer))) > 0){ // muestro lo que tieen el archivo configuracion
    //strcat(aux,buffer);
  }
  //write(fds,aux,strlen(aux));
  close(fds);

  printf("\n");
  //close(fds);
  pthread_exit(NULL);
}

int main(){
	int fd, connfd;
	struct sockaddr_in procrem={};
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if  (fd < 0 ){
		perror ("socket");
		return -1;
	}

	procrem.sin_family = AF_INET;
	procrem.sin_port = htons(8080);
	//procrem.sin_addr = 192.168.1.52;
	procrem.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"0.0.0.0", &procrem.sin_addr);
	int a=1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {
		perror ("bind");
                return -1;
        }

	listen(fd, 5);

	while ( (connfd = accept(fd,NULL, 0)) > 0 ){
    printf("---> confd vale %d",connfd);
    pthread_t tid;
    //crear un hilo
    pthread_create(&tid,NULL,servicio,(void *) (intptr_t) connfd); // CREANDO EL HILO
  	//ptread_create();
	}
  close(connfd);
  pthread_exit(NULL);
	return 0;
}
