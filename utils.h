	
// Structures
typedef struct{
	int idV; // id de la voiture
	int S1; //temps pris pour le secteur1
	int S2;	//temps pris pour le secteur 2
	int S3; //temps pris pour le secteur3 
	int total; // total des 3 secteurs
	int fini; // si la voiture a fini la course
	bool disq; //boolean, true: la voiture est disqualifié, false: la voiture ne l'est pas
} voiture;

typedef struct { 
	sem_t sem; 
	int init_cars;
	voiture v[20];
} shared_state_t;

/**
* Variable
*/

//int semid;
shared_state_t* mem;
int shmid;
key_t key = 7816;

/**
* tri du tableau
*/
void ordonnerTableauVoitures(voiture tableau[], long tailleTableau/*, int mode*/) {
	voiture rempl1;
	voiture rempl2;
	printf("je vais trier le tableau\n");
	long i,t;
	for(t = 0; t < tailleTableau; t++) {
		for(i=1; i < tailleTableau - 1; i++) { 
			if(tableau[i].S1 > tableau[i+t].S1) { 
				rempl1 = tableau[i];
				rempl2 = tableau[i+t];
				tableau[i] = rempl2;
				tableau[i+t] = rempl1;
			}
		} 
	}
}

/** 
* fonction générant les nombres randoms
*/
int rand_a_b(int a, int b){							
	return rand()%(b-a) +a;																		
}

/**
* Fonction d'initialisation de la mémoire partagée
*/
void initShm(){
	shmid= shmget(key, sizeof(shared_state_t), 0666 | IPC_CREAT);
	if(shmid<0){
		printf("ERREUR: la création du segment a échoué\n");
		exit(1);
	}
}

/**
*Fonction qui permet de s'attacher a la mémoire partagée
*/
void attShm(){
	mem = shmat(shmid, 0, 0);
	if(mem==(void *)-1){			//verification 
		printf("shmat a echoue\n");
		exit(1);
	}
}

/**
*Fonction qui permet de se détacher de la mémoire partagée et la supprimer
*/
void detShm(int shmid, shared_state_t* mem){
	if(shmdt(mem)==-1){
		perror("detachement impossible\n") ;
		exit(1);
	}
	shmctl(shmid,IPC_RMID, NULL);
}

/**
* Sémaphore partie
*/

/*void semInit(shared_state_t* stru){
	if( (sem_init(stru->sem, 1, 1)) < 0 ) {	
		perror("erreur semget");
		exit(1);
	}
}*/

