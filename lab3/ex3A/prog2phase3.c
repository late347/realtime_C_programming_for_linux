
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include<time.h>


//print time in dd.mm.yyyy hh.mm.ss

 int main(int argc, char *argv[])
{
	//the file flag argument such as O_WRONLY is called access modes (Write only)
	int fd  = open("exlog.txt", O_WRONLY | O_CREAT | O_APPEND);

	for (int i = 0; i < 100000; ++i)
	{
		/* code */


		write(fd,  "BBBB", 4);
		
		
	}


	return 0;
}
