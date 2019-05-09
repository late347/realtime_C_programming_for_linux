/*lab9A pipes
regular exercise*/

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


int main(int argc, char const *argv[])
{
	/* 
	-create fork
	- childprocess sends digits 0,1,2,3... 9 in once sec interval into pipe
	- when all digits are sent, child closes the write end of piipe and exits

	parentprocess
	-reads the pipe and immediately display digit to screen
	-parent terminates when it has received all chars from pipe
	-parent recoqnize ending when write end has closed (eof)
	*/

	pid_t pid, wpid;
	int status = 0;
	int seconds = 1;
	int fdArray[2]; //make the pipe
	pipe(fdArray);
	pid = fork();


	if (pid == 0)
	{
		/* childprocess*/
		char sendData[] = "0123456789";
		close(fdArray[0]);
		char * curChar = sendData;
		while( (*curChar) != 0 )
		{
			write(fdArray[1], curChar, 1);
				//sleep 1 sec, ifNot last legal char
			sleep(1);
			curChar++;
		}
		close(fdArray[1]);
		exit(0);
	}

	else if (pid > 0)
	{
		/* parentprocess */
		close(fdArray[1]);
		char received;
		int resultRead;
		while( (resultRead = read(fdArray[0], &received, 1)) > 0 )
		{
			write(STDOUT_FILENO, &received, 1);
		}
		if (resultRead == -1)
		{
			printf("error in read from pipe\n");
		}
		while( (wpid = wait(&status)) > 0  )
		{
			printf("\n[terminated childPID was %ld\n]", (long) wpid );
		}
		close(fdArray[0]);
		exit(0);
	}

	else
	{
		printf("error in the forking happened...\n");
	}

	return 0;
}