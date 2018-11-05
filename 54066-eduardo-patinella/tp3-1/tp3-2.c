#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
//#include "mayuscularizar.h"
//#include "contar.h"
#define BUFF 1024
sem_t *sema, *sema2, *sema3, *sema4;
int pid1,pid2,*variables=NULL,*leido=NULL,fdo;
int flagf=0;
char *nombre_archivo,opt,*buffer=NULL;
char *semaforos=NULL;

int count(){
  printf("contar\n");
}
int convert(){
  printf("convertir\n");
}


int main(int argc, char * argv []){
  semaforos = mmap(NULL,4*sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS, -1,0); //Proceso relacionado y mapeo anonimo (sin abriri archivo)
	sema  = (sem_t *)semaforos ;// colocamos el semaforo en memoria compartida
	sema2 = sema + sizeof(sema2);	// lo mismo pero en posiciones mas adelantes para q no se pise
	sema3 = sema2 + sizeof(sema3);
	sema4 = sema3 + sizeof(sema4);
	sem_init(sema,1,0);
	sem_init(sema2,1,0);
	sem_init(sema3,1,0);
	sem_init(sema4,1,0);
  leido=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS, -1,0);
	variables=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS, -1,0);
	buffer = mmap(NULL,1024,PROT_READ|PROT_WRITE,MAP_SHARED |MAP_ANONYMOUS, -1,0);

  while ((opt = getopt(argc, argv, "f:")) != -1){
    switch (opt){
      case 'f':
      flagf = 1;
      nombre_archivo=optarg;
      break;
    }
  }

  if(flagf==1){ //CODIGO CUNADO SE ENVIA UN PARAMETRO
    printf ("PASO PARAMETRO\n");
    pid1=fork(); //defino hijo 1
    if(pid1==0){ // CODIGO PRIMER HIJO
      printf("hola soy hijo 1\n");
      return 0;
    } // FIN HIJO 1

    pid2=fork(); //defino hijo 2
    if(pid2==0){ // CODIGO SEGUNO HIJO
      printf("hola soy hijo 2\n");
      return 0;
    } //FIN DEL CODIGO SEGUNDO HIJO

  }else{ // CODIGO CUANDO NO SE ENVIA UN PARAMETRO ------------------------------------
    pid1=fork(); //defino hijo 1
    if(pid1==0){ // CODIGO PRIMER HIJO
      count();
      return 0;
    } // FIN HIJO 1

    pid2=fork(); //defino hijo 2
    if(pid2==0){ // CODIGO SEGUNO HIJO
      convert();
      return 0;
    } //FIN DEL CODIGO SEGUNDO HIJO

    while((*leido=(read(0,buffer,BUFF))) > 0){
    	//sem_post(sema);
    	//sem_post(sema2);
    	//sem_wait(sema3);
    	//sem_wait(sema4);
    }

  }
  return 0;
}
