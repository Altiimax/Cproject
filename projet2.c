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
	int pid;
	//variables 
	voiture* mem;
	voiture tabVoitures[20];
	int shmid;
	key_t key = 7816;
	shmid= shmget(key, 20*sizeof(voiture), 0666 | IPC_CREAT);
	if(shmid<0){
		printf("ERREUR: la création du segment a échoué\n");
		exit(1);
	}
	mem = shmat(shmid, 0, 0);
	if(mem==(void *)-1){			//verification 
		printf("shmat a echoue\n");
		exit(1);
	}
	
	//creation de 3 fils
	for(int i=0;i<20;i++){
		pid=fork(); //création du fork
		if(pid==-1){
			printf("impossible de créer un fils\n");
			exit(1);
		}
		if(pid==0){ 
			sleep(1);
			voiture voit;
			voit.idV = i;
			srand(time(0)^(getpid()));
			//Ecriture dans la variable à transmettre "voit"
			voit.S1=rand_a_b(20,40);
			voit.S2=rand_a_b(20,40);
			voit.S3=rand_a_b(20,40);
			memcpy(mem+i, &voit, sizeof(voit));
			printf("la voiture est composée de: %d, %d, %d\n",mem[i].S1, mem[i].S2, mem[i].S3);
			
			detShm(shmid, mem);
			
			exit(1);
		/*} else {
			sleep(1);
			printf("je suis le père\nJe lis ce que le fils a mis: %d---\n",mem[i].S1);
			
			if(shmdt(mem)==-1){
				perror("detachement impossible\n");
				exit(1);
			}
			exit(1);
		}	*/
		}
	}
	for (int i=0;i<20;i++){
		tabVoitures[i] = mem[i];
		sleep(2);
		printf("je suis le père\nJe lis ce que le fils a mis: %d, %d, %d---\n",tabVoitures[i].S1,tabVoitures[i].S2,tabVoitures[i].S3);
	}
	
	/*ordonnerTableauVoitures(tabVoitures,20);

	for (int i=0;i<20;i++){
		tabVoitures[i] = mem[i];
		sleep(1);
		printf("Après tri: %d---\n",tabVoitures[i].S1);
	}*/
	
	detShm(shmid, mem);
}