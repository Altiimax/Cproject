#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include "utils.h"
	
int main(){
	//variables
	voiture tabVoitures[20];
	voiture disqVoit[20];
	/*for(int i =0; i<20;i++){
		meilleur[i] = 0;
	}*/
	initShm();
	attShm();
	if( (sem_init(&(mem->sem), 1, 1)) < 0 ) {	
		perror("erreur semget");
		exit(1);
	}
	genUnTour(tabVoitures);
	afficheUnTour(tabVoitures);
}