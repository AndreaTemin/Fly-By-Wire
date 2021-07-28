#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "goToDir.h"
#include "researchPid.h"


#define success 1
int choosenProcess = 0;

void writeOnLog(char sentSignal[]) {
	
	char buff[50];
	
	goToDir("log");
	
	FILE *point;
	point = fopen("failures.log", "a");

	sprintf(buff, "Inviato segnale: %s al PFC%d", sentSignal, choosenProcess);
	fprintf(point, "%s\n", buff);	
	fclose(point);
	
	goToDir("src");
}


void send(int segnale, pid_t pid, int signum, char sentProcess[]) {
	
	if(segnale == success && pid > 0) {
		kill(pid, signum);
		writeOnLog(sentProcess);
		//printf("Processo bloccato con pid: %d\n", pid);
    	}
}


int randomNumber(int lower, int upper) {
	int n = (rand() % (upper - lower + 1)) + lower;
	
	return n;
}


void sendSignal(pid_t pid) {

    	int sendSIGSTOPsignal = randomNumber(1, 100);
    	send(sendSIGSTOPsignal, pid, SIGSTOP, "SIGSTOP");
    	
    	int sendSIGINTsignal = randomNumber(1, 10000);
    	send(sendSIGINTsignal, pid, SIGINT, "SIGINT");
    	
    	int sendSIGCONTsignal = randomNumber(1, 10);
    	send(sendSIGCONTsignal, pid, SIGCONT, "SIGCONT");
    	
    	int sendSIGUSR1signal = randomNumber(1, 10);
    	send(sendSIGUSR1signal, pid, SIGUSR1, "SIGUSR1");	
}


void errorGenerator() {
	
	choosenProcess = randomNumber(1, 3);
		
	int pidProcessoScelto = researchPid(choosenProcess);	
	
	sendSignal(pidProcessoScelto);
}


int main(int argc, char* argv[]) {
	
	while(1){
		sleep(1);
		errorGenerator();
	
	}
	
	return 0;
}


