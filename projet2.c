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
	voiture finiVoit[20];
	voiture disqVoit[20];
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
			voit.dqchance = 0;
			voit.dq = false;
			voit.fini = 0;
			memcpy(&(mem->v[i]), &voit, sizeof(voit));
			if( (sem_wait(&(mem->sem))) < 0 ) {	
				perror("erreur semget");
				exit(1);
			}
			(mem->init_cars)++;
			if( (sem_post(&(mem->sem))) < 0 ) {	
				perror("erreur semget");
				exit(1);
			}
			printf("Numéro %d: %d, %d, %d, %d, initcars = %d\n",i ,mem->v[i].S1, mem->v[i].S2, mem->v[i].S3, mem->v[i].idV, mem->init_cars);
			detShm(shmid, mem);
			exit(1);
		}
	}
	while(1){
		if( (sem_wait(&(mem->sem))) < 0 ) {	
			perror("erreur semget");
			exit(1);
		}
		if(mem->init_cars == 19){
			sem_post(&(mem->sem));
			break;
		}else{
			sem_post(&(mem->sem));
			sleep(1);
		}
	}
	
	system("clear");	
	fprintf(stderr, "Numéro\tS1 \tS2 \tS3 \tTotal\n");
	for (int i=0;i<20;i++){
		tabVoitures[i] = mem->v[i];
		tabVoitures[i].total = tabVoitures[i].S1+tabVoitures[i].S2+tabVoitures[i].S3;
		fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \n",tabVoitures[i].idV,tabVoitures[i].S1,tabVoitures[i].S2,tabVoitures[i].S3,tabVoitures[i].total);
	}
	reset(tabVoitures);
	while(mem->fini != 20){
		//sem_wait(&(mem->sem));
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
		++S1;
		for(int j=0; j<20; j++){
			sem_wait(&(mem->sem));
			if (S1 == tabVoitures[j].S1){
				tabVoitures[j].fini = mem->fini ;
				//finiVoit[(tabVoitures[j].fini)] = tabVoitures[j];
				affichageFini(tabVoitures, j, 0);
				++(mem->fini);
			} else if (tabVoitures[j].fini != 0){ 
				affichageFini(tabVoitures, j, 0);
			} else {
				affichage(tabVoitures, j, 0);
			}
			sem_post(&(mem->sem));
		}
		fprintf(stderr, "VALEUR DE FIN: %d", mem->fini);
		sleep(0.7);
	}
	for (int i=0;i<20;i++){
		tabVoitures[i] = mem->v[i];
		tabVoitures[i].total = tabVoitures[i].S1+tabVoitures[i].S2+tabVoitures[i].S3;
		fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \n",tabVoitures[i].idV,tabVoitures[i].S1,tabVoitures[i].S2,tabVoitures[i].S3,tabVoitures[i].total);
	}
	fprintf(stderr, "RESET");
	reset(tabVoitures);
	while(mem->fini != 20){
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
		++S2;
		fprintf(stderr,"\nSecteur 2 du deuxième bool: %d\nS2: %d\n",tabVoitures[1].fini, S2);
		for(int j=0; j<20; j++){
			sem_wait(&(mem->sem));
			if (S2 == tabVoitures[j].S2){
				tabVoitures[j].fini = true ;
				//finiVoit[(tabVoitures[j].fini)] = tabVoitures[j];
				affichageFini(tabVoitures, j, 1);
				++(mem->fini);
			} else if (tabVoitures[j].fini){ 
				affichageFini(tabVoitures, j, 1);
			} else {
				affichage(tabVoitures, j, 1);
			}
		}
		sleep(1);
	}
	reset(tabVoitures);
	while(mem->fini != 20){
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
		++S3;
		for(int j=0; j<20; j++){
			sem_wait(&(mem->sem));
			if (S3 == tabVoitures[j].S3){
				tabVoitures[j].fini = mem->fini ;
				//finiVoit[(tabVoitures[j].fini)] = tabVoitures[j];
				affichageFini(tabVoitures, j, 2);
				++(mem->fini);
			} else if (tabVoitures[j].fini){ 
				affichageFini(tabVoitures, j, 2);
			} else {
				affichage(tabVoitures, j, 2);
			}
			sem_post(&(mem->sem));
		}
		sleep(1);
	};
	detShm(shmid, mem);
}