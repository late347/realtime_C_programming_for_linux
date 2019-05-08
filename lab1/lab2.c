#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char  *argv[]){

	if(argc != 2) // 
		write(STDOUT_FILENO, "error with argc", 15);

	else{
		while(1){
			write(STDOUT_FILENO,  argv[1], strlen(argv[1]));
			//printf("argv was %s\n", argv[1]);
		}
	}
	//close(STDOUT_FILENO);
	return 0;
}
