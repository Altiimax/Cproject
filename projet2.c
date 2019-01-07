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
			voit.dqchance = 0;
			voit.dq = false;
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
		tableau[0][i]=tabVoitures[i].S1;
		tableau[1][i]=tabVoitures[i].S2;
		tableau[2][i]=tabVoitures[i].S3;
	}
	for(int i = 0; i<20;i++){
		int S1=0, S2=0, S3=0, total=0;
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
		do{
			//sem_wait(&(mem->sem));
			system("clear");
			fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
			for(int j=0; j<20; j++){
				if (S1 != tabVoitures[j].S1){
					++S1;
				}
				total = S1+S2+S3;
				sem_wait(&(mem->sem));
				tabVoitures[j].dqchance = rand_a_b(0,1000);
				if(tabVoitures[j].dq==true){
					fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \n", tabVoitures[j].S1);
				} else {
					fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \n",tabVoitures[j].S1,S1,S2,S3,total);
					if(tabVoitures[j].dqchance<=5){
						tabVoitures[j].dq = true;
					}
					if(tabVoitures[j].dqchance>5){
						tabVoitures[j].dq = false;
					}
				}
				sem_post(&(mem->sem));
			}
			//sem_post(&(mem->sem));
			sleep(1);
		}while(S1!= tableau[0][i]);
		do{
			//sem_wait(&(mem->sem));
			system("clear");
			fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
			++S2;
			total = S1+S2+S3;
			for(int j=0;j<20;j++){
				sem_wait(&(mem->sem));
				tabVoitures[j].dqchance = rand_a_b(0,1000);
				if(tabVoitures[j].dq==true){
					fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \n", tabVoitures[j].S1);
				} else {
					fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \n",tabVoitures[j].S1,S1,S2,S3,total);
					if(tabVoitures[j].dqchance<=5){
						tabVoitures[j].dq = true;
					}
					if(tabVoitures[j].dqchance>5){
						tabVoitures[j].dq = false;
					}
				}
				sem_post(&(mem->sem));
			}
			sleep(1);
			//sem_post(&(mem->sem));
		}while(S2!= tableau[1][i]);
		do{
			//sem_wait(&(mem->sem));
			system("clear");
			fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal");
			++S3;
			total = S1+S2+S3;
			for(int j=0;j<20;j++){
				sem_wait(&(mem->sem));
				tabVoitures[j].dqchance = rand_a_b(0,1000);
				if(tabVoitures[j].dq==true){
					fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \n", tabVoitures[j].idV);
				} else {
					fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \n",tabVoitures[j].idV,S1,S2,S3,total);
					if(tabVoitures[j].dqchance<=5){
						tabVoitures[j].dq = true;
					}
					if(tabVoitures[j].dqchance>5){
						tabVoitures[j].dq = false;
					}
				}
				sem_post(&(mem->sem));
			}
			sleep(1);
			//sem_post(&(mem->sem));
		}while(S3!= tableau[2][i]);
	}
	detShm(shmid, mem);
}