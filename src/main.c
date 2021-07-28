#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "goToDir.h"

/*
  Metodo previsto per lo svolgimento dei processi PFC1, PFC2 e PFC3.
*/
void procesPFC(char *argv){
		
	char *args[] = {"./readG18", argv, NULL}; 
	int n = execvp(args[0], args);
	
	

}


/*
  Metodo previsto per lo svolgimento dei processi Transducers.
*/	
void procesTransducers(char *argv){
	
	char *args[] = {"./Transd", argv, NULL}; 
	int n = execvp(args[0], args);

}


/*
  Metodo previsto per lo svolgimento del processo Generatore Fallimenti.
*/	
void procesGenFall(char *argv){
	
	char *args[] = {"./errorGenerator", argv, NULL}; 
	int n = execvp(args[0], args);

}


/*
  Metodo previsto per lo svolgimento del processo Wes.
*/	
void procesWes(char *argv){
	
	char *args[] = {"./wes", argv, NULL}; 
	int n = execvp(args[0], args);

}


/*
  Metodo previsto per lo svolgimento del processo PFC Disconnect Switch.
*/
void procesPFCDS(char *argv){
	
	char *args[] = {"./discSwitch", argv, NULL}; 
	int n = execvp(args[0], args);

}



/*
  writePID() e' un metodo, che consiste nel scrivere all'interno di un file il pid del processo
  passato in input. Oltre al "pid", passeremo un "id", che identifica il numero del processo, che stiamo 
  eseguendo e un valore intero (posizione). 
  Il valore intero "posizione" viene aggiornato in base alla lunghezza delle stringhe, che vengono 
  scritte nel file. Infatti, viene restituito alla fine del metodo.
  
*/
void writePID(int pid, int id){
	
	char buff[50];
	
	char argv[3]; 
	sprintf(argv, "%d\n", id);
	
	goToDir("log");
		
	FILE *point;
	point = fopen("filePID.log", "a");
	
	switch(id){
	
		case 1:
			
			sprintf(buff, "PFC1 valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesPFC(argv);
			
			break;
		case 2:
			
			sprintf(buff, "PFC2 valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesPFC(argv);
			
			break;
			
		case 3:		
			
			sprintf(buff, "PFC3 valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesPFC(argv);			
			
			break;
			
		case 4:
			
			sprintf(buff, "Transd1 valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesTransducers(argv);
			
			break;
			
		case 5:
			
			sprintf(buff, "Transd2 valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesTransducers(argv);			
			
			break;
		
		case 6:
			
			sprintf(buff, "Transd3 valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesTransducers(argv);			
			
			break;
			
		case 7:
	
			sprintf(buff, "Generatore fallimenti valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesGenFall(argv);			
			
			break;
			
		case 8:
			sprintf(buff, "WES valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesWes(argv);
			
			break;
			
		case 9:
			sprintf(buff, "PFC Disconnect Switch valore pid: %d\n", pid);
			fputs(buff, point);
			fclose(point);
			
			goToDir("src");
			
			procesPFCDS(argv);
			
			break;
	}

	
}


/*
  Il metodo createSon() permette di creare tutti i processi necessari per lo svolgimento del progetto.
  Ogni volta, che viene creato un nuovo progesso, esso scrivera' il proprio PID nel file: "filePID.txt".
  Succesivamente, tramite il comando "execvp", eseguira' il proprio codice entrando nel file "eseguzioneProcessi.c".
  Prende come input: l'id di identificazione e un valore interno, che corrisponde alla posizione del puntatore
  del file.	

*/
void createSon(int i){
	int child;
	child = fork();
	
	if(child < 0){
		
		printf("Errore nella creazione del processo figlio numero %d\n", i);
		perror("createSon:");
		exit(1);
	
	}else if(child == 0){
				
		int pid;
		
		pid = getpid();
		writePID(pid, i);
		
		
	}

}


int main(){

	for(int i = 1; i < 10; i++){
		createSon(i);
		
	}

	while(wait(NULL) > 0);

	return 0;	
	
}


