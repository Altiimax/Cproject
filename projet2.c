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
	attShm();
	if( (sem_init(&(mem->sem), 1, 1)) < 0 ) {	
		perror("erreur semget");
		exit(1);
	}
	
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
			printf("je vais utiliser la semaphore");
			if( (sem_wait(&(mem->sem))) < 0 ) {	
				perror("erreur semget");
				exit(1);
			}
			printf("je suis dans la semapore");
			(mem->init_cars)++;
			if( (sem_post(&(mem->sem))) < 0 ) {	
				perror("erreur semget");
				exit(1);
			}
			exit(1);
		}
	}
	while(1){
		printf("je vais utiliser la semaphore");
		if( (sem_wait(&(mem->sem))) < 0 ) {	
			perror("erreur semget");
			exit(1);
		}
		if(mem->init_cars == 19){
			printf("je vais utiliser la semaphore");
			sem_post(&(mem->sem));
			break;
		}else{
			printf("je vais utiliser la semaphore");
			sem_post(&(mem->sem));
			sleep(1);
		}
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