// Structures
typedef struct{
    int numero; //numéro de la voiture en réalité
    int idV; // id de la voiture
    int S1; //temps pris pour le secteur1
    int S2;    //temps pris pour le secteur 2
    int S3; //temps pris pour le secteur3 
    int total; // total des 3 secteurs
    int meilleurTour; //meilleur tour de la voirute parmi tous ceux qui ont déjà été courrus
    int totalTours;//temps total de la voiture depuis son départ à un moment donné (le maximum étant donné en fonction de quelle course on fait)
    int nombreTours; //le nombre de tours déjà parcourus par la voirure 
    bool dq;  //boolean, true: la voiture est disqualifié, false: la voiture ne l'est pas
    int dqchance; //le pourcentage de chance de se faire disqualifier
    bool fini; // si la voiture a fini la course
} voiture;

typedef struct { 
	sem_t sem; 
	int init_cars;
	int fini;
	voiture v[20];
} shared_state_t;

/**
* Variable
*/

int num[20]={44, 77, 5, 7, 3, 33, 11, 31, 18, 35, 27, 55, 10, 28, 8, 20, 2, 14, 9, 16};
int S1=0, S2=0, S3=0, total=0, meilleur[20];
int totaltab[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
shared_state_t* mem;
int shmid;
int pid;
key_t key = 7816;

/** 
* fonction générant les nombres randoms
*/
int rand_a_b(int a, int b){							
	return rand()%(b-a) +a;																		
}

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
* Affichage lorsque la voiture a fini son tour
*/

void affichage(voiture tableau[], int j, int sec){
	tableau[j].dqchance = rand_a_b(0,100000);
	if( sec == 0){
		total = S1+S2+S3;
		if(tableau[j].dq==true){
			fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \t%d\n", tableau[j].numero, tableau[j].meilleurTour);
		} else {
			fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \t%d\n",tableau[j].numero,S1,S2,S3,total, tableau[j].meilleurTour);
			if(tableau[j].dqchance<=5){
				tableau[j].dq = true;
			}
			if(tableau[j].dqchance>5){
				tableau[j].dq = false;
			}
		}
	} else if(sec == 1){
		total = tableau[j].S1+S2+S3;
		if(tableau[j].dq==true){
			fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \t%d \n", tableau[j].numero,tableau[j].meilleurTour);
		} else {
			fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \t%d \n",tableau[j].numero,tableau[j].S1,S2,S3,total,tableau[j].meilleurTour);
			if(tableau[j].dqchance<=5){
				tableau[j].dq = true;
			}
			if(tableau[j].dqchance>5){
				tableau[j].dq = false;
			}
		}
	} else {
		total = tableau[j].S1+tableau[j].S2+S3;
		if(tableau[j].dq==true){
			fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \t%d\n", tableau[j].numero,tableau[j].meilleurTour);
		} else {
			fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \t%d \n",tableau[j].numero,tableau[j].S1,tableau[j].S2,S3,total,tableau[j].meilleurTour);
			if(tableau[j].dqchance<=5){
				tableau[j].dq = true;
			}
			if(tableau[j].dqchance>5){
				tableau[j].dq = false;
			}
		}
	}
}

/**
* Affichage lorsque la voiture a fini son tour
*/

void affichageFini(voiture tableau[], int j, int sec){
	tableau[j].dqchance = rand_a_b(0,100000);
	if( sec == 0){
		total = tableau[j].S1+S2+S3;
		if(tableau[j].dq==true){
			fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \t%d\n", tableau[j].numero,tableau[j].meilleurTour);
		} else {
			fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \t%d\n",tableau[j].numero,tableau[j].S1,S2,S3,total,tableau[j].meilleurTour);
			if(tableau[j].dqchance<=5){
				tableau[j].dq = true;
			}
			if(tableau[j].dqchance>5){
				tableau[j].dq = false;
			}
		}
	} else if(sec == 1){
		total = tableau[j].S1+tableau[j].S2+S3;
		if(tableau[j].dq==true){
			fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \t%d\n", tableau[j].numero,tableau[j].meilleurTour);
		} else {
			fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \t%d\n",tableau[j].numero,tableau[j].S1,tableau[j].S2,S3,total,tableau[j].meilleurTour);
			if(tableau[j].dqchance<=5){
				tableau[j].dq = true;
			}
			if(tableau[j].dqchance>5){
				tableau[j].dq = false;
			}
		}
	} else {
		total = tableau[j].S1+tableau[j].S2+tableau[j].S3;
		if(tableau[j].dq==true){
			fprintf(stderr, "\n  %d \tDQ \tDQ \tDQ \tDQ \t%d\n", tableau[j].numero,tableau[j].meilleurTour);
		} else {
			fprintf(stderr, "\n  %d \t%d \t%d \t%d \t%d \t%d\n",tableau[j].numero,tableau[j].S1,tableau[j].S2,tableau[j].S3,total,tableau[j].meilleurTour);
			if(tableau[j].dqchance<=5){
				tableau[j].dq = true;
			}
			if(tableau[j].dqchance>5){
				tableau[j].dq = false;
			}
		}
	}
}

/**
* remettre a 0 les variables utiles pour l affichage
*/

void reset(voiture tableau[]){
	S1=0, S2=0, S3=0, total=0;
	mem->fini = 0;
	for (int j=0; j<20; j++){
		tableau[j].fini =false;
	}
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

void genUnTour(voiture tabVoitures[]){
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
	sleep(3);
	for (int i=0;i<20;i++){
		tabVoitures[i] = mem->v[i];
		tabVoitures[i].numero = num[i];
		for(int i=0; i<20; i++){
			tabVoitures[i].meilleurTour = meilleur[i];
		}
		tabVoitures[i].total = tabVoitures[i].S1+tabVoitures[i].S2+tabVoitures[i].S3;
	}
}

/**
* Affichage D'un tour complet!
*/

void afficheUnTour(voiture tabVoitures []){
	reset(tabVoitures);
	while(mem->fini != 20){
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal \tMeilleur tour");
		++S1;
		for(int j=0; j<20; j++){
			sem_wait(&(mem->sem));
			if (S1 == tabVoitures[j].S1){
				tabVoitures[j].fini = mem->fini ;
				affichageFini(tabVoitures, j, 0);
				++(mem->fini);
			} else if (tabVoitures[j].fini != 0){ 
				affichageFini(tabVoitures, j, 0);
			} else {
				affichage(tabVoitures, j, 0);
			}
			sem_post(&(mem->sem));
		}
		usleep(1000*100);
	}
	reset(tabVoitures);
	while(mem->fini != 20){
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal \tMeilleur tour");
		++S2;
		for(int j=0; j<20; j++){
			sem_wait(&(mem->sem));
			if (S2 == tabVoitures[j].S2){
				tabVoitures[j].fini = true ;
				affichageFini(tabVoitures, j, 1);
				++(mem->fini);
			} else if (tabVoitures[j].fini){ 
				affichageFini(tabVoitures, j, 1);
			} else {
				affichage(tabVoitures, j, 1);
			}
			sem_post(&(mem->sem));
		}
		usleep(1000*100);
	}
	reset(tabVoitures);
	while(mem->fini != 20){
		system("clear");
		fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal \tMeilleur tour");
		++S3;
		for(int j=0; j<20; j++){
			sem_wait(&(mem->sem));
			if (S3 == tabVoitures[j].S3){
				tabVoitures[j].fini = mem->fini ;
				affichageFini(tabVoitures, j, 2);
				++(mem->fini);
			} else if (tabVoitures[j].fini){ 
				affichageFini(tabVoitures, j, 2);
			} else {
				affichage(tabVoitures, j, 2);
			}
			sem_post(&(mem->sem));
		}
		usleep(1000*100);
	}
	for(int i=0;i<20;i++){
		sem_wait(&(mem->sem));
		if((meilleur[i] > tabVoitures[i].total || tabVoitures[1].meilleurTour == 0) && tabVoitures[i].dq==0){
			meilleur[i] = tabVoitures[i].total;
		} else {
			meilleur[i] = tabVoitures[i].meilleurTour;
		}
		sem_post(&(mem->sem));
	}
	for(int i=0;i<20;i++){
		sem_wait(&(mem->sem));
		tabVoitures[i].meilleurTour = meilleur[i];
		totaltab[i] += tabVoitures[i].total;
		sem_post(&(mem->sem));
	}
	system("clear");
	fprintf(stderr, "Numéro \tS1 \tS2 \tS3 \tTotal \tMeilleur tour");
	for(int j = 0; j<20;j++){
		affichageFini(tabVoitures, j, 2);
	}
	detShm(shmid, mem);
}

