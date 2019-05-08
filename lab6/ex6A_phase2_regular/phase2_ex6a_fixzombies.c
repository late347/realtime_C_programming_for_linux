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
	id_t pid1, pid2, wpid;
	int status=0;
	char BUF[21] = {0};


	printf("Enter input (n = next, other input terminates):");
	fgets(BUF, 21, stdin);

	while(BUF[0] == 'n') {

		pid1 = fork();
		if (pid1 == 0){
			/* first child is here == child1*/
			pid2 = fork();
			if(pid2 > 0){
				/*here is the child1, which is the master of the second fork
				masterOfSecondFork exits normally*/
				exit(0);
			}else if(pid2 == 0){
				/*here is the child2, which is the child of masterOfSecondFork
				he will do the jobs that are needed*/
				sleep(10); // This represents something real work
				exit(0);
				// that is done for the client	
				// it appears that this orphan child, 
				// will be given new parent, (init)

			}else if(pid2 < 0){
				perror("Fork:");
				exit(1);
			}
		}else if(pid1 < 0){
			perror("Fork:");
			exit(1);
		}

		wait(NULL);
		printf("Enter input (n = next, other input terminates):");
		fgets(BUF, 21, stdin);
	}

	
	return 0;
}