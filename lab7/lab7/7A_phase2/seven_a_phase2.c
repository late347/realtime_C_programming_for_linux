#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>

//alarm handler
void Alarm_Handler(int sig_numb){
	volatile static int visited = 0;
	visited++;
	printf("Alarm_Handler was called %d. times\n", visited);
}




int main(int argc, char const *argv[])
{
	pid_t pid, wpid;
	int status = 0;
	int seconds = 5;



	/*make first fork*/
	pid = fork();

	if (pid == 0){
		/* childprocess*/

		//signal(SIGALRM, Alarm_Handler);
		alarm(5);

		execl("./ChildExecProgram.exe", 
			"ChildExecProgram", 
			(char*) 0);
		perror("exec");
		exit(0);
		
	}

	else if (pid > 0){
		/*parentprocess*/
		//pause();
		//printf("parentprocess unblocked by alarm\n");
				//wait for childproceses to terminate
		//print the file contents in buffered mode 7char per line
		while( (wpid = wait(&status)) > 0  ){
			printf("\n[terminated childPID was %ld\n]", (long) wpid );
		}
	}


	return 0;
}