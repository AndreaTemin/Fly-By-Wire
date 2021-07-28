#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "goToDir.h"

void writeStatus(int id, char *buff){

	char str[15];
	FILE *fp;
	
	
	sprintf(str, "statusPFC%d.tmp", id);
	
	goToDir("tmp");
	
	fp = fopen(str, "w");
	fputs(buff, fp);
	fclose(fp);	

	goToDir("src");

}



void readStatus(int id, char *buff){
	char str[15];
	FILE *fp;
	
	sprintf(str, "statusPFC%d.tmp", id);
	
	goToDir("tmp");
	
	fp = fopen(str, "r");
	fgets(buff, 15, fp);
	fclose(fp);
	
	goToDir("src");

}
