
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include<time.h>
#include <errno.h>


void print_flags(int fileDesc){

	 
	int fileFlags = fcntl(fileDesc, F_GETFL);
	int accessMode = fileFlags & O_ACCMODE;

	 if ( accessMode == O_WRONLY ){
	 	printf("accessmode: w\n");
	 }else if ( accessMode == O_RDONLY){
	 	printf("accessmode: r\n");
	 }else{
	 	printf("accessmode: w+r\n");
	 }

	 if( fileFlags & O_NONBLOCK ){
	 	printf("O_NONBLOCK: True\n");
	 } else{
	 	printf("O_NONBLOCK: False\n");
	 }

	 if( fileFlags & O_APPEND){
	 	printf("O_APPEND: True\n");
	 }else{
	 	printf("O_APPEND: False\n");
	 }

}


 int main(int argc, char *argv[])
{
	//the file flag argument such as O_WRONLY is called access modes (Write only)
	//errno = 0 // reset errno

	int fd  = open("/dev/tty", O_RDONLY);
	print_flags(fd);

	char letter;

	ssize_t res = read(STDIN_FILENO, &letter , 1);

	if( res != -1 ){
		printf("read succeeded, char was %c\n", letter);
	}else{
		printf("read failed %s\n", strerror(errno));
	}


	const char letter0 = 'A';
	ssize_t resW = write(STDOUT_FILENO, &letter0, 1);
	if ( resW != -1 ){
		printf(" write succeeded\n");
	}else{
		printf("write failed %s\n", strerror(errno));
	}

	return 0;
}
