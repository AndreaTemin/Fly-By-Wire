#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "goToDir.h"


/*  Permette, partendo da una sotto cartella si Progetto-so, di accedere ad una sottocartella di Progeetto-so passatain input.
    Restituira' -1 se la cartella alla quale si intende accedere non esiste
 */
int goToDir(char * dir){

      
    chdir("..");
//     printf("Dovrei essere in /Progetto-so\n");
    
    if(chdir(dir) < 0) {
        
        perror("Errore");
        return -1;
    }
    
//     printf("yeee sono entrato in %s\n",dir);
    return 0;
    
}
