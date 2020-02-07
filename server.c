#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(1); }

int main(int argc, char *argv[])
{
	int BUFFSIZE = 500;
	char mess[500];
	char response[500];
	int nread = 0;
	int c;
	char clientHandle[11];

	int listenSocketFD, establishedConnectionFD, portNumber, charsWritten;
	socklen_t sizeOfClientInfo;
	int status;
	pid_t pid;
	struct sockaddr_in serverAddress, clientAddress;
	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); }
	memset((char *)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[1]);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		error("ERROR on binding");
	listen(listenSocketFD, 1);
	while(1){
		sizeOfClientInfo = sizeof(clientAddress);
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
		if (establishedConnectionFD < 0) error("ERROR on accept");
		bzero(mess, 500);
		memset(clientHandle, '\0', 11);
		memset(response, '\0', 500);
//		read(establishedConnectionFD, clientHandle, 11);
		while(1){
			read(establishedConnectionFD, mess, BUFFSIZE);
			printf("%s: %s\n", clientHandle, mess);
			memset(mess, '\0', 500);
			printf("%s ", ">>>");
			fgets(response, 500, stdin);
			strtok(response, "\n");
			printf("%s \n", response);
			if (strcmp(response, "quit") == 0){
				printf("%s \n", "MATCH!");
				break;
			}
			write(establishedConnectionFD, response, BUFFSIZE);
			memset(response, '\0', 500);
		}
		close(establishedConnectionFD);
	}
	close(listenSocketFD);
	return 0;
}
