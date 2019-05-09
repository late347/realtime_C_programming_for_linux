// Five threads increment the common counter. They know the upper limit that is
// 5 000 000. The main thread waits until upper limit has been reached.
// There are three problems in this program. 
// 1. The main thread wastes resources because it waits in the busy loop.
// 2. Threads at least sometimes do extra steps
// 3. The main thread cannot detect the situation when counter becomes exactly 
//    MAXCOUNT.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define N 5
#define MAXCOUNT 5000000

int counter;  // Common counter

void *tf(void *param){
    int i;
    while ( counter < MAXCOUNT) {
	        counter++;
	    }
    pthread_exit(NULL);
}

int main(void) {
    int i;
    pthread_t ti_arr[N];
    
    for (i = 0 ; i < N ; i++) {
        if (pthread_create(ti_arr+i, NULL, tf, NULL) != 0) {
            printf("Error in creating a thread %d\n", i);
            exit (0);
        }
    }
    
    while (counter < MAXCOUNT);
	printf("Counter is after the while loop %d\n", counter);

    for ( i = 0 ; i < N ; i++) {
		pthread_join(ti_arr[i], NULL);
      }
	printf("Counter is when all sub threads have finnished %d\n", counter);
            
    return 0;
}

