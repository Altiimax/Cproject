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


typedef struct
	{
		int idV;
		double S1;
		double S2;
		double S3;
		int total;
		bool disq;
	} voiture;
	
void ordonnerTableauVoitures(voiture tableau[], long tailleTableau, char secteur) {
	long i,t,k=0;
	char sec[2];
	for(t = 1; t < tailleTableau; t++) {
		for(i=0; i < tailleTableau - 1; i++) { 
			if(tableau[i].secteur > tableau[i+1].secteur) { 
				k= tableau[i].secteur - tableau[i+1].secteur; 
				tableau[i].secteur -= k; 
				tableau[i+1].secteur += k;
				tableau[i+1].secteur += k;
			}
		} 
	} 
}
	
int main(){
	int pid;
	//variables 
	voiture* mem;
	voiture tabVoitures[3];
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
			voit.S1=rand_a_b(20.0,40.0);
			voit.S2=rand_a_b(20.0,40.0);
			voit.S3=rand_a_b(20.0,40.0);
			
			memcpy(mem+i, &voit, sizeof(voit));
			printf("la voiture est composée de: %f, %f, %f\n",mem[i].S1, mem[i].S2, mem[i].S3);
			
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
		tabVoitures[i] = mem[i];
		sleep(2);
		printf("je suis le père\nJe lis ce que le fils a mis: %f, %f, %f---\n",mem[i].S1, mem[i].S2, mem[i].S3);
	}
	
	ordonnerTableauVoitures(tabVoitures,3,"S1");
	ordonnerTableauVoitures(tabVoitures,3,"S2");
	ordonnerTableauVoitures(tabVoitures,3,"S3");

	
	if(shmdt(mem)==-1){
		perror("detachement impossible\n") ;
		exit(1) ;
		}
}