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
			memcpy(&((mem+i)->v[i]), &voit, sizeof(voit));
			printf("la voiture est composée de: %d, %d, %d, %d\n",mem[i].v[i].S1, mem[i].v[i].S2, mem[i].v[i].S3, mem[i].v[i].idV);
			detShm(shmid, mem);
			sem_wait(&sem);
			mem->init_cars++;
			sem_post(&sem);
		}
	}
	while(1){
		sem_wait(&sem);
		if(mem->init_cars == 19){
			sem_post(&sem);
			break;
		}
		sleep(1);
		sem_post(&sem);
	}
	
	system("clear");	
	fprintf(stderr, "Numéro\tS1 \tS2 \tS3 \tTotal\n");
	for (int i=0;i<20;i++){
		tabVoitures[i] = mem[i].v[i];
	
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