#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
	int BUFFSIZE = 500;
	char mess[BUFFSIZE];
	char response[BUFFSIZE];
	char clientHandle[11];
	int listenSocketFD, establishedConnectionFD, portNumber, charsWritten;
	socklen_t sizeOfClientInfo;
	int status;
	pid_t pid;
	struct sockaddr_in serverAddress, clientAddress;
	if (argc < 2) {
		fprintf(stderr,"Please Run %s Followed By Port Number\n", argv[0]); exit(1);
	}
	memset((char *)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[1]);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocketFD < 0){
		printf("%s \n", "An Error Occured");
		exit(1);
	}
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
		printf("%s \n", "An Error Occured");
		exit(1);
	}
	listen(listenSocketFD, 1);
	while (1){
		listen(listenSocketFD, 1);
		sizeOfClientInfo = sizeof(clientAddress);
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
		if (establishedConnectionFD < 0){
			printf("%s \n", "An Error Occured");
			exit(1);
		}
		bzero(mess, BUFFSIZE);
		memset(clientHandle, '\0', 11);
		memset(response, '\0', BUFFSIZE);
		pid = fork();
		switch (pid){
			case 0:{
				while(1){
						read(establishedConnectionFD, mess, BUFFSIZE);
						strtok(mess, "\n");
						if (strcmp(mess, "\\quit") == 0){
							printf("%s \n", "Terminating Connection");
							break;
						}
						printf("%s\n", mess);
						memset(mess, '\0', BUFFSIZE);
						printf("%s ", "Enter Message:>>");
						fgets(response, BUFFSIZE, stdin);
						strtok(response, "\n");
						if (strcmp(response, "\\quit") == 0){
							write(establishedConnectionFD, response, BUFFSIZE);
							printf("%s \n", "Terminating Connection");
							break;
						}
						write(establishedConnectionFD, response, BUFFSIZE);
						memset(response, '\0', BUFFSIZE);
						printf("%s \n", "Waiting on response...");
				}
				exit(0);
			}
			default:{
				pid_t actialpid = waitpid(pid, &status, WNOHANG);
			}
		}
		close(establishedConnectionFD);
	}

	close(listenSocketFD);
	return 0;
}
