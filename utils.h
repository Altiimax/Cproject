	
// Structures
typedef struct{
	int idV; // id de la voiture
	int S1; //temps pris pour le secteur1
	int S2;	//temps pris pour le secteur 2
	int S3; //temps pris pour le secteur3 
	int total; // total des 3 secteurs
	bool dq;
	int dqchance;
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
//int S1=0, S2=0, S3=0, total=0;
int tableau[3][20];
shared_state_t* mem;
int shmid;
key_t key = 7816;

/**
*Fonction max
*/



/**
* tri du tableau
*/
void ordonnerTableauVoitures(voiture tableau[], long tailleTableau) {
	voiture rempl1;
	int i,t;
	for(t = 0; t < tailleTableau; t++) {
		for(i=1; i < tailleTableau; i++) { 
			if(tableau[t].S1 < tableau[i].S1) { 
				rempl1 = tableau[t];
				tableau[t] = tableau[i];
				tableau[i] = rempl1;
				i--;
			}
		} 
	}
	rempl1=tableau[0];
    for(i=0;i<tailleTableau;i++)
    tableau[i]=tableau[i+1];
    tableau[tailleTableau-1]=rempl1;
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

