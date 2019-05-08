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

	pid_t pid;
	char buf[20];
	printf("Enter input (n = next, other input terminates):");
	fgets(buf, 20, stdin);
	while(buf[0] == 'n') {
		pid = fork();
		if (pid < 0) {
			perror("Fork:");
			exit(1);
		}
		if (pid == 0) {
			sleep(5); // This represents something real work
			exit(0);
			// that is done for the client
		}
		printf("Enter input (n = next, other input terminates):");
		fgets(buf, 20, stdin);


	}
	
	return 0;

}