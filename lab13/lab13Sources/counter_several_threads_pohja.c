// Five threads increment the common counter. They know the upper limit that is
// 5 000 000. The main thread waits until upper limit has been reached.
// There are three problems in this program. 
// 1. The main thread wastes resources because it waits in the busy loop.
// 2. Threads at least sometimes do extra steps
// 3. The main thread cannot detect the situation when counter becomes exactly 
//    MAXCOUNT.




/*SOLUTION

maybe we can fix the wrong increments if we have mutexprotected commoncounter
maybe we can use semaphore to send the semph, IF the counter is at the limit
mainthread then waits on the semph (blocked wait is not wasting resources)
use unnamed semph, so you can use it in same process, but from any possible subthread*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

#define N 5
#define MAXCOUNT 5000000

int counter=0;  // Common counter

pthread_mutex_t g_mutexLock = PTHREAD_MUTEX_INITIALIZER; //globalMutexLock
sem_t g_binSemph; //global binSemph sent from any subthread, mainthread waits and blocks on it



void *tf(void *param){
    int i;
    
    while ( counter < MAXCOUNT ) {
        /*protect commoncounter with mutex READ-Read-Write-Modify cycle*/
            pthread_mutex_lock(&g_mutexLock);//FIRST, try mutexLock
            if (counter < MAXCOUNT){ //check IF incrementable, or nonIncrementable
                counter++;
            }else{
                sem_post(&g_binSemph);
            }
            pthread_mutex_unlock(&g_mutexLock);//ENDING, unlock mutexLock
    }
    
             

    pthread_exit(NULL);
}

int main(void) {
    int i;
    pthread_t ti_arr[N];
    sem_init(&g_binSemph, 0, 0); //usedBetweenThreads, initVal==0

    
    for (i = 0 ; i < N ; i++) {
        if (pthread_create(ti_arr+i, NULL, tf, NULL) != 0) {
            printf("Error in creating a thread %d\n", i);
            exit (0);
        }
    }
    
    //while (counter < MAXCOUNT);
    sem_wait(&g_binSemph);
    sem_destroy(&g_binSemph);
	printf("Counter is after the while loop %d\n", counter);

    for ( i = 0 ; i < N ; i++) {
		pthread_join(ti_arr[i], NULL);
      }
	printf("Counter is when all sub threads have finnished %d\n", counter);
            
    return 0;
}

