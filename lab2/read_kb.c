#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char  *argv[]){

	int earlyStop=0; //stop if run out of alphabets or if user press q/Q

	//set inputGen into non blocking mode
	/*	int fellow_desc;	//get filedescriptor
	fellow_desc = OpenChatFellow();
	int file_flags= fcntl(fellow_desc, F_GETFL); //curr fileflags
	file_flags = file_flags | O_NONBLOCK; //add nonblock bit
	fcntl(fellow_desc, F_SETFL, file_flags);*/

	//set keyboard into non blocking mode
	//int keyflags = fcntl(STDIN_FILENO, F_GETFL);
	//keyflags = keyflags | O_NONBLOCK;
	//fcntl(STDIN_FILENO, F_SETFL, keyflags);

	while(1){

		char keypress;
		int keyboardPoll = read(STDIN_FILENO, &keypress, 1);
		if (keyboardPoll != -1)
		{
			if (keypress == 'q' || keypress == 'Q')
			{

				write(STDOUT_FILENO, &keypress, 1);
				break;
			}
			else 
			{
				write(STDOUT_FILENO, &keypress, 1);
			}
		}



	}
	return 0;
}