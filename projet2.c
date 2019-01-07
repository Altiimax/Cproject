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
	
int main(int argc, char argv[]){
	//variables
	voiture tabVoitures[20];
	voiture disqVoit[20];
	bool fin = true;
	while(fin){
		initShm();
		attShm();
		if( (sem_init(&(mem->sem), 1, 1)) < 0 ) {	
			perror("erreur semget");
			exit(1);
		}
		//mem->init_cars = 0;
		genUnTour(tabVoitures);
		if(tabVoitures[1].meilleurTour == 0){
			for(int i =0; i<20;i++){
				sem_wait(&(mem->sem));
				tabVoitures[i].nombreTours = 0;
				sem_post(&(mem->sem));
			}
		}
		afficheUnTour(tabVoitures);
		for(int i=0;i<20; i++){
			fprintf(stderr,"total = %d\n", totaltab[i]);
			tabVoitures[i].nombreTours++;
			if(totaltab[i] >= 350){
				fin= false;
			}
		}
	}
	fprintf(stderr,"\nFIN\n");
}