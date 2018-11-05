#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
/*struct sockaddr_in {
short            sin_family;   // e.g. AF_INET
unsigned short   sin_port;     // e.g. htons(3490)
struct in_addr   sin_addr;     // see struct in_addr, below
char             sin_zero[8];  // zero this if you want to
};
struct in_addr {
unsigned long s_addr;  // load with inet_aton()
};*/

int connfd;
char buff[1024];
int leido;
void buscar_palabra(char *palabra){
	//int tam,n=0;
	char aux[100];
	char *ptr=NULL;
	ptr = strtok( buff, ": <\n;\t,>\0." );    // Primera llamada => Primer token
	if(strcmp(ptr,palabra)==0){
		printf("%s ",ptr);
		for(int i=0; i<10 ; i++){
			if ((ptr = strtok( NULL, ": <\n;\t,>\0." )) != NULL){
				printf("%s ",ptr);
			}
		}
		printf("\n");
	}
	while((ptr = strtok( NULL, ": <\n;\t,>\0." )) != NULL ){    // Posteriores llamadas
		if(strcmp(ptr,palabra)==0){
			printf("%s ",ptr);
			strcpy(aux,ptr);
			strcat(aux," ");
			for(int j=0; j<10 ; j++){
				if ((ptr = strtok( NULL, ": <\n;\t,>\0." )) != NULL){
					printf("%s ",ptr);
					strcat(aux,ptr);
					strcat(aux," ");
				}
			}
			printf("\n");
			printf("---> aux: %s\n",aux);
			strcat(aux,"\n");
			write(connfd,aux,strlen(aux));
		}

	}
	//write(1,buffer,leido);
}

int main(){
	int fd, fdcliente, conectado, pid;
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
	int a = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {
		perror ("bind");
		return -1;
	}

	listen(fd, 5);
	signal(SIGCHLD,SIG_IGN ); //para sacar procesos terminados de la PCB ...
	while ( (connfd = accept(fd,NULL, 0)) > 0 ){
		pid = fork();
		//hijo
		if ( pid == 0 ) {
			leido = read (connfd,buff ,sizeof buff);
			write (1 ,buff ,leido);

			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------
			char *ptr=NULL,*link=NULL,*palabra_reservada=NULL,*ancla=NULL;
			char *p1=NULL,*p2=NULL,*url=NULL;
			int largo_ancla=0,tam_ancla=0,tam_url=0,tam_oracion=0;

			ptr = strtok(buff,"-");
			link=ptr;
			if((ptr = strtok(NULL,"")) != NULL){
				palabra_reservada=ptr;
			}
			printf("\nlink ->> %s\n",link);
			printf("palabra a buscar->> %s\n",palabra_reservada);
			//-------------------------------------------------------------------------------

		  p1="http:";
		  p2="https:";
		  ptr = strtok(link,"/");    // Primera llamada => Primer token

		  if((strcmp(ptr,p2)==0) || (strcmp(ptr,p1)==0)){
		    ptr = strtok(NULL,"/");
		    url=ptr;
		    printf("url -> %s\n",url);
		  } else {
		    url=ptr;
		    printf("url -> %s\n",url);
		  }

		  if((ptr = strtok(NULL,"")) != NULL){
		    ancla=ptr;
		  }
		  largo_ancla=strlen(ancla);
		  char nuevo_ancla[largo_ancla+1];
		  for(int i=0; i<=largo_ancla;i++){
		    if(i==0){
		      nuevo_ancla[0]='/';
		    }else{
		      nuevo_ancla[i]=ancla[i-1];
		    }
		  }

			tam_url=strlen(url);
			tam_ancla=strlen(nuevo_ancla);
			printf("palabra reservada-> %s \n",palabra_reservada);
			printf("ancla-> %s \n",nuevo_ancla);
			printf("tam url: %d\n",tam_url);
			printf("tam ancla: %d\n",tam_ancla);
			tam_oracion=40+tam_url+tam_ancla;
			char oracion[tam_oracion];
			printf("tam oracion: %d\n",tam_oracion);
			char part1[]="GET ";
			char part2[]=" HTTP/1.1\nHost: ";
			char part3[]="\nConnection: close\n\n";
			strcpy(oracion,part1);
			strcat(oracion,nuevo_ancla);
			strcat(oracion,part2);
			strcat(oracion,url);
			strcat(oracion,part3);
			printf("oracion:\n%s",oracion);
			printf("-----------------\n");
			printf("GET /es/ua/fi.html HTTP/1.1\nHost: www.um.edu.ar\nConnection: close\n\n");

			//--------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------

			//write (connfd ,buff ,leido);
			// se conecta como cliente a un servidor http

			struct sockaddr_in serverhttp={};
			fdcliente = socket(AF_INET, SOCK_STREAM, 0);
			if  (fdcliente < 0 ){
				perror ("socketcliente");
				return -1;
			}
			serverhttp.sin_family = AF_INET;
			serverhttp.sin_port = htons(80); //puerto default servidor http
			//procrem.sin_addr = 192.168.1.52;
			//procrem.sin_addr.s_addr = htonl(0xc0a80134);
			//inet_pton(AF_INET,"www.um.eud.ar", &procrem.sin_addr);

			struct hostent *hp = gethostbyname(url);

			inet_pton(AF_INET,inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0])), &serverhttp.sin_addr);
			conectado = connect(fdcliente,(struct sockaddr *)&serverhttp, sizeof serverhttp);
			if  (conectado < 0 ){
				perror ("connect");
				return -1;
			}
			write(fdcliente, oracion, tam_oracion);

			char p[strlen(palabra_reservada)];
			strcpy(p,palabra_reservada);
			while ((leido = read(fdcliente, buff , sizeof buff)) > 0){
				if  (leido < 0 ){
					perror ("read");
					return -1;
				}
				//-------------------------------------------------------
				//-------------------------------------------------------
				buscar_palabra(p);

				//-------------------------------------------------------
				//-------------------------------------------------------
				//write(connfd,buff,leido);
				//parsear palabra
			}

			return 0;
		}
		close(connfd);
	}
	return 0;
}
