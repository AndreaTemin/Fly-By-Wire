#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h> // PER USARE IL AFUNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>


#include "functions.h"
#include "researchPid.h"
#include "goToDir.h"
#include "status.h"

#define MAX 50
#define VUOTO 0


bool shifSpeed = false;
int pfc;
int riga;

/*
  Il metodo "dataTransfer" permette, in base al tipo di processo, di inviare il valore
  della velocita' al processo "Transducers". Le condizioni solo le seguenti:
   - Per il processo PFC1, il dato viene inviato tramite socket
   - Per il processo PFC2, il dato viene inviato tramite pipe (si utilizza un pipe con nome)
   - Per il processo PFC3, il dato viene inviato tramite file condiviso (nome del file: fileTran.log)
*/
void dataTransfer(int pfc, double speed){
	char str [10];
	sprintf(str, "%6f", speed); 				// Mi permette di inserire il valore double nella stringa
	

	//Dati per PFC1
	
	int clientLen, skClient;
	
	if(pfc == 1){
		sleep(1);
		
		struct sockaddr_un client; /*Server address */
	
		skClient = socket(AF_UNIX, SOCK_STREAM, 0);

		client.sun_family = AF_UNIX;
		
		strcpy(client.sun_path, "fileSocket");

		clientLen = strlen(client.sun_path) + sizeof(client.sun_family);

		int ris;
		
		do {
			ris = connect(skClient, (struct sockaddr*) &client, clientLen);
		}while(ris != -1);
	}

	//Dati di PFC2
	int fd;
	char *mypipe = "/tmp/mypipe";
	if(pfc == 2){	
		mkfifo(mypipe, 0666);
	}
	
	// Dati di PFC3
	FILE *file;
	char buff[20];
		
	
	switch(pfc){
	
	
		case 1:
			
			send(skClient, str, strlen(str), 0);
			close(skClient);
			
			break;
		
		case 2:
			
			fd = open(mypipe, O_WRONLY);
			write(fd, str,sizeof(str));			
			close(fd);
			sleep(1);
			unlink("mypipe");

			break;
				
	
	
		case 3:
			goToDir("log");
			
			file = fopen("fileTransducers.log", "w");
			sprintf(buff, "R:%d %s", riga, str);
			fputs(buff, file);
			fclose(file);
	
			goToDir("src");
			riga += 1;
			sleep(1);
			break;
			
	}
}


/*
  Si entra nel metodo solamente, se il processo riceve un segnale SIGUSER1. 
  Il valore booleano (shifSpeed) viene portato a true, cosi' da avvertire il processo di 
  effettuare lo shift nel prossimo calcolo della velocita'.

*/
void controlSignalUSR(int sig){

	shifSpeed = true;
	char status[] = "Shift\n";
	writeStatus(pfc, status);
}


void controlSignalSTOP_INT(int sig){
	char status[] = "Sospeso\n";
	writeStatus(pfc, status);

}


void controlSignalCONT(int sig){
	char status[] = "Ripresa esecuzione\n";
	writeStatus(pfc, status);

}



/*
  Calcolo della velocita' in base alle stringhe, che vengono passate in input.
  Tutti i metodi si trovano nel file "functions.h", che abbiamo importato all'inizio
  del file.
*/
double speed(char str1[], char str2[]){
	
	double tmp1 = tempoGLL(str1);				// Ricavo il tempo per le prime coordinate
	double tmp2 = tempoGLL(str2);				// Ricavo il tempo per le seconde coordinate
												
	
								// Ricavo la latitudine e longitudine delle prime coordinate
	double lat1 = latitudine(str1);
	double long1 = longitudine(str1);
	
								// Ricavo la latitudine e longitudine delle seconde coordinate
	double lat2 = latitudine(str2);
	double long2 = longitudine(str2);
	
		
												
	double dspazio = distance(lat1, long1, lat2, long2); // Calcolo dspazio = (Sfinale - Siniziale), che corrisponde alla distanza tra due segnali GPS
	double dtempo = tmp2 - tmp1;				// Calcolo dtempo = (Tfinale - Tiniziale)
	

	double velocita = dspazio / dtempo;			// Calcolo la velocita' in metri al secondo
	
	
	return velocita;
}

/*
  All'interno del metodo main si effettua la lettura del file "G18.txt", tale che, 
  ad ogni riga letta preleviamo la latitudine, la longitudine e il tempo, cosi' da poter calcolare
  la velocita' attuale. In seguito, entrando nel metodo: "dataTransfer", si effettua l'invio della
  velocita' al processo "Transducers", in base alle condizioni stabilite nel progetto (tramite pipe, socket
  o file condiviso). Questo viene eseguito una volta al secondo.
  In questa parte di codice, e' presente anche la gestione dei segnali inviati dal progesso "Generatore Fallimenti",
  che saranno di default per i segnali: SIGINT, SIGSTOP e SIGCOUNT, mentre in presenza del segnale SIGUSR1, verra' 
  effettuato uno shift di due posizioni verso sinistra nella velocita'. 

*/
int main(int argc, char* argv[]){
	

	double v = 0.0;	
	pfc = atoi(argv[1]); 		// Prelevo l'id del processo

	
	int Trans;

	if(pfc == 1){
		Trans = researchPid(4);
	}else if(pfc== 2){
		Trans = researchPid(5);
		
	}else if(pfc== 3){
		Trans = researchPid(6);
	}
	
	FILE *pointG;
	char buff[MAX];
	char *res = "INIZIO";

	char str1[MAX];
	char str2[MAX];

	str1[0] = VUOTO;				// Lo inizializzo per if
	str2[0] = VUOTO;

	char status[] = "Esecuzione\n";
	writeStatus(pfc, status);

	
	pointG = fopen("G18.txt", "r");

	while (res != NULL) {
		signal(SIGSTOP, controlSignalSTOP_INT);
		signal(SIGINT, controlSignalSTOP_INT);
		signal(SIGCONT, controlSignalCONT);
		
		
		res = fgets(buff, MAX, pointG);
		
		if (buff[4] == 'L'){
		 	if (str1[0] == VUOTO)
		 						/* Verifico se la stringa letta nella posizione 4 e' presente la lettera L, perche' a noi interessano le righe, 
								   che iniziano: "$GPGLL.." e successivamente, controllo se str1 è vuota, cioe' non ho ancora salvato nessuna riga 
								   al suo interno.
								*/
				strcpy(str1, buff);	
			
			else
				strcpy(str2, buff);		// Entro nel caso in cui, str1 fosse piena, allora salvo la seconda riga nella stringa str2.
		
		}
		if (str1[0] == '$' && str2[0] == '$') {
			
			v += speed(str1, str2);
			
			if(shifSpeed == true){
				int vshift = (int)v; 
				vshift <<= 2; 			// shift di due posti a sinistra
								// printf("%d\n", vshift);
				
				v = (double)vshift;
								//printf("%f\n", v);
				
				shifSpeed = false;
			}			
			
			dataTransfer(pfc, v);
			
			strcpy(str1, str2);
						
			str2[0] = VUOTO;
			sleep(1);
			
		}
		
		signal(SIGUSR1, controlSignalUSR);
				
	}
	
	kill(Trans, SIGINT);
	
	return 0;

}

