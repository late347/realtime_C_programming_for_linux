
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

	//file descriptor
	int randFd= OpenRandomGenerator(); 
	int countBelow_20 = 0;
	FILE*stream = fdopen(randFd, "r");

	int val = 0;
	int guard = 10;

	while(guard != EOF){
		guard = fscanf(stream, "%d", &val);

		if (guard != EOF)
		{

			if (val < 20)
			{
				countBelow_20++;
			}
			printf("lotto was: %d\n", val );
		}
	}

	printf("numbers below 20 was: %d\n", countBelow_20);

	return 0;
}