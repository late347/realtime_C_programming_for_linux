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

	pid_t pid, wpid;
	int status = 0;

    /*NOTE! you can create file with all permissions with comma parameter for the O_CREAT*/
	int fd  = open("ChatLogFive.txt", O_RDWR | O_CREAT, 0777 | O_APPEND);
	char BUF[8] = {0};

	/*make first fork*/
	pid = fork();

	if ( pid == 0 ){
		/* childprocess*/
		int fellow_desc = OpenChatFellow(); //get chatfellowdescriptor
		int resultRead = 99;
		char received;	//get receivedChar

		while ( (resultRead  = read(fellow_desc, &received, 1)) > 0 ){
			write(STDOUT_FILENO, &received, 1); //print and write to file
			write(fd, &received, 1);
		}

		close(fellow_desc);
		if ( resultRead == -1 ){
			printf("error in reading from chatfellow\n");
			exit(1);
		}
		exit(0);

	} else if ( pid > 0 ){
		/*parent process*/
		char keypress = 'a';
		int keyboardPoll = 99;
		for (; keyboardPoll > 0;){
			keyboardPoll = read(STDIN_FILENO, &keypress, 1);
			if ( keyboardPoll > 0 ){
				write(STDOUT_FILENO, &keypress, 1); //print char
				if (keypress == 'q' || keypress == 'Q'){
					keyboardPoll = -100; //end loop with Q/q received
				} 
			}
		}

		if ( keyboardPoll == -1 ){
			printf("error in reading from keyboard\n");
		}

		//wait for childproceses to terminate
		//print the file contents in buffered mode 7char per line
		while ( (wpid = wait(&status)) > 0  ){
			printf("\n[terminated childPID was %ld\n]", (long) wpid );
		}

		printf("printing FileContents now...\n");
		off_t seekRes = lseek(fd, 0L, SEEK_SET); //goto start of file

		if ( seekRes != -1 ){
			int readSuccess = 99;
			while ( (readSuccess = read(fd, &BUF, 7)) > 0 ){ //read the file until ending, when no more chars => returns 0
				printf("%s\n", BUF);     //print the buffer line by line
				memset(&BUF, 0, 8);		//clear buffer
			}
			if (readSuccess == -1){
				printf("error in reading from the file\n");
			}
		}else{
			printf("error in lseek() at the file\n");
		}

		close(fd);

	}


	return 0;
}