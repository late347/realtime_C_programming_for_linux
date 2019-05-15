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

// Functions for struct timespec and nanoseconds
long long int getnanosec(const struct timespec* ts);
struct timespec maketimespec(long long int ns);
struct timespec timespecdiff(const struct timespec* ts1, 
			const struct timespec* ts2);
void timespecdisplay(const struct timespec* ts);
int timespec_greater_than(const struct timespec* ts1, 
			const struct timespec* ts2);


volatile sig_atomic_t sigReceivedCount = 0; //for debug purposes?
	//struct timespec theTotalDelay= {0,0}; //temp variable to hold theDelay data from timespec from sensor
	//struct timespec deltaTime= {0,0};
	struct timespec curTime= {0,0};
	struct timespec recCreateTime = {0,0};
	struct timespec createTime= {0,0};
	struct timespec diffTime = {0,0};
	struct timespec real_time_res = {0,0};
	struct timespec sleepTime = {1,0};

void sigaction_handler(int signo, siginfo_t *info, void *);

void sigaction_handler(int signo, siginfo_t *info, void *dummy) {
	/*get the currentTime IMMEDIATELY when signal has been received!
	then, after handler has run with all functions, and after
	childprocess has read the createTime from pipe,
	then we can easily make debug printouts later on, (even inside 
	this handler I think...)*/
		clock_gettime(CLOCK_REALTIME, &curTime); 
											
	sigReceivedCount++; //for debug only!

	printf("Signal is caught. Value passed to handler was %d, sigReceivedCount == %d \n", 
		info->si_value.sival_int, 
		sigReceivedCount);

	printf("curTimeSpec (in handler) == ");
	timespecdisplay(&curTime);
	return;
}


int main(int argc, char const *argv[])
{

	/*parentprocess
	-gets current time => creationTime
	-generate signal , and use sigqueue
	-use SIGRTMAX
	-after signal generated, then send the creationTime into pipe

	for compilation use -lrt option
	
	childprocess
	-wait for signal sigsuspend
	-get current time value with getttime inside signal handler
	read from the pipe the sendingTime (when signal creation time was
	-calculate deliveryLag Time, and print it in microseconds
	*/

    clock_getres(CLOCK_REALTIME, &real_time_res);
    printf("Real time clock resolution is  :%ld s %ld ns\n", 
		real_time_res.tv_sec, real_time_res.tv_nsec );

	pid_t pid, wpid;
	pid_t theChildPid = 0;
	int status = 0;
	//int seconds = 1;

	time_t end_time;
	sigset_t mask;//sigset for the sigsuspend purposes...
	sigset_t oldmask; 
	union sigval value;

	/*setup the sigaction and the handler this should 
	be the only  thing thats strictly necessary, except
	that we also need sigset for sigsuspend, I think....*/

	/*code loaned from 
	UNIX Systems Programming: Communication, Concurrency, and Threads
	Robbins&Robbins
	program 9.10 page.324*/

	struct sigaction sigact;
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = sigaction_handler;


	if ( (sigemptyset(&sigact.sa_mask) == -1) || 
		(sigaction(SIGRTMAX, &sigact, NULL) == -1) ) {
		perror("failed to setupt signal somewhere...:");
	exit(1);
	}

	/*probably this sets-up the sigset for the purposes of sigsuspend
	TODO:: add error checking*/
	int res1, res2, res3;
	res1=sigemptyset(&mask);
	if(res1==-1){
		perror("sigemptysetError:");
		exit(1);
	}
	res2=sigaddset(&mask, SIGRTMAX);
	if (res2 == -1)
	{
		perror("sigaddsetError:");
		exit(1);
	}
	res3=sigprocmask(SIG_BLOCK, &mask, &oldmask);
	if (res3==-1)
	{
		perror("sigprocmaskError:");
		exit(1);
	}





	/*setup the pipe and the fork*/
	int fdArray[2]; 
	pipe(fdArray);
	pid = fork();

	if (pid == 0){
		/* childprocess, only READ from pipe*/
		close(fdArray[1]);
		printf("startedWaiting_sigsuspend in childprocess\n");

		/*block at the sigsuspend*/
		sigsuspend(&oldmask);
		printf("endedWaiting_sigsuspend in childprocess\n");

		/*read createTime from pipe, and print the diffTime*/
		read(fdArray[0], &recCreateTime, sizeof(recCreateTime));

		/*get the diffTime, which is the lag between signals*/
		diffTime = timespecdiff(&curTime, &recCreateTime);
		
		/*debug printouts*/
		printf("finishedReadingPipe\n");
		printf("received createTime (after pipe, in child): ");
		timespecdisplay(&recCreateTime);
		printf("diffTimeSpec=curTime-createTime ==> ");
		timespecdisplay(&diffTime);

		/*print the diffTime in microseconds*/
		printf("microseconds= %f us", (diffTime.tv_sec*1000000.0 + diffTime.tv_nsec*0.001) );

		close(fdArray[0]);
		exit(0);
	
	}else if(pid > 0){
		/*parentprocess, only WRITE to pipe*/
		close(fdArray[0]);
		int i=9999;
		/*get createTime
		generateSignal to sigqueue
		send the createTime to pipe*/
		int queueRes;

		while ( i <= 9999) {
			nanosleep(&sleepTime, NULL);		
			value.sival_int = i; //signal to send to the handler also...

			/*get createTime, and send immediately signal => 
			afterwards, we can debug printf, and also send to pipe
			because those  aren't in the signalLagDelay*/
			clock_gettime(CLOCK_REALTIME, &createTime);
			queueRes = sigqueue(pid, SIGRTMAX, value);

			/*debug printouts*/
			printf("createTimeSpec (in parent, before pipe send)== ");
			timespecdisplay(&createTime);
			
			if(queueRes == -1){
				perror("sigQueueError:");
				exit(1);
			}
			/*debug printouts, and pipe send*/
			printf("Signal.no %d sent from parentprocess\n", SIGRTMAX);
			write(fdArray[1], &createTime, sizeof(createTime)); //send createTime to struct
			i++;
		}
		close(fdArray[1]);
		printf("sentAllDataToPipe...\n");
		
		while( (wpid = wait(&status)) > 0  ){
			printf("\n[terminated childPID was %ld\n]", (long) wpid );
		}
		
		exit(0);

	}else{
		perror("ForkError... :");//fork error, unlikely though...
		exit(1);
	}

	return 0;
}



// Functions for struct timespec and nanoseconds
long long int getnanosec(const struct timespec* ts) {
    return ((long long int)ts->tv_sec)*1000000000 + ts->tv_nsec;
};

struct timespec maketimespec(long long int ns) {
    struct timespec aux;
    aux.tv_sec = ns/1000000000;
    aux.tv_nsec = ns%1000000000;
    return aux;
};

struct timespec timespecdiff(const struct timespec* ts1,
			const struct timespec* ts2) {
    long long int time1_nanos = getnanosec(ts1);
    long long int time2_nanos = getnanosec(ts2);
    long long int timediff_nanos = time1_nanos - time2_nanos;
    return maketimespec(timediff_nanos);
}
void timespecdisplay(const struct timespec* ts) {
    printf("%ld s %ld ns\n", ts->tv_sec, ts->tv_nsec);
}
int timespec_greater_than(const struct timespec* ts1, 
			const struct timespec* ts2) {
    long long int time1_nanos = getnanosec(ts1);
    long long int time2_nanos = getnanosec(ts2);
    return time1_nanos > time2_nanos;
}