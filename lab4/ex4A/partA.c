
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include<time.h>
#include <errno.h>


//print time in dd.mm.yyyy hh.mm.ss

int main(int argc, char *argv[])
{

	//outputstream

	char buf[5 + 1] = {0};
	int res = setvbuf(stdout, buf, _IOFBF, 5);
	//int res0 = setvbuf(stdin, NULL  _IONBF, 0);
	//if (res == 0 )
	//	printf("setbuf succeed\n");
	//else
	//	printf("setbuf error\n");

	int c = 'z';
	int j = 1;

	while ( j <= 13 ){
		c = getchar();
		if(c != -1) {
			putchar(c);
			sleep(1);
			j++;
		}
	}

	//exit(0);
	fflush(stdout);
	return 0;
}