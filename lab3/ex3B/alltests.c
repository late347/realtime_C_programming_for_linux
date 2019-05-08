
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


	//TEST1 start
	//errno = 0 // reset errno
	printf("test1begins...\n");
	int fd  = open("/dev/tty", O_RDWR);
	print_flags(fd);
	char letter;
	ssize_t res = read(fd, &letter , 1);

	if( res != -1 ){
		printf("read succeeded, char was %c\n", letter);
	}else{
		printf("read failed %s\n", strerror(errno));
	}

	const char letter0 = 'A';
	ssize_t resW = write(fd, &letter0, 1);
	if ( resW != -1 ){
		printf(" write succeeded\n");
	}else{
		printf("write failed %s\n", strerror(errno));
	}

	close(fd);

	//TEST2 start
	printf("test2begins...\n");
	fd  = open("/dev/tty", O_RDONLY);
	print_flags(fd);
	res = read(fd, &letter , 1);

	if( res != -1 ){
		printf("read succeeded, char was %c\n", letter);
	}else{
		printf("read failed %s\n", strerror(errno));
	}

	resW = write(fd, &letter0, 1);
	if ( resW != -1 ){
		printf(" write succeeded\n");
	}else{
		printf("write failed %s\n", strerror(errno));
	}


	//TEST3 sttart
	printf("test3begins...\n");
	int newFlags = fcntl(fd, F_GETFL);
	newFlags =  newFlags | O_NONBLOCK;
	newFlags =  newFlags | O_APPEND;
	fcntl(fd, F_SETFL, newFlags);
	print_flags(fd);
	res = read(fd, &letter , 1);

	if( res != -1 ){
		printf("read succeeded, char was %c\n", letter);
	}else{
		printf("read failed %s\n", strerror(errno));
	}

	resW = write(fd, &letter0, 1);
	if ( resW != -1 ){
		printf(" write succeeded\n");
	}else{
		printf("write failed %s\n", strerror(errno));
	}


	//TEST4
	printf("test4begins...\n");
	newFlags = fcntl(fd, F_GETFL);
	newFlags =   newFlags & ~O_NONBLOCK; //set bitOFF O_NONBLOCK
	fcntl(fd, F_SETFL, newFlags);
	print_flags(fd);
	res = read(fd, &letter , 1);

	if( res != -1 ){
		printf("read succeeded, char was %c\n", letter);
	}else{
		printf("read failed %s\n", strerror(errno));
	}
	resW = write(fd, &letter0, 1);
	if ( resW != -1 ){
		printf(" write succeeded\n");
	}else{
		printf("write failed %s\n", strerror(errno));
	}	








	close(fd);



	return 0;
}
