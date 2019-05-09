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
#include <semaphore.h>


const int incAmount = 100000;

pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;
sem_t subThreadPrintSemph; //semaphores shuold synchronize threads between each other
sem_t mainThreadPrintSemph;


void * thread_func(void*args);

void * thread_func(void*args){
	//get args into local pointer
	//print theChar in loop
	char * theCharP = (char*)args;
	sem_post(&subThreadPrintSemph); //allow mainthread to start prints
	// inital semaphore send should allow the mainthread to start printouts...

	for (int i = 0; i < incAmount; ++i)
	{
		/*firstly, wait for mainthraed to have printed its own character
		secondly, print subthread character
		thirdly, send the subthread to allow mainthread to start new round
		*/
		sem_wait(&mainThreadPrintSemph); //wait until youve received semph, from mainthread
		printf("%c", *theCharP);
		sem_post(&subThreadPrintSemph);

	}
	return NULL;

}






int main(int argc, char const *argv[])
{


	pthread_t id;

	char letter2 = '2';
	char letter1 = '1';
    sem_init(&subThreadPrintSemph, 0, 0); //usedBetweenThreads, initVal==0
    sem_init(&mainThreadPrintSemph, 0, 0); //usedBetweenThreads, initVal==0




	/*create subthread*/
	pthread_create( &id, NULL, thread_func, &letter2 );
	

	/*mainthread does something useful*/
	for (int i = 0; i < incAmount; ++i){
		/*first wait for subthreadSemph
		secondly print the char, because mainthread starts 
		printing... (initialRound)
		thirdly send mainthreadSemph to allow subthread to print*/
		sem_wait(&subThreadPrintSemph);
		printf("%c", letter1);
		sem_post(&mainThreadPrintSemph);
	}

	/* join thread */
	pthread_join(id, NULL);
	sem_destroy(&mainThreadPrintSemph);
	sem_destroy(&subThreadPrintSemph);
	


	return 0;
}