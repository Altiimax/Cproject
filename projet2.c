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
	
	//variables 
	voiture* mem;
	int shmid;
	key_t key;
    key = 7816;
	
	//Partie sémaphore
	//fin partie sémaphore
	
	//Partie shared memory
	shmid= shmget(key, 3*sizeof(voiture), 0666 | IPC_CREAT);
	if(shmid<0){
				printf("ERREUR: la création du segment a échoué\n");
				exit(1);
	}
	
	mem = shmat(shmid, 0, 0);
	if(mem==(void *)-1){			//verification 
		printf("shmat a echoue\n");
		exit(1);
	}
	// fin partie shared memory
	
	//creation de 3 fils
	for(int i=0;i<3;i++){
		pid=fork(); //création du fork
		if(pid==-1){
			printf("impossible de créer un fils\n");
			exit(1);
		}
		if(pid==0){
			voiture voit;
			voit.idV = i;
			//Ecriture dans la variable à transmettre "voit"
			int rand_a_b(int a, int b){							
				return rand()%(b-a) +a;																		
			}
			srand(time(0));
			voit.S1=rand_a_b(20,40);
			voit.S2=rand_a_b(20,40);
			voit.S3=rand_a_b(20,40);
			
			memcpy(mem+i, &voit, sizeof(voit));
			printf("la voiture est composée de: %d, %d, %d\n",mem[i].S1, mem[i].S2, mem[i].S3);
			
			/*
			 * detachement au segment
			 */
			if(shmdt(mem)==-1){
				perror("detachement impossible\n") ;
				exit(1);
			}
			
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
	for (int i=0;i<3;i++){
		sleep(3);
		printf("je suis le père\nJe lis ce que le fils a mis: %d, %d, %d---\n",mem[i].S1, mem[i].S2, mem[i].S3);
	}
	if(shmdt(mem)==-1){
				perror("detachement impossible\n") ;
				exit(1) ;
			}
}
