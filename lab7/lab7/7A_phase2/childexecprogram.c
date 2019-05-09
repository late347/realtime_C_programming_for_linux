
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>


void Exec_Child_Alarm_Handler(int sig_numb){
	printf("signal received!!!\n");
}

int main(int argc, char const *argv[])
{
	

    signal(SIGALRM, Exec_Child_Alarm_Handler);
    pause();
	printf("ChildExecProgram.exe unblocked by alarm\n");


	return 0;
}