#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char  *argv[]){

	int earlyStop=0; //stop if run out of alphabets or if user press q/Q

	//set inputGen into non blocking mode
	int fellow_desc;	//get filedescriptor
	fellow_desc = OpenChatFellow();
	//int file_flags= fcntl(fellow_desc, F_GETFL); //curr fileflags
	//file_flags = file_flags | O_NONBLOCK; //add nonblock bit
	//fcntl(fellow_desc, F_SETFL, file_flags);
	

	
	while(1){

		char received;	//get receivedChar
		int resultRead = read(fellow_desc, &received, 1);
		
		if (resultRead != -1)
		{
			if (resultRead == 0)
			{
				//write(STDOUT_FILENO, &received, 1);
				//earlyStop = 1;	
				//strange things happened to my program when 
				//the letters ran out from z onwards
				//so I decided it was better to exit gracefully rather than
				//keep printing Z's
			}
			else
			{
				write(STDOUT_FILENO, &received, 1);
			}
		}

	}
	close(fellow_desc);
	return 0;
}
