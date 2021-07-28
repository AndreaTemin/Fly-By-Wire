#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "goToDir.h"

enum boolean {FALSE,TRUE};
    


int namedPipe(int mess)
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char message[3];
    sprintf(message,"%d",mess);
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);
    write(fd, message, sizeof(message));
    close(fd);

    /* remove the FIFO */
    unlink(myfifo);

    return 0;
}





void printMessage(char * mess){
    
    FILE * pf;
    
    
    goToDir("log");
    
    pf = fopen("status.log","a");

    //printf("%i",pf);

    if(fputs(mess,pf)<1)
        perror("Errore");
    
    fclose(pf);
    fflush(pf);
    
    printf("\n %s \n", mess);
    fflush(stdout);
    
    goToDir("src");
    
    
}



/* prende dal file ricevendo il nome in stringa i primi 5 caratteri del file (da verificare quanto sara' grande il testo da leggere)
 * e restituisce il valore letto come intero  
*/  

float takeSpeed(char * nomeFile,int LinePosition){
    
        
    FILE *mioFile;
    char speed[10];
    float rst;
    char *scroll; 
    
    mioFile = fopen(nomeFile,"r");
    //LinePosition = LinePosition + 1;     
             
    for(int i=0;i<LinePosition;i++){
        if(fgets(speed,10,mioFile) == NULL){
     		printf("non ha letto riga\n");
     		rst = -2;
     		return rst; 
        
        }
    }
    printf("%s: %s",nomeFile,speed);
        
    
   
    rst = atof(speed);
    
    //printf("%f\n",rst);

    fclose(mioFile);
      
    
    
    return rst;
}
 
 
  
 /*
  * restituisce:
  *     0 se le velocita' sono tutte e 3 uguali,
  *     1,2,3 a seconda di quale processo ha lasciato un valore diverso nei file di log (1 = speedPFC1.log etc...)
  *     -1 se tutti i valori sono differenti; 
  *     -2 almeno una delle velocita non è stata letta
  */ 
    
int compareSpeeds(int posLine) {
    
    float s1,s2,s3;
    int rst = 0;
    
    goToDir("log");
    
    s1 = takeSpeed("speedPFC1.log",posLine);
    s2 = takeSpeed("speedPFC2.log",posLine);
    s3 = takeSpeed("speedPFC3.log",posLine);
    
    goToDir("src");
    
    if(s1 == -2 || s2 == -2 || s3 == -2){
    	rst = -2;
    
    } else{
	    
	    if(s1 != s2)        
		if(s1 != s3)
		    if(s2 != s3)
		        rst = -1;
		    else
		         rst = 1;
		else
		    rst = 2;
	    else 
		if (s1 != s3)
		    rst = 3;
		
    }    
    return rst;    
    
}
    
    
    
    
    
int main(int argc, char *argv[]) {
    
    sleep(3);
    
    int i = 0;              
    int toSend;
    char toPrint[50];
    
    while(TRUE){
        
        
        sleep(1);
        
        i++;
        toSend = compareSpeeds(i);
        
    
        printf("\n %i° controllo \n",i);
        fflush(stdout);
        switch (toSend){
            
            case 0: sprintf(toPrint," __OK__ \n");break;
            case 1: sprintf(toPrint," __ERRORE__ PFC1 velocita' discorde \n"); break;
            case 2: sprintf(toPrint," __ERRORE__ PFC2 velocita' discorde \n"); ; break;
            case 3: sprintf(toPrint," __ERRORE__ PFC3 velocita' discorde \n"); ; break;
            case -1: sprintf(toPrint," __EMERGENZA__ \n"); break;
            case -2: sprintf(toPrint," __TERMINAZIONE__ \n"); break;
            
        }
        
        printMessage(toPrint);
               
        namedPipe(toSend); 
        
       
         
        
    }  
    
        
    
    return 0;
}    

    
