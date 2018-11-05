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


void *servicio (void * connfd){
  int leido,fds;
  char buff[1024];
  fds = (intptr_t)connfd;
  leido = read (fds,buff ,sizeof buff);
	write (1 ,buff ,leido);
	//write (connfd ,buff ,leido);
  printf("------------>ahora connfd vale %d",fds);
  close(fds);
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
