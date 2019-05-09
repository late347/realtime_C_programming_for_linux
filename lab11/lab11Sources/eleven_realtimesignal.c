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
	struct timespec createTime= {0,0};
	struct timespec diffTime = {0,0};
	struct timespec real_time_res = {0,0};

void sigaction_handler(int signo, siginfo_t *info, void *);

void sigaction_handler(int signo, siginfo_t *info, void *dummy) {
		clock_gettime(CLOCK_REALTIME, &curTime); //get the currentTime, when signal had been received!
											// note that the curTime is global volatile struct, volatile was recommended in 
											/*UNIX Systems Programming: Communication, Concurrency, and Threads
											Tekijät Kay A. Robbins, Steven Robbins*/
	sigReceivedCount++; //for debug only!

	printf("Signal is caught. Value passed to handler was %d, sigReceivedCount == %d \n", 
		info->si_value.sival_int, 
		sigReceivedCount);

	printf("curTimeSpec (handler) == ");
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
		/* childprocess
		only READ from pipe*/
		close(fdArray[1]);
		//theChildPid=getpid();
		printf("startedWaiting_sigsuspend in childprocess\n");
		/*block at the signalwait*/
		sigsuspend(&oldmask);
		printf("endedWaiting_sigsuspend in childprocess\n");

		char receivedBuf[50] = {0};
		char curBuf[50]={0};
		/*read createTime from pipe, and print the diffTime*/
		while(read(fdArray[0], &receivedBuf, 50)>0){
			printf("readingFromPipeInChild...\n");
		}
		printf("finishedReadingPipe\n");
		snprintf(curBuf, 50, "%lld.%.9ld", (long long)curTime.tv_sec, curTime.tv_nsec);
		printf("received createTime buf (after pipe, in child) %s\n", receivedBuf);
		/*already we know the curTime from signal handler
		so calculate and print the diffTime*/
		


		double cur_s = atof(curBuf);
		double create_s = atof(receivedBuf);
		double resultSeconds = cur_s - create_s;
		double microsec = resultSeconds *= 1000000.0;
		printf("childprocess got delay microseconds %f\n", microsec );
		close(fdArray[0]);
		exit(0);
	
	}else if(pid > 0){
		/*parentprocess
		only WRITE to pipe*/
		close(fdArray[0]);
		int i=0;
		char buf[50]={0}; //should contain two secondDigit + decimal+ nine digits +'\0' 
		
		/*get createTime
		generateSignal to sigqueue
		send the createTime to pipe*/
		int queueRes;
		while ( i < 1) {
			sleep(1);
			value.sival_int = i;
			clock_gettime(CLOCK_REALTIME, &createTime);
			printf("createTimeSpec (parent)== ");
			timespecdisplay(&createTime);
			queueRes = sigqueue(pid, SIGRTMAX, value);
			if(queueRes == -1){
				perror("sigQueueError:");
				exit(1);
			}
			printf("Signal %d sent from parentprocess\n", SIGRTMAX);

			/*format timespec into char array and send it to pipe*/
			snprintf(buf, 50, "%lld.%.9ld", (long long)createTime.tv_sec, createTime.tv_nsec);
			printf("original createTime buf (before pipe, in parent) %s\n", buf);
			/*put it to pipe*/
			write(fdArray[1], buf, strlen(buf));
			//memset(buf, 0, 13); //reset buf just to be sure, maybe its not necessary though...

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