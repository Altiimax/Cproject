﻿#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
//#define VOITURE44 , VOITURE77, VOITURE5, VOITURE7, VOITURE3, VOITURE33, VOITURE31, VOITURE18, VOITURE35, VOITURE27, VOITURE55, VOITURE10, VOITURE28, VOITURE8, VOITURE20, VOITURE2, VOITURE14, VOITURE9, VOITURE16, VOITURE11

int main(int argc, char *argv[]){
	int voiture1 = courseDeVoitures(44);
	int voiture2 = courseDeVoitures(77);
}
int courseDeVoitures(int numVoiture){
int temps;
	int tableau[4];
	int S1 = 0;
	int S2 = 0;
	int S3 = 0;
	bool dq = false;
	int dqchance = 0;
	int total;
	//pid_t pid;
	int rand_a_b(int a, int b){
        return rand()%(b-a) +a;
	}
	//pid = fork();
	//if(pid==0){
		int i;
		srand(time(0));
		for(i=0; i<3; i++)
		{
			tableau[i] = rand_a_b(20,40);
		}
		temps = tableau[0]+tableau[1]+tableau[2];
		tableau[3] = temps;
	//}else{
		do{
		  system("clear");
		  fprintf(stderr, "Numéro            S1            S2            S3            Total");
		  if(S1!= tableau[0]){
			  if (S2!= tableau[1]){
				  if (S3!= tableau[2]){
					  ++S1;
					  ++S2;
					  ++S3;
				  } else {
					  ++S1;
					  ++S2;
				  }
			  } else {
				  if (S3!= tableau[2]){
					  ++S1;
					  ++S3;
				  } else {
					  ++S1;
				  }
			  }
		  } else {
			  if (S2!= tableau[1]){
				  if (S3!= tableau[2]){
					  ++S2;
					  ++S3;
				  } else {
					  ++S2;
				  }
			  } else {
				  if (S3!= tableau[2]){
					  ++S3;
				  }
			  }
		  }
		  total = S1+S2+S3;
		  dqchance = rand_a_b(0,1000);
		  if(dq==true){
			  fprintf(stderr, "\n  44              DQ            DQ            DQ            DQ \n");
		  } else {
			  fprintf(stderr, "\n  44              %d            %d            %d            %d \n",S1,S2,S3,total);
		  if(dqchance<=5){
			  dq = true;
		  }
		  if(dqchance>5){
			  dq = false;
		  }
		  }
		  sleep(1);
		}while(S1!= tableau[0] || S2 != tableau[1] || S3 != tableau[2]);
        
	//}
	return tableau;
}