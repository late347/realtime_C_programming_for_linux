#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include<time.h>


//print time in dd.mm.yyyy hh.mm.ss

 int main(int argc, char *argv[])
{
	time_t t0;

	struct tm *timeinfo;
	time(&t0); //this is a system call in Linux according to tutorialspoint.com
	// http://www.tutorialspoint.com/unix_system_calls/index.htm
	timeinfo = localtime(&t0);
	printf("%02d.%02d.%4d ---- %02d:%02d:%02d\n", timeinfo->tm_mday,
												 timeinfo->tm_mon+1, 
												 timeinfo->tm_year+1900,
												 timeinfo->tm_hour,
												 timeinfo->tm_min,
												 timeinfo->tm_sec );


	return 0;
}