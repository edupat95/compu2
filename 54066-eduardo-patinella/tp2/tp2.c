#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char * argv []){

  char buffer[1024];
  int leido;
  int fdo;
  int flagf=0;
  char *nombre_archivo,opt;
  int pid1,pid2; //defino los pid para los hijos
  int ph1[2], ph2[2]; // define el arreglo para los pipes
  pipe(ph1); pipe(ph2); // defino los pipes
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
    if(pid1==0){ // CODIGO PRIMER HIJO
      char *ptr;
      int c1=0,c2=0,c3=0,c4=0,c5=0,c6=0,c7=0,c8=0,c9=0,c10=0,c11=0,c12=0,c13=0,c14=0,c15=0; //contadores
      close(ph2[0]);
      close(ph2[1]);
      close(ph1[1]);

    while((leido = read(ph1[0], buffer, sizeof buffer)) > 0){ // el while es para que el sleep de padre no afecte a la lectura

      ptr = strtok( buffer, " \n\t\0" );    // Primera llamada => Primer token
      //printf( "%s\n", ptr ); //--> MUESTRA LA PALABRA
      while( (ptr = strtok( NULL, " \n\t\0" )) != NULL ){    // Posteriores llamadas
        //printf( "%s\n", ptr ); //Muestra la palabra
        int largo = strlen(ptr); //para guardar el largo de la cadena
        //printf("El largo es: %d \n", largo);
        if(largo==1){
          c1++;
        }else if(largo==2){
          c2++;
        }else if(largo==3){
          c3++;
        }else if(largo==4){
          c4++;
        }else if(largo==5){
          c5++;
        }else if(largo==6){
          c6++;
        }else if(largo==7){
          c7++;
        }else if(largo==8){
          c8++;
        }else if(largo==9){
          c9++;
        }else if(largo==10){
          c9++;
        }else if(largo==11){
          c9++;
        }else if(largo==12){
          c9++;
        }else if(largo==13){
          c9++;
        }else if(largo==14){
          c9++;
        }else if(largo==15){
          c9++;
        }
      }
      //write(1,buffer,leido); // sino estubiese leeria lo que encuentra, en ese caso el primer wait
    }
    printf("\n\n\n");
    printf("Palabras con 1 caracter: %d \n",c1);
    printf("Palabras con 2 caracter: %d \n",c2);
    printf("Palabras con 3 caracter: %d \n",c3);
    printf("Palabras con 4 caracter: %d \n",c4);
    printf("Palabras con 5 caracter: %d \n",c5);
    printf("Palabras con 6 caracter: %d \n",c6);
    printf("Palabras con 7 caracter: %d \n",c7);
    printf("Palabras con 8 caracter: %d \n",c8);
    printf("Palabras con 9 caracter: %d \n",c9);
    printf("Palabras con 10 caracter: %d \n",c10);
    printf("Palabras con 11 caracter: %d \n",c11);
    printf("Palabras con 12 caracter: %d \n",c12);
    printf("Palabras con 13 caracter: %d \n",c13);
    printf("Palabras con 14 caracter: %d \n",c14);
    printf("Palabras con 15 caracter: %d \n",c15);
    return 0;
  } // FIN HIJO 1

  pid2=fork(); //defino hijo 2
  if(pid2==0){ // CODIGO SEGUNO HIJO
    close(ph1[1]);
    close(ph1[0]);
    close(ph2[1]);
    char *ptr;
    char palabra1[]="a";
    char palabra2[]="ante";
    char palabra3[]="bajo"; // CONJUNTO DE PALABRAS RESERVADAS
    char palabra4[]="cabe";
    char palabra5[]="con";
    char palabra6[]="contra";
    while((leido = read(ph2[0], buffer, sizeof buffer)) > 0){
        ptr = strtok( buffer, " \n\t\0" );    // Primera llamada => Primer token

        //printf( "%s\n", ptr ); //--> MUESTRA LA PALABRA
        while((ptr = strtok( NULL, " \n\t\0" )) != NULL ){    // Posteriores llamadas
          if(strcmp(ptr,palabra1)==0){
            for (int i=0; i<strlen(ptr); i++) {
              ptr[i]=toupper(ptr[i]);
              //printf("%c", toupper(ptr[i]));
            }
            printf("%s ",ptr);
          }else if(strcmp(ptr,palabra2)==0){
            for (int i=0; i<strlen(ptr); i++) {
              ptr[i]=toupper(ptr[i]);
              //printf("%c", toupper(ptr[i]));
            }
            printf("%s ",ptr);
          }else if(strcmp(ptr,palabra3)==0){
            for (int i=0; i<strlen(ptr); i++) {
              ptr[i]=toupper(ptr[i]);
              //printf("%c", toupper(ptr[i]));
            }
            printf("%s ",ptr);
          }else if(strcmp(ptr,palabra4)==0){
            for (int i=0; i<strlen(ptr); i++) {
              ptr[i]=toupper(ptr[i]);
              //printf("%c", toupper(ptr[i]));
            }
            printf("%s ",ptr);
          }else if(strcmp(ptr,palabra5)==0){
            for (int i=0; i<strlen(ptr); i++) {
              ptr[i]=toupper(ptr[i]);
              //printf("%c", toupper(ptr[i]));
            }
            printf("%s ",ptr);
          }else if(strcmp(ptr,palabra6)==0){
            for (int i=0; i<strlen(ptr); i++) {
              ptr[i]=toupper(ptr[i]);
              //printf("%c", toupper(ptr[i]));
            }
            printf("%s ",ptr);
          } else {
            printf("%s ",ptr);
          }
        }
    }
    printf("\n");
    return 0;
  } //FIN DEL CODIGO SEGUNDO HIJO

  close(ph1[0]);
  close(ph2[0]);

  printf("el nombre del archivo es: %s ",nombre_archivo);
  fdo = open(nombre_archivo,O_RDONLY,0666);
  while((leido=(read(fdo,buffer,sizeof (buffer)))) > 0){
    write(ph1[1],buffer,leido); //LE ENVIO EL TEXTO AL PPIPE 1
  }
  close(ph1[1]);
  close(fdo);
  fdo = open(nombre_archivo,O_RDONLY,0666);
  while((leido=(read(fdo,buffer,sizeof (buffer)))) > 0){
    write(ph2[1],buffer,leido); //LE ENVIO EL TEXTO AL PPIPE 2
  }
  close(ph2[1]);
  close(fdo);

}else{ // CODIGO CUANDO NO SE ENVIA UN PARAMETRO ------------------------------------
  pid1=fork(); //defino hijo 1
  if(pid1==0){ // CODIGO PRIMER HIJO
    char *ptr;
    int c1=0,c2=0,c3=0,c4=0,c5=0,c6=0,c7=0,c8=0,c9=0,c10=0,c11=0,c12=0,c13=0,c14=0,c15=0; //contadores
    close(ph2[0]);
    close(ph2[1]);
    close(ph1[1]);

  while((leido = read(ph1[0], buffer, sizeof buffer)) > 0){ // el while es para que el sleep de padre no afecte a la lectura

    ptr = strtok( buffer, " \n\t\0" );    // Primera llamada => Primer token
    //printf( "%s\n", ptr ); //--> MUESTRA LA PALABRA
    while( (ptr = strtok( NULL, " \n\t\0" )) != NULL ){    // Posteriores llamadas
      //printf( "%s\n", ptr ); //Muestra la palabra
      int largo = strlen(ptr); //para guardar el largo de la cadena
      //printf("El largo es: %d \n", largo);
      if(largo==1){
        c1++;
      }else if(largo==2){
        c2++;
      }else if(largo==3){
        c3++;
      }else if(largo==4){
        c4++;
      }else if(largo==5){
        c5++;
      }else if(largo==6){
        c6++;
      }else if(largo==7){
        c7++;
      }else if(largo==8){
        c8++;
      }else if(largo==9){
        c9++;
      }else if(largo==10){
        c9++;
      }else if(largo==11){
        c9++;
      }else if(largo==12){
        c9++;
      }else if(largo==13){
        c9++;
      }else if(largo==14){
        c9++;
      }else if(largo==15){
        c9++;
      }
    }
    //write(1,buffer,leido); // sino estubiese leeria lo que encuentra, en ese caso el primer wait
  }
  printf("\n\n\n");
  printf("Palabras con 1 caracter: %d \n",c1);
  printf("Palabras con 2 caracter: %d \n",c2);
  printf("Palabras con 3 caracter: %d \n",c3);
  printf("Palabras con 4 caracter: %d \n",c4);
  printf("Palabras con 5 caracter: %d \n",c5);
  printf("Palabras con 6 caracter: %d \n",c6);
  printf("Palabras con 7 caracter: %d \n",c7);
  printf("Palabras con 8 caracter: %d \n",c8);
  printf("Palabras con 9 caracter: %d \n",c9);
  printf("Palabras con 10 caracter: %d \n",c10);
  printf("Palabras con 11 caracter: %d \n",c11);
  printf("Palabras con 12 caracter: %d \n",c12);
  printf("Palabras con 13 caracter: %d \n",c13);
  printf("Palabras con 14 caracter: %d \n",c14);
  printf("Palabras con 15 caracter: %d \n",c15);
    printf("\n\n\n");
    return 0;
  } // FIN HIJO 1

  pid2=fork(); //defino hijo 2
  if(pid2==0){ // CODIGO SEGUNO HIJO
    close(ph1[1]);
    close(ph1[0]);
    close(ph2[1]);
    char *ptr;

    char palabra1[]="a";
    char palabra2[]="ante";
    char palabra3[]="bajo"; // CONJUNTO DE PALABRAS RESERVADAS
    char palabra4[]="cabe";
    char palabra5[]="con";
    char palabra6[]="contra";
    while((leido = read(ph2[0], buffer, sizeof buffer)) > 0){
      ptr = strtok( buffer, " \n\t\0" );    // Primera llamada => Primer token

      //printf( "%s\n", ptr ); //--> MUESTRA LA PALABRA
      while((ptr = strtok( NULL, " \n\t\0" )) != NULL ){    // Posteriores llamadas
        if(strcmp(ptr,palabra1)==0){
          for (int i=0; i<strlen(ptr); i++) {
            ptr[i]=toupper(ptr[i]);
            //printf("%c", toupper(ptr[i]));
          }
          printf("%s ",ptr);
        }else if(strcmp(ptr,palabra2)==0){
          for (int i=0; i<strlen(ptr); i++) {
            ptr[i]=toupper(ptr[i]);
            //printf("%c", toupper(ptr[i]));
          }
          printf("%s ",ptr);
        }else if(strcmp(ptr,palabra3)==0){
          for (int i=0; i<strlen(ptr); i++) {
            ptr[i]=toupper(ptr[i]);
            //printf("%c", toupper(ptr[i]));
          }
          printf("%s ",ptr);
        }else if(strcmp(ptr,palabra4)==0){
          for (int i=0; i<strlen(ptr); i++) {
            ptr[i]=toupper(ptr[i]);
            //printf("%c", toupper(ptr[i]));
          }
          printf("%s ",ptr);
        }else if(strcmp(ptr,palabra5)==0){
          for (int i=0; i<strlen(ptr); i++) {
            ptr[i]=toupper(ptr[i]);
            //printf("%c", toupper(ptr[i]));
          }
          printf("%s ",ptr);
        }else if(strcmp(ptr,palabra6)==0){
          for (int i=0; i<strlen(ptr); i++) {
            ptr[i]=toupper(ptr[i]);
            //printf("%c", toupper(ptr[i]));
          }
          printf("%s ",ptr);
        } else {
          printf("%s ",ptr);
        }
      }
    }
    printf("\n");
    return 0;
  } //FIN DEL CODIGO SEGUNDO HIJO

  close(ph1[0]);
  close(ph2[0]);

  while((leido=(read(1,buffer,sizeof (buffer)))) > 0){
    write(ph1[1],buffer,leido); //LE ENVIO EL TEXTO AL PPIPE 1
  }
  close(ph1[1]);

  while((leido=(read(1,buffer,sizeof (buffer)))) > 0){
    write(ph2[1],buffer,leido); //LE ENVIO EL TEXTO AL PPIPE 2
  }
  close(ph2[1]);
}
return 0;
}
