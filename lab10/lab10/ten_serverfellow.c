/*lab10 serverFellow
regular exercise*/

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
#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	id_t pid1, pid2, wpid;
	int status=0;
	int keepRunning = 1;
	char BUF[21] = {0};
	int clientDetected=0;

	/*server serves the clients as fast as possible
	-server gets the serviceRequiest and sends the characters to the connection
	-server maybe closes connection after sending the characters???

	client
	-sends the serviceRequest
	-reads from server the characters
	-does whatever itt needs to do (gets end of file)
	
	-no zombies must be created
	-use Unix domain sockets
	-socket adress == server.sock

	*/


	int sockFD = socket(AF_UNIX, SOCK_STREAM, 0); //serverSocket
	if (sockFD == -1){
			perror("socket()error occured?!:");//client socket error, unlikely though...
			exit(1);
	}


	struct sockaddr_un saddru; //serverAddress
	saddru.sun_family = AF_UNIX;
	strcpy(saddru.sun_path, "server.sock"); //get the serverAddress
	int bindRes= bind(sockFD, (struct sockaddr*) &saddru, sizeof(saddru)); //bind the serverAddress
																// into the serverSocket
	if (bindRes == -1){
			perror("bind()error occured?!:");//client socket error, unlikely though...
			exit(1);
	}


	int listenToClient = listen(sockFD, SOMAXCONN);
	if (listenToClient == -1){
			perror("listenError occured?!:");//client socket error, unlikely though...
			exit(1);
	}
	/*use doublefork architecture to prevent the next season of 
	'the Walking Dead'
	*/
	while(1) {
		//wait for client (blocking probably..., which is ok for our purposes probably)
		int client_socket = accept(sockFD, NULL, 0);//accept the client's serviceRequest
		if (client_socket == -1){
			perror("client_socket Error occurred?!:");//client socket error, unlikely though...
			exit(1);
		}else{
			printf("connectionAccepted!\n");
			++clientDetected;
		}

		pid1 = fork();
		if (pid1 == 0){
			pid2 = fork();
			if(pid2 > 0){
				exit(0);
			}else if(pid2 == 0){
				/*REALWORK HAPPAENS HERE!!!  This is the orphanprocess
				DO the serviceRequest to the client*/
				int temp_client_socket = client_socket;
				int i = 0;
				char receivedBuf[4]={0};
				char curChar;
				ssize_t readRes;
				//read incoming packet (serviceRequest)
				readRes =  read(client_socket, receivedBuf, 4); 
				printf("reading from client... ");
				printf(" buf==%s\n",receivedBuf );
					

 				//trust,but,verify that it will have been digit[0,1,2...9]
				int sleepDelay = receivedBuf[2] - 48;
				printf("sleepDelay was %d\n",sleepDelay );
				if (sleepDelay >= 0){ //legal sleepDelay given, (correct packet format)
					for (int i = 0; i < 10; ++i){
						write(client_socket, receivedBuf, 1);
						sleep(sleepDelay);
					}
					//char c = '\0';
					//write(client_socket, &c, 1);
					printf("clientServedSuccessfully!\n");
					//close(client_socket);
					exit(0);
				}else{
					printf("illegalPacketFormat from Client?!?!?");
					//close(client_socket);
					exit(1);
				}
			
			}else{
				perror("Fork:"); //fork error, unlikely though...
				exit(1);
			}
		}else if(pid1 < 0){
			perror("Fork:");//fork error, unlikely though...
			exit(1);
		}

		printf("clientDetected %d\n",clientDetected );
		//close(client_socket);
		wait(NULL);
		close(client_socket);

	}

	close(sockFD);


	return 0;
}