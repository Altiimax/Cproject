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
	int pid;
	voiture tabVoitures[20];
	initShm();
	semInit();
	attShm();
	
	//creation de 20 fils
	for(int i=0;i<20;i++){
		pid=fork(); //création du fork
		if(pid==-1){
			printf("impossible de créer un fils\n");
			exit(1);
		}
		if(pid==0){ 
			voiture voit;
			voit.idV = getpid();
			srand(time(0)^(getpid()));
			//Ecriture dans la variable à transmettre "voit"
			voit.S1=rand_a_b(20,40);
			voit.S2=rand_a_b(20,40);
			voit.S3=rand_a_b(20,40);
			memcpy(&(mem->v[i]), &voit, sizeof(voit));
			printf("CCNuméro %d: %d, %d, %d, %d, initcars = %d\n",i ,mem->v[i].S1, mem->v[i].S2, mem->v[i].S3, mem->v[i].idV, mem->init_cars);
			detShm(shmid, mem);
			sem_wait(&(mem->sem));
			printf("je suis dans la semapore");
			(mem->init_cars)++;
			sem_post(&(mem->sem));
			exit(1);
		}
	}
	while(1){
		sem_wait(&(mem->sem));
		if(mem->init_cars == 19){
			sem_post(&(mem->sem));
			break;
		}
		sleep(1);
		sem_post(&(mem->sem));
	}
	
	system("clear");	
	fprintf(stderr, "Numéro\tS1 \tS2 \tS3 \tTotal\n");
	for (int i=0;i<20;i++){
		tabVoitures[i] = mem->v[i];
	
		/**
		* AFFICHAGE DU PERE
		*/
		tabVoitures[i].total = tabVoitures[i].S1+tabVoitures[i].S2+tabVoitures[i].S3;
		fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \n",tabVoitures[i].idV,tabVoitures[i].S1,tabVoitures[i].S2,tabVoitures[i].S3,tabVoitures[i].total);
	}
	/*ordonnerTableauVoitures(tabVoitures,20);

	for (int i=0;i<20;i++){
		tabVoitures[i] = mem[i];
		sleep(1);
		printf("Après tri: %d---\n",tabVoitures[i].S1);
	}*/
	
	detShm(shmid, mem);
}