#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#include "goToDir.h"
#include "status.h"

#define MAX_BUF 10


void terminateFamilygroup(){
    int groupPid = getpgid (getppid());
    
    int  killReturn = killpg( groupPid, SIGKILL);  // Kill child process group    

    if(killReturn == -1) {

        if( errno == ESRCH){      // pid does not exist

            printf("Group does not exist!\n");

        }

        else if( errno == EPERM){ // No permission to send signal

            printf( "No permission to send signal!\n" );
        }
        else
            printf( "Signal sent. All Ok!\n" );
    }
    
}
void printMessage(char * mess){
    
    FILE * pf;
    
    
    goToDir("log");
    
    pf = fopen("switch.log","a");

    if(fputs(mess,pf)<1)
        perror("Errore");
    
    fclose(pf);
    fflush(pf);
    
    fflush(stdout);
    
    goToDir("src");
    
}



void menageErrors(int id){
    char buff[15];
    char printOnFile[60];
    
    if(id > 0){
                
        readStatus(id,buff);
        sprintf(printOnFile,"ERRORE: PFC%i e' in stato di %s \n",id,buff);
        printMessage(printOnFile);
        
    }
    
    if(id == -1){
        printMessage("EMERGENZA: Il programma viene terminato  \n");
        terminateFamilygroup();
    }
    
    if(id == -2){
        printMessage("ARRIVO: Il programma viene terminato  \n");
        terminateFamilygroup();
    }
}


void readPipe(char* buf){
    int fd;
    char * myfifo = "/tmp/myfifo";
    
    strcpy(buf, "");     
     /* open, read, and display the message from the FIFO */
    fd = open(myfifo, O_RDONLY);
    read(fd, buf, MAX_BUF);
        
    close(fd);
  
    sprintf(buf,"%s\n",buf);
}






int main(int argc, char **argv) {   
    pid_t pid;
    int id;
    int i = 1;
    char buf[MAX_BUF];
    sleep(3);
    
    while(1){
        sleep(1);

        
        readPipe(buf);
        id = atoi(buf);
             
        menageErrors(id); 
        
        //sleep(1);
        
    }
    
    
    return 0;
}
