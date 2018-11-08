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

int main(){
	int pid;
	typedef struct
	{
		int idV;
		int S1;
		int S2;
		int S3;
		int total;
		bool disq;
	} voiture;
	
	//variable qui sera place dans la shared memory au niveau du fils
	voiture *voit;
	
	// variable qui va recuperer le message dans la shared memory au niveau du pere
	voiture voitTable[20];
	
	voiture* mem;
	int shmid;
	key_t key;
    key = 786;
	shmid= shmget(key, 5*sizeof(voiture), IPC_CREAT | 0666);
	mem = (voiture *)shmat(shmid, 0, 0);
	if(mem==(void *)-1){			//verification 
		printf("shmat a echoue\n");
		exit(1);
	}
	//creation de 5 fils (pour l instant)
	for(int i=0;i<5;i++){
		pid=fork(); //création du fork
		if(pid==-1){
			printf("impossible de créer un fils\n");
			exit(1);
		}
		if(pid==0){
			voit->idV = (int)getpid();
			//Ecriture dans la variable à transmettre "voit"
			int rand_a_b(int a, int b){							
				return rand()%(b-a) +a;																		
			}										                
			srand(time(0));
			voit->S1=rand_a_b(20,40);
			voit->S2=rand_a_b(20,40);
			voit->S3=rand_a_b(20,40);
			printf("la voiture est composée de: %d, %d, %d\n",voit->S1, voit->S2, voit->S3);
			
			
			if(shmid<0){
				printf("ERREUR: la création du segment a échoué\n");
				exit(1);
			}
			
			if(mem==(void *)-1){			//verification 
				printf("shmat a echoue\n");
				exit(1);
			}
			memcpy(mem, voit, sizeof(voit));
			
			/*
			 * detachement au segment
			 */
			if(shmdt(mem)==-1){
				perror("detachement impossible\n") ;
				exit(1) ;
			}
			
			exit(1);
		} else {
			if((mem=shmat(shmid, 0, 0))==(void *)-1){
				perror("shmat");
				exit(1);
			}
			//memcpy(&tableau, mem, sizeof(mem));
			/*for(int i =0; i<5;i++){
				voitTable[i]=*mem;
			}*/
			if(shmdt(mem)==-1){
				perror("detachement impossible\n");
				exit(1);
			}
			printf("je suis le père\nJe lis ce que le fils a mis: %d, %d, %d\n",(*mem).S1, (*mem).S2,(*mem).S3);
			exit(1);
		}
	}
	shmctl(shmid,IPC_RMID,0);
}
