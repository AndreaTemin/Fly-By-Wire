#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "researchPid.h"
#include "goToDir.h"

#define MAX 6
#define MAX_BUFF 50

/*
  Il metodo restituisce il pid del processo cercato. Prende come input la riga di testo letta 
  nel file "filePID.log", poi partendo dal fondo del buffer, scorriamo a ritroso cosi' da ricavare il pid
  e lo salviamo in un altra stringa (str).
  A quel punto, tramite la funzione "atoi" convertiamo la stringa in un intero.
  
*/
int copyStr(char buff[]){
	int pid;
	char str[MAX];

	int j = strlen(buff) - 1;
	int i = 5;
	while(buff[j] != ' '){
		str[i] = buff[j];
		j --;
		i --;
	}
		
	pid = atoi(str);

	return pid;
}




/*
  Il file di eseguzione "researchPid.c" consiste nel ricavare i pid degli altri processi, tramite la lettura
  nel file "filePID.log". In input viene passato l'identificativo del processo, alla quale, vogliamo ricavare il suo pid.
  Infatti, restituisce il pid del processo cercato. 
*/


int researchPid(int id){
	int pidProcess = 0;
	char buff[MAX_BUFF];
	char str[MAX];
	char *num;

	goToDir("log");	

	FILE *point;
	point = fopen("filePID.log", "r");
	
	while(1){
		
		num = fgets(buff, sizeof(buff), point);
		if (num == NULL || pidProcess > 0) {
			// Il file è terminato
			break;
		}
			
		if(id == 8 && buff[0] == 'W'){
			pidProcess = copyStr(buff);			// Restituisce il PID di WES
			
		}else if(id == 4 && buff[0] == 'T' && buff[6] == '1'){
			pidProcess = copyStr(buff);			// Restituisce il PID di Transducers
			
			
		}else if(id == 5 && buff[0] == 'T' && buff[6] == '2'){
			pidProcess = copyStr(buff);			// Restituisce il PID di Transducers
			
			
		}else if(id == 6 && buff[0] == 'T' && buff[6] == '3'){
			pidProcess = copyStr(buff);			// Restituisce il PID di Transducers
			
		}else if(id == 7 && buff[0] == 'G'){			
			pidProcess = copyStr(buff);			// Restituisce il PID di Generatore Fallimenti
			
			
		}else if(id == 1 && buff[0] == 'P' && buff[3] == '1'){
			pidProcess = copyStr(buff);		// Restituisce il PID di PFC1
			
			
		}else if(id == 2 && buff[0] == 'P' && buff[3] == '2'){
			pidProcess = copyStr(buff);			// Restituisce il PID di PFC2
			
			
		}else if(id == 3 && buff[0] == 'P' && buff[3] == '3'){
			pidProcess = copyStr(buff);			// Restituisce il PID di PFC3
			
			
		}else if(id == 9 && buff[4] == 'D'){
			pidProcess = copyStr(buff);			// Restituisce il PID di PFC Disconnect Switch
			
		}
	}

	fclose(point);
	
	goToDir("src");
	
	return pidProcess;
}


