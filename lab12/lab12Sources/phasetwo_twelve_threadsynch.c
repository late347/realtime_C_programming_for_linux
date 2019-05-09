#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>


uint64_t commonCounter = 0; //globalCounter is common to threads
const uint64_t incAmount = 1000000;

pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;


void * thread_func(void*argz);

void * thread_func(void*argz){
	//get args into local pointer
	uint64_t * subthreadCounter = (uint64_t*)argz;
	
	for (uint64_t i = 0; i < incAmount; ++i)
	{
		
		(*(subthreadCounter))++; //increment local
		
		pthread_mutex_lock(&mutexLock);//FIRST, try mutexLock
		commonCounter++;//increment global
		pthread_mutex_unlock(&mutexLock);//ENDING, unlock mutexLock

	}
	return NULL;

}






int main(int argc, char const *argv[])
{
/* tips & explanation

*mainthread
-waits until all subthreads are done working
-displays value of common counter, I think...

*each subthread
-increments the commoncounter 1 000 000 times
-there is counterArray which is in scope of main funciton,
-subthread incremets their own "counter" inside counerArray

*what is problem?
-probably the proble is read-modify-write cycle for threads
-increment is not normally atomic
-so context switching fucks it up
-mutex can be used to protect the counter before increments are done
*/
/*
pthread_t tid[2];
/// create all threads
for (int i = 0; i < 2; i++) {
    pthread_create(&tid[i], NULL, routine, NULL);
}
/// wait all threads by joining them
for (int i = 0; i < 2; i++) {
    pthread_join(tid[i], NULL);  
}
*/
	const int subThreads = 10;
	int sumOfLocals=0; //amount of localIncements in total, should be 10*subThreadCounter

	uint64_t counterArray[subThreads];
	for (int i = 0; i < subThreads; ++i)
	{
		counterArray[i]=0;
	}

	pthread_t id[subThreads];
	int status[subThreads];

	
	for (int i = 0; i < subThreads; ++i)
	{	/*create subthreads*/
		pthread_create( &(id[i]), NULL, thread_func, &(counterArray[i]) );
	}

	for (int j = 0; j < subThreads; ++j)
	{
		/* join threads */
		pthread_join(id[j], NULL);
	}
	/*display commonCounter
	list how many times each thread incremented the counters
	list how many times increment was missed*/
	printf("commonCounter was == %ld\n", commonCounter);
	printf("localCounters were as follows:\n");
	for (int i = 0; i < subThreads; ++i)
	{
		printf("%ld\n", counterArray[i]);
		sumOfLocals += counterArray[i];
	}
	printf("percentage accurateIncrement was %f\n", (float)commonCounter/sumOfLocals );



	return 0;
}