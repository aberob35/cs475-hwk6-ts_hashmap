
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "ts_hashmap.h"

ts_hashmap_t *data;
pthread_spinlock_t *lock;
int pshared;
int val;
int main(int argc, char *argv[]) {
//allocation
lock = (pthread_spinlock_t*)malloc(sizeof(pthread_spinlock_t));

//Void function for threads had issues making globals so i
// put the function here
srand(time(NULL));
void *hashTest() {
//condtion for if it should be a put or get or delete
  int Cond = (int)(rand() * 3.0 / RAND_MAX) + 1;

//Chooses and runs function
  //printf("The choosen function is: %d \n", Cond);
  if(Cond == 1){
  pthread_spin_unlock(lock);
  pthread_spin_lock(lock);
  int Pvalue = (int)(rand() * 98.0 / RAND_MAX) + 1;
  put(data,Pvalue,Pvalue);
  pthread_spin_unlock(lock);
  }else if(Cond == 2){
   pthread_spin_unlock(lock);
   pthread_spin_lock(lock);
   int Gvalue = (int)(rand() * 98.0 / RAND_MAX) + 1;
   get(data,Gvalue);
  pthread_spin_unlock(lock);
  }else if(Cond == 3){
   pthread_spin_unlock(lock);
   pthread_spin_lock(lock);
   int Dvalue = (int)(rand() * 98.0 / RAND_MAX) + 1;
      del(data, Dvalue);
   pthread_spin_unlock(lock);
  } else{
   printf("YOU GOT AN ERROR BIG FELLA \n");
  }
	return NULL;
}

// Getting necessary variables
	int num_threads = atoi(argv[1]);
	int cap = (unsigned int) atoi(argv[2]);
	srand(time(NULL));
	data = (ts_hashmap_t*) malloc(sizeof(struct ts_hashmap_t) * cap);  
	if (argc < 3) {
		printf("Usage: %s <num threads> <hashmap capacity>\n", argv[0]);
		return 1;
	} else {
		data = initmap(cap);

	//Threading tings ya know
		//allocating space to hold threads
		pthread_t *threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
		for(int i = 0; i < num_threads; i++ ){
			pthread_create(&threads[i],NULL, hashTest,NULL);
		}

		//Join Phase
		for(int i = 0; i < num_threads; i++){
			pthread_join(threads[i],NULL);
		}
		printmap(data);
	}
	pthread_spin_destroy(lock);
	return 0;
}

