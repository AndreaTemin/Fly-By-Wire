#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "functions.h"

#define pi   M_PI  							// Definisco pigreco dato che è già dentro la libreria math.h

/*
  Calcolo il raggio della terra in base alla latitudine di Genova.
  (dato che, tutte le nostre coordinate sono situate in quella zona).

*/
double calcuReadiusEarth(){
	double latGenova = 44.4222; 					// Gia' convertito in gradi
	double readiusPoles = 3949.903;
	double readiusEquator = 3949.903;
	double partA = (pow(pow(readiusEquator, 2) * cos(latGenova), 2) + pow(pow(readiusPoles, 2) * sin(latGenova), 2));
	double partB = (pow(readiusEquator * cos(latGenova), 2) + pow(readiusPoles * sin(latGenova), 2));
	double reaEar = sqrt(partA /partB);	
	

	return reaEar;
}


/*
  Calcolo la distanza prendendo i dati ricavati dai metoti precedenti (latitudine e longitudine di entrambe le coordinate).
*/
double distance(double lat1, double long1, double lat2, double long2) {
									/*La formula completa sarebbe: 
									   dlat = (lat2 - lat1) * radiante (dr2)
									   dlong = (long2 - long1) * radiante
									   a = sin^2(dlat/2) + cos(lat1 * radiante) * cos(lat2 * radiante) * sin^2(dlon/2)
									   c = 2 * atan2( sqrt(a), sqrt(1-a))
									   d = raggioTerrestre(6371) * c
									*/
	
	double readiusEarth = calcuReadiusEarth(); 																			
	double dlat = ((lat2 - lat1) * pi)/180.0 ;								
	double dlong = ((long2 - long1) * pi)/180.0;
	double a = pow(sin(dlat / 2.0), 2) + cos((lat1 * pi)/180.0) * cos((lat2 * pi)/180.0) * pow(sin(dlong / 2.0), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = readiusEarth * c;

	return d;
}


/*
  Effettuo il calcolo del tempo convertendolo in secondi.
*/
double calcoloTmp(int time){			
	int ore;
	int minuti;
	int secondi;	
									//La strutta del tempo nelle coordinate GPS e': hhmmss, dunque li devo ricavare singolarmente
	
	ore = time / 10000;
	time = time - (ore*10000);
	
	minuti = time/ 100;
	secondi = time - (minuti * 100);	

	double tempo =(ore * 3600) + (minuti * 60) + secondi;	// Ricordare che: 1h = 3600s
									//                1m = 60S
	return tempo;
}


/*
  Il metodo permette di ricavare il tempo della stringa passata in input.
*/
double tempoGLL(char str[]){										
	char timeA[6];
	int time;
	int k = 0;
	int i;
	
	for(i = 0; i < strlen(str); i++){				// Arrivo alla posizione in cui si trova la direzione della longitudine
	
		if(str[i] == 'E' || str[i] == 'W'){
			i = i + 2;					// Effettuo due spostamenti in avanti, perche' devo arrivare alla prima posione del tempo
									// (siccome nel mezzo di trova la virgola per separare i dati).
			break;
		}
	}
							   	
	while(str[i] != ','){						// Inserisco i valori del tempo nell'array time
		timeA[k] = str[i];
		k++;
		i++;
	
	}
	time = atoi(timeA);						// Tramite la funzione atoi(const char *stringa) converto le stringhe in interi
	
	
	double tmp = calcoloTmp(time);				// Effettuo il calcolo del tempo

	return tmp;
}


/*
  Utilizzato nei metoti "latitudine" e "longitudine" per effettuare il calcolo effettivo della 
  latitudine e longitudine.
*/
double calcoloLateLong(char str[], int i, char dir) {		
	i = i - 2;
	char strMinuti[7];						// Creo una stringa di 7 positizioni per inserire i minuti della latitudine/longitudine
	
	double minuti;
	int gradi; 
	int tmp;
	
	int j = 6;
	int k = 1;
	while(j != -1){						// Ricavo i minuti della latitudine/longitudine
	
		strMinuti[j] = str[i];
		i = i - 1;
		j = j - 1;

			
	}
	
	minuti = atof(strMinuti);					// Converto la stringa di caratteri in double (La funzione atoi/atof si trova nella libreria <stdlib.h>)
					
	minuti = minuti / 60.0;					// Divido i minuti per 60, perche' cosi' vengono convertiti in nodi
	
	
	if(dir == 'N' || dir == 'S'){
		
		gradi = 10;						// Inizializzo a 10, perche' nella latitudine i gradi sono rappresentati da due valori
		i = i - 2;						// Mi sposto di due posizioni per arrivare alla prima posizione del grado
		tmp = 48 - (int)(str[i]);				// Ricavo il primo valore del grado
		gradi = gradi * tmp;
		i++;
		tmp = (int)(str[i]) - 48;				// Ricavo il secondo valore del grado
		gradi += tmp;
	}else{
	
		gradi = (int)(str[i]) - 48;				// Converto una stringa contenente il grado della longitudine in intero
	
	}
	
	return gradi + minuti;						// Corrisponde alla Lat/Long finale

}


/*
  Il metodo permette di ricavare la longitudine della stringa passata in input.

*/
double longitudine(char str[]) {					
	char dir = 0;							//Devo inizializzarlo, altrimenti mi genera errore in "calcoloLateLong(str, i, dir)"
	int i;

	for(i = 6; i < strlen(str); i++){					

		if (str[i] == 'E' || str[i] == 'W'){

			dir = str[i];					// Salvo la direzione, perche' servira' per capire se la longitudine e' negativa o positiva
			break;
		}
	}

	double lon = calcoloLateLong(str, i, dir);			// Effettuo il calcolo, chiamando il metodo "calcoloLateLong"

	if (dir == 'W'){						//Nel caso in cui la direzione sia Ovest(West), la longitudine e' negativa, altrimenti positiva.
		lon = -lon;
	}

	return lon;
}

/*
  Il metodo permette di ricavare la latitudine della stringa passata in input.

*/
double latitudine(char str[]) {					// Ricavo la latitudine dei segnali GPSGLL
	char dir = 0;							//Devo inizializzarlo, senno' segnala errore in "calcoloLateLong(str, i, dir);"
	int i;

	for(i = 6; i < strlen(str); i++){		

		if (str[i] == 'N' || str[i] == 'S') {

			dir = str[i];					// Salvo la direzione, perche' servira' per capire se la latitudine e' positiva o negativa				
			break;						// Ferma il for ed esce
		}

	}

	double lat = calcoloLateLong(str, i, dir);			// Calcolo la latitudine, chiamando il metodo "calcoloLateLong"

	if (dir == 'S') {						// Se la direzione e' SUD, allora la latitudine diventa negativa, altrimenti positiva.
		lat = -lat;
	}

	return lat;
}

