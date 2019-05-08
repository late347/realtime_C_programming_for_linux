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
	
	/*
	((parent process))
	-creates child process

	((childprocess))
	-that child process reads from chatfellow
	-output to screen
	-write to file also
	-terminate when EOF received from chatfellow???

	((parentprocess))
	-parent process reads user inputs from keyboard
	-when user writes Q or q, then parent process stops reading
	and then parent proces starts waiting on child
	-when child has terminated and waiting unblocks, then parentprocess prints
	file contents
	-print filecontents buffered as one line  == 7char


	-use syscalls in file handling
	-open the file in RW mode  in parentprocess before fork
	-open the chatfellow only in childprocess
	-
	*/

	pid_t pid, wpid;
    int status = 0;

    /*NOTE! you can create file with all permissions with comma parameter for the O_CREAT*/
    int fd  = open("ChatLogExtraFive.txt", O_RDWR | O_CREAT, 0777 | O_APPEND);
    char BUF[8] = {0};


/*make first fork*/
	pid = fork();

	if (pid == 0){
		/* childprocess*/
		int fellow_desc;	//get chatfellowdescriptor
		fellow_desc = OpenChatFellow();
		int lettercount = 0;

		while(1){

			char received;	//get receivedChar
			int resultRead = read(fellow_desc, &received, 1);
			
			if (resultRead != -1){
				if (resultRead == 0){ //doesnt read any char any longer after z character
					break;			  // then we can end childprocess
				}else{
					write(STDOUT_FILENO, &received, 1);
					write(fd, &received, 1);
					++lettercount;
				}
			}else{
				break;
			}

		}
		close(fellow_desc);
		exit(lettercount);

	} else if(pid > 0){
		/*parent process*/
		while(1){

			char keypress;
			int keyboardPoll = read(STDIN_FILENO, &keypress, 1);
			if (keyboardPoll != -1){
				/*NOTE! parentprocess only ends when user press key Q or q*/
				if (keypress == 'q' || keypress == 'Q'){
					write(STDOUT_FILENO, &keypress, 1);
					break;
				}else {
					write(STDOUT_FILENO, &keypress, 1);
				}
			}
		}

		//wait for childproceses to terminate
		//print the file contents in buffered mode 7char per line
		while( (wpid = wait(&status)) > 0  ){
			printf("\n[terminated childPID was %ld\n]", (long) wpid );
		}
		printf("printing lettercount now== %d", status/255);
	}


	return 0;
}
