#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h> // PER USARE IL AFUNIX



#include "goToDir.h"

#define MAX 10
#define SOCK_PATH "fileSocket"


enum  pfc {PFC1=1,PFC2,PFC3};
char buffBefore[20];


/*
  Crea l'array con il nome del file in cui scrivere il dato.
*/
void creatName(int id, char *buff){
	sprintf(buff,"speedPFC%d.log", id);
}


/*
  Effettua la scrittura del dato inviato nei rispettivi file:
   - speedPFC1.log
   - speedPFC2.log
   - speedPFC3.log

*/
void writeLog(char *str,char *buff){
	
	FILE *fpoint;
		
	goToDir("log");
	
	fpoint = fopen(str, "a");
	if(fpoint == NULL){
		printf("errore");
	}
	
	fprintf(fpoint, "%s\n", buff);
	fclose(fpoint);
	
	goToDir("src");


}


/*
  Ad ogni secondo, il processo "Transducers1" entra nel metodo per leggere il dato, che li 
  e' stato inviato dal processo "PFC1" tramite socket. 
  Successivamente, scrive il dato nel file log: "speedPFC1.log" richiamando il metodo "writeLog".

*/
void readPFC1(){
	
	// Dati per PFC1
	char name[15];
	int serverSocket, socketClient, serverLen, clientLen;
	char buff1[MAX];
	char str1[MAX];
	
	
	
	struct sockaddr_un server; //Server address 

	struct sockaddr_un client; //Client address 
	
	
	serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(serverSocket < 0){
		printf("Errore nella creazione della socket");
		perror("socket:");
		exit(1);
	}
	
	server.sun_family = AF_UNIX;
	
	strcpy(server.sun_path, SOCK_PATH);
	unlink(SOCK_PATH);
	serverLen = strlen(server.sun_path) + sizeof(server.sun_family);

	bind(serverSocket, (struct sockaddr *) &server, serverLen);
	
	listen(serverSocket, 100);

	clientLen = sizeof (client);
	
	socketClient = accept(serverSocket, (struct sockaddr *) &client, &clientLen);
	
	recv(socketClient, buff1, MAX, 0);
	
	close(socketClient);
	

	strcpy(str1, buff1);	// copia una stringa ad un altra
	
	
	creatName(PFC1, name);
	writeLog(name, str1);
}


/*
  Ad ogni secondo, il processo "Transducers2" entra nel metodo per leggere il dato, che li 
  e' stato inviato dal processo "PFC2" tramite pipe. 
  Successivamente, scrive il dato nel file log: "speedPFC2.log" richiamando il metodo "writeLog".

*/
void readPFC2(){
	char name[15]; 
	int fd;
	char *mypipe = "/tmp/mypipe";
	char buff2[MAX];
	
	fd = open(mypipe, O_RDONLY);
	read(fd, buff2, MAX);
	close(fd);
		
	creatName(PFC2, name);
	writeLog(name, buff2);
	
}


/*
  Ad ogni secondo, il processo "Transducers3" entra nel metodo per leggere il dato, che li 
  e' stato inviato dal processo "PFC3" tramite file condiviso. 
  Successivamente, scrive il dato nel file log: "speedPFC3.log" richiamando il metodo "writeLog".
  Ogni volta controlla che la riga appena letta, sia diversa da quella letta precedentemente.
  In caso in cui non lo fosse, attende il nuovo dato, richiamando il metodo ogni volta, fin 
  quando le due stringhe (quella precedente e quella nuova) non siano diverse.

*/
void readPFC3(){
	FILE *file;
	
	char name[15];
	char buff3[20];
	char str3[MAX-1];
	
	sleep(1);
	
	goToDir("log");
	
	file = fopen("fileTransducers.log", "r");
	fgets(buff3, 20, file);
	fclose(file);
	
	goToDir("src");
	
	int ris = strcmp(buff3, buffBefore); 	/* Tramite il comando "strcmp" effettuo un confronto tra le due stringhe, la quale restituisce:
						    - il valore 0, se le stringhe sono diverse (quindi richiamo il metodo "readPFC3")
						    - un valore maggiore di 0, in base a quanti elementi degli due array sono diversi tra di loro.
						      A quel punto, tramite il comando "strcpy" copio l'array nuovo (buff3) nell'array "buffBefore".
						*/
	
	if(ris == 0){
		
		readPFC3();	
	
	}else{
		strcpy(buffBefore, buff3);     // copia la stringa buff3 nella stringa buffBefore
	}
	
	int count = 0;
	while(buff3[count] != ' '){		/* Dato che, la riga prelevata dal file e' della forma: "R: 0.00..", ho bisogno di un contatore
						   per ricavare la posizione in cui ho il valore della velocita'. 
						*/
		count += 1;
	
	}
		
		
	int i, j;
	for (i = 0, j = count +1; j < strlen(buff3); i++, j++) { /* Partendo dalla posizione ricavata precedentemente, riscrivo il valore della velocita' 
							      	      in un altra stringa, cosi' da poter strivere più avanti all'interno del file.
							   	   */
						   
		char tmp = buff3[j];
		str3[i] = tmp;
			
	}
	
	creatName(PFC3, name);	
	writeLog(name, str3);
	
	
}

/*
  Il main(), attraverso un if, manda i vari processi "Transducers" nei rispettivi metodi di lettura
  della velocita'. Le condizioni sono:
   - Transducers1 (numero processo 4): entra nel metodo "readPFC1" per leggere il dato inviato da "PFC1"
                                       tramite socket.
                                       
   - Transducers2 (numero processo 5): entra nel metodo "readPFC2" per leggere il dato inviato da "PFC2"
                                       tramite pipe.
                                       
    - Transducers3 (numero processo 6): entra nel metodo "readPFC3" per leggere il dato inviato da "PFC3"
                                       tramite file condiviso. 

*/
int main(int argc, char* argv[]){
		
	int id = atoi(argv[1]);
	
	if(id == 4){
		while(1){
			sleep(1);
			readPFC1();
				
		}
	
	}else if(id == 5){
		while(1){
			sleep(1);
			readPFC2();		
		}
	
	}else if (id == 6){
		while(1){
			
			readPFC3();
			sleep(1);
			
		}
	
	}
	
	return 0;
}






