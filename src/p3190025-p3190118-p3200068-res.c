#include "p3190025-p3190118-p3200068-res.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>






int Ncust;

int seed;




int seats[30][10]={
	{0,0,0,0,0,0,0,0,0,0},//zoneB	apo i=0
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},	//ews i=19
	{0,0,0,0,0,0,0,0,0,0},//zoneA	apo i=20
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},	//ews i=29

}; 

/*initialize with 0=availiable
	1=taken  */
	
	
unsigned int available_phones=Ntel;
unsigned int available_cash=Ncash;

	
int BankAcount=0;


float OKorders=0.0f;
float seatFails=0.0f;
float payFails=0.0f;	
	
pthread_mutex_t mutextel; 
pthread_cond_t condtel;


pthread_mutex_t mutexzone;

pthread_mutex_t mutextime;
 
pthread_mutex_t mutexprint;


pthread_mutex_t mutexpayment;

pthread_mutex_t mutexcash; 
pthread_cond_t condcash;

int sucCust=0;


int totalphonetime;
int totaltime;
int phase2cust=0;




void* routine(void* threadid){
		
		int rc;
		int tid = *((int*)(threadid));
		
		int seed2 = seed + tid; //diaforopoiei ta seed tou kathe thread
		struct timespec before;
		clock_gettime(CLOCK_REALTIME,&before);
		
		
		
		rc = pthread_mutex_lock(&mutextel);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(0);
		}
		
		
		while (available_phones == 0){
			rc = pthread_cond_wait( &condtel,&mutextel);
			if (rc != 0) {
				printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
				pthread_exit(0);
			}
		}
		
		available_phones--;
		rc = pthread_mutex_unlock(&mutextel);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(0);
		}
		
		
		//arxh paraggelias
		
		
		struct timespec after;
		clock_gettime(CLOCK_REALTIME,&after);
		int time_elapsed=after.tv_sec-before.tv_sec;

		rc = pthread_mutex_lock(&mutextime);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(0);
		}
		
		totaltime=totaltime+time_elapsed;
		
		rc = pthread_mutex_unlock(&mutextime);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(0);
		}
		
		int r=rand_r(&seed2)%100;
		int zone; 		//zone
		int tickets=rand_r(&seed2) % (Nseathigh +1 - Nseatlow)+Nseatlow;	//random number of tickets
		
		
		if(r<=PzoneA){		//epilogh zwnhs me bash thn pithanothta
			zone=1;
		}else{
			zone=2;
		}
		
		sleep(rand_r(&seed2) % (tseathigh +1 - tseatlow)+tseatlow);	//xronos poy o thlefwnhths exetazei an yparxoyn synexomenes theseis
		
		rc = pthread_mutex_lock(&mutexzone);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(0);
		}
		
		
		int found=0;
		int seira;
		int theseis[tickets];
		int rowfirst;
		int rowlast;
		if(zone==2){
			rowfirst=0;
			rowlast=19;
			}
		if(zone==1){
			rowfirst=20;
			rowlast=29;
			}	
		for(int x=rowfirst;x<=rowlast&&found==0;x++){
			for(int y=0;y<=9-(tickets-1)&&found==0;y++){
				int sum=0;
				int rip=0;
				for(int k=y;(k<=y+tickets-1)&&rip==0&&found==0;k++){
						if (seats[x][k]==0){
							sum++;
						}else{
							rip=1;
						}
						if(sum==tickets){
							found=1;
							seira=x;
							int y2=y;
							for(int t=0;t<tickets;t++){
								theseis[t]=y2;
								seats[x][y2]=tid;
								y2++;									
							}									
						}
					}	
			}
		}
		
		rc = pthread_mutex_unlock(&mutexzone);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(0);
		}
		
		rc = pthread_mutex_lock(&mutextel);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(0);
		}
		
		available_phones++;
		
		if (found==0) {seatFails++;}
		
		rc = pthread_cond_signal(&condtel);//signal
		if (rc != 0) {
			printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);
			pthread_exit(0);
		}
		
		rc = pthread_mutex_unlock(&mutextel);
		if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(0);
		}
		
		
		
		//b fash..
		if(found==0){
		
			
			rc = pthread_mutex_lock(&mutexprint);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(0);
			}
			
			printf("Gia pelath : %d ",tid);
			printf("h krathsh apetyxe giati den yparxoyn katallhles theseis.\n");
			
			rc = pthread_mutex_unlock(&mutexprint);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(0);
			}
			
			pthread_exit(NULL); //pethainei to nima
			
		}
		else{	
									//proxwrame sth plhrwmh..
			int cost;
			if (zone==1){					//ypologismos kostoys..
				cost=tickets*CzoneA;
				}
			else{cost=tickets*CzoneB;}
			
			
					
			//proxwrame ston tamia
			struct timespec before2;
			clock_gettime(CLOCK_REALTIME,&before2);
			
			
			rc = pthread_mutex_lock(&mutexcash);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(0);
			}
			
			while ( available_cash == 0){
				rc = pthread_cond_wait( &condcash,&mutexcash);
				if (rc != 0) {
					printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
					pthread_exit(0);
				}
				
			}
			available_cash--;
			rc = pthread_mutex_unlock(&mutexcash);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(0);
			}
			
			
			struct timespec after2;
			clock_gettime(CLOCK_REALTIME,&after2);
			int time_elapsed2=after2.tv_sec-before2.tv_sec;

			rc = pthread_mutex_lock(&mutextime);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(0);
			}
			
			totaltime=totaltime+time_elapsed2;
			
			rc = pthread_mutex_unlock(&mutextime);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(0);
			}
			
			//sucCust++; //pelates poy proxwrhsan..	
				
			sleep(rand_r(&seed2) % (tcashhigh +1 - tcashlow)+tcashlow);	//xronos poy o tamias prospathei na kanei thn plhrwmh
			
			int r2=rand_r(&seed2)%100;
			if(r2<=Pcardsucces){


				rc = pthread_mutex_lock(&mutexpayment);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
					pthread_exit(0);
				}
				
				BankAcount=BankAcount+cost;
				OKorders++;
				rc = pthread_mutex_unlock(&mutexpayment);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
					pthread_exit(0);
				}


				rc = pthread_mutex_lock(&mutexprint);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
					pthread_exit(0);
				}
				printf("Gia pelath %d :",tid);
				printf("h krathsh oloklhrwthke epityxws!Oi theseis sas einai sth zwnh %d ",zone);
				printf("seira %d ",seira);
				printf("arithmos ");
				
					
				for(int m=0;m<tickets;m++){
						printf("%d ",theseis[m]);
													
				}
				printf("kai to kostos synallaghs einai : %d eurw \n",cost);
				rc  =pthread_mutex_unlock(&mutexprint);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
					pthread_exit(0);
				}
					
			
				
			}
			else{


				rc = pthread_mutex_lock(&mutexzone);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
					pthread_exit(0);
				}
				for(int p=0;p<tickets;p++){
					seats[seira][theseis[p]]=0;
				}		//epistrofh thesewn	
				payFails++;
				rc = pthread_mutex_unlock(&mutexzone);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
					pthread_exit(0);
				}


				//payFails++;

				rc = pthread_mutex_lock(&mutexprint);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
					pthread_exit(0);
				}
				printf("Gia pelath %d :",tid);
				printf("h krathsh apetyxe giati h synallagh me thn pistwtikh karta den egine dekth. \n");
				rc = pthread_mutex_unlock(&mutexprint);
				if (rc != 0) {
					printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
					pthread_exit(0);
				}
								
			}

			rc = pthread_mutex_lock(&mutexcash);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(0);
			}
			available_cash++;
		
	
		
			rc = pthread_cond_signal(&condcash);//signal
			if (rc != 0) {
					printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);
					pthread_exit(0);
			}
			
			rc = pthread_mutex_unlock(&mutexcash);
			if (rc != 0) {
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(0);
			}

			
		}			
	struct timespec after4;
	clock_gettime(CLOCK_REALTIME,&after4);
	int time_elapsed4=after4.tv_sec-before.tv_sec;

	rc = pthread_mutex_lock(&mutextime);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(0);
	}
	
	totalphonetime=totalphonetime+time_elapsed4;
	
	rc = pthread_mutex_unlock(&mutextime);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(0);
	}
	
	pthread_exit(NULL);
}


int main(int argc, char** argv){
	
	Ncust = atoi(argv[1]);
	seed = atoi(argv[2]);
	
	int rc;

	rc = pthread_mutex_init(&mutexprint, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_init(&mutextel, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
			return -1;
	}
		
	rc = pthread_mutex_init(&mutexpayment, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_init(&mutexzone, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
			return -1;
	}
		
	rc = pthread_mutex_init(&mutexcash, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_init(&mutextime, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
			return -1;
	}



	rc = pthread_cond_init(&condtel, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_cond_init(&condcash, NULL);
	if (rc != 0) {
			printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
			return -1;
	}




	//pthread_t threads[Ncust];	
	pthread_t* threads=(pthread_t*)malloc((Ncust+1)*sizeof(pthread_t));						//pinakas nhmatwn
	int * tids = (int*)malloc((Ncust+1)*sizeof(int)); 
	
	for(int i=1;i<=Ncust;i++){	
	
		tids[i]=i;	 
		if(pthread_create(&threads[i],NULL, routine,&tids[i])!=0)		//create thread
		{
			perror("failed to create thread"); //debug
			return 1;
		}
		
		if (i!= Ncust){
		//debug printf("thread %d has started\n",i);
			sleep(rand_r(&seed) % (treshigh +1 - treslow)+treslow);
		}		//tyxaios xronos anamesa stis 																					klhseis(nea nhmata-pelates)
		
	}

	for(int i=1;i<=Ncust;i++){	 
		if(pthread_join( threads[i], NULL )!=0)						//wait for the threads to join..
		{
			return 2;
		}
		//debug printf("thread %d has finished execution\n",i);
	}
	printf("\n");
	for(int i=0;i<30;i++){
				for(int j=0;j<10;j++){
					if(seats[i][j]!=0){
						if(i<20){printf("ZONE B/SEIRA %d thesh %d pelaths %d \n",i,j,seats[i][j]);}
						else{printf("ZONE A/SEIRA %d thesh %d pelaths %d \n",i,j,seats[i][j]);}
					}
				}
			}
	
	printf(" \n");
	rc = pthread_mutex_destroy(&mutexprint);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_destroy(&mutextime);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_destroy(&mutextel);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_destroy(&mutexpayment);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_destroy(&mutexzone);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_mutex_destroy(&mutexcash);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
			return -1;
	}

	rc = pthread_cond_destroy(&condtel);
	if (rc != 0) {
			printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
			return -1;
	}
	
	rc = pthread_cond_destroy(&condcash);
	if (rc != 0) {
			printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
			return -1;
	}
	
	printf("Ta synolika esoda apo tis pwlhseis einai: %d eyrw\n",BankAcount);			//synolika esoda
	float pososto1=(float)(OKorders/Ncust);						//pososta 
	float pososto2=(float)(seatFails/Ncust);
	float pososto3=(float)(payFails/Ncust);

	printf("To pososto twn synallagwn pou oloklhrwnetai epitxws einai : %.2f %%\n",pososto1*100.0);
	printf("To pososto twn synallagwn pou apotygxanei logo eleipshs thesewn einai : %.2f %%\n",pososto2*100.0);
	printf("To pososto twn synallagwn pou apotygxanei sth diadikasia ths synallaghs me pistwtikh karta einai : %.2f %%\n",pososto3*100.0);	
	
	float mesosxronos1=((float)totaltime)/Ncust;
	printf("O mesos xronos anamonhs twn pelatwn einai : %f  sec \n",mesosxronos1);	
	
	float mesosxronos2= ((float)totalphonetime)/Ncust;
	printf("O mesos xronos exipiretisis einai : %f  sec \n",mesosxronos2);																		 
	
	free(threads);
	free(tids);
	pthread_exit(NULL);
	
	return 0;
	
}


