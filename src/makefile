
all: setDir clearall compile

compile: main readG18 Transd errorGenerator wes discSwitch

main: main.o goToDir.o 
	gcc main.o goToDir.o -o main

main.o: main.c goToDir.h 
	gcc -c main.c

Transd: Transducers.o goToDir.o
	gcc Transducers.o goToDir.o -o Transd

Transducers.o: Transducers.c goToDir.h
	gcc -c Transducers.c

readG18: readG18.o researchPid.o functions.o goToDir.o status.o
	gcc readG18.o researchPid.o functions.o goToDir.o status.o -o readG18 -lm

errorGenerator: generatore_fallimenti.o researchPid.o goToDir.o
	gcc generatore_fallimenti.o researchPid.o goToDir.o -o errorGenerator
	
generatore_fallimenti.o: generatore_fallimenti.c researchPid.h goToDir.h
	gcc -c generatore_fallimenti.c

wes: wes.o goToDir.o
	gcc wes.o goToDir.o -o wes

wes.o: wes.c goToDir.h
	gcc -c wes.c

discSwitch: discSwitch.o status.o goToDir.o
	gcc discSwitch.o status.o goToDir.o -o discSwitch

discSwitch.o: discSwitch.c status.h 
	gcc -c discSwitch.c

researchPid.o: researchPid.c goToDir.h researchPid.h
	gcc -c researchPid.c

readG18.o: readG18.c functions.h goToDir.h researchPid.h status.h
	gcc -c readG18.c 

goToDir.o: goToDir.c goToDir.h
	gcc -c goToDir.c

status.o: status.c status.h goToDir.h
	gcc -c status.c

functions.o: functions.c functions.h 
	gcc -c functions.c


setDir:
	cd ..; mkdir -p log; mkdir -p src; mkdir -p tmp
	
	
clearall: clearsrc clearlog cleartmp clean

clearsrc:
	cd ..; cd src; rm -f fileSocket
	
clearlog:
	cd ..; cd log; rm -rf *.log

cleartmp:
	cd ..; cd tmp; rm -rf *.tmp
	
clean:
	rm -rf *.o
