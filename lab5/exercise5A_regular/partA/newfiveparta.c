#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
 #include <sys/wait.h>
#include <stdio.h>
#include <sys/resource.h>

int main(int argc, char const *argv[])
{

	setbuf(stdout, NULL); // set stdout to be un-buffered
    struct rlimit rl;
    getrlimit(RLIMIT_NPROC, &rl);
    printf("softlimit for resource: %ld\n", rl.rlim_cur);



	//print how many processes can be made
	//create 5 child processes
	//first child print A
	//then wait one second
	//repeat five times
	//parent process waits for all children
	//when any chlid had terminated then print "a child xxxx has terminated" show the pid
	//when all children are terminated, parent terminates
	pid_t cpid, wpid;
	int status = 0;

	/*make first fork, and get child1 and simply master1 continues forkings*/
	cpid = fork();
	if (cpid == 0){
		//child1
		for (int i = 0; i < 5; ++i){
			printf("A");
			sleep(1);
		}
		exit(0);
	}else if (cpid > 0){
		/*master continues forkings*/
		cpid = fork();
		if (cpid == 0){
			//child2
			for (int i = 0; i < 5; ++i){
				printf("B");
				sleep(1);
			}
			exit(0);		
		}else if (cpid > 0){
			/*master continues forkings*/
			cpid = fork();
			if (cpid == 0){
				//child3
				for (int i = 0; i < 5; ++i){
					printf("C");
					sleep(1);
				}
				exit(0);
			}else if (cpid > 0){
				/*master continues forkings*/
				cpid = fork();
				if (cpid == 0){
					//child4
					for (int i = 0; i < 5; ++i){
						printf("D");
						sleep(1);
					}
					exit(0);
				}else if(cpid > 0){
					/*master continues forkings*/
					cpid = fork();
					if (cpid == 0){
						//child 5
						for (int i = 0; i < 5; ++i){
							printf("E");
							sleep(1);
						}
					}
				}
			}
		}
	}












//blocking wait until all children have terminated!!!
	// NOTE! returnvalue from wait() will be inside the wpid variable
	// so you can get the child processID
	while( (wpid = wait(&status)) > 0  ){
		printf("\n[terminated childPID was %ld]", (long) wpid );
	}




	return 0;
}