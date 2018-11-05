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
#include "mayuscularizar.h"
#include "contar.h"
#define BUFF 1024

int main(int argc, char * argv []){
  int pid1,pid2,*leido=NULL,fdo;
  int flagf=0;
  char *nombre_archivo,opt,*buffer=NULL;
  //sem_t *sema, *sema2, *sema3, *sema4;

  while ((opt = getopt(argc, argv, "f:")) != -1){
    switch (opt){
      case 'f':
      flagf = 1;
      nombre_archivo=optarg;
      break;
    }
  }

  if(flagf==1){ //CODIGO CUNADO SE ENVIA UN PARAMETRO
    pid1=fork(); //defino hijo 1
    if(pid1==0){
      convert();
    }
    pid2=fork(); //defino hijo 2
    if(pid2==0){
      count();
    }

    printf("el nombre del archivo es: %s ",nombre_archivo);
    /*
    fdo = open(nombre_archivo,O_RDONLY,0666);
    while((*leido=(read(fdo,buffer,BUFF))) > 0){
  		//sem_post(sema);
  		//sem_post(sema2);
  		//sem_wait(sema3);
  		//sem_wait(sema4);
  	}*/
  }else{ // CODIGO CUANDO NO SE ENVIA UN PARAMETRO ------------------------------------
    pid1=fork(); //defino hijo 1
    if(pid1==0){
      printf("llamo a convert\n");
      convert();
    }

    pid2=fork(); //defino hijo 2
    if(pid2==0){
      printf("llamo a count\n");
      count();
    }
/*
    while((*leido=(read(0,buffer,BUFF))) > 0){
  		//sem_post(sema);
  		//sem_post(sema2);
  		//sem_wait(sema3);
  		//sem_wait(sema4);
  	}*/
  }
  return 0;
}
