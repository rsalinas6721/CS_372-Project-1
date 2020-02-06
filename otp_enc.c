#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <ctype.h>

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char message[100000];
	char key[100000];

//	if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); }

	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = 3000;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverHostInfo = gethostbyname("localhost");
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	int yes = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		error("CLIENT: ERROR connecting");

    FILE *inMess;
    char cMess;
    int countMess = 0;
		printf("TEST: 1");
    inMess = fopen(argv[1], "r");

    fseek(inMess, 0L, SEEK_END);
    long int size = ftell(inMess);
    fseek(inMess, 0L, SEEK_SET);
    if (size == 0){
	printf("File is Empty\n");
	exit(1);
    }
    cMess = fgetc(inMess);
    while (cMess != EOF){
        cMess = fgetc(inMess);
        countMess++;
    }
	fclose(inMess);


	char file_Len[11];
	sprintf(file_Len, "%d", countMess);


	char src[] = "*";
	while (strlen(file_Len) < 10){
		strncat(file_Len, src, 1);
	}

	send(socketFD, file_Len, strlen(file_Len), 0);
/*
	char srcTwo[] = "*";
	while (strlen(key_Len) < 10){
		strncat(key_Len, srcTwo, 1);
	}

	send(socketFD, key_Len, strlen(key_Len), 0);
*/
    inMess = fopen(argv[1], "r");
	while(fgets(message, sizeof(message)-1, inMess));
    fclose(inMess);
	int messLen = strlen(message);
	charsWritten = send(socketFD, message, messLen+1, MSG_WAITALL);
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(message)) printf("CLIENT: WARNING: Not all data written to socket!\n");

/*
    inKey = fopen(argv[2], "r");
	while(fgets(key, sizeof(key)-1, inKey));
	fclose(inKey);
	int keyLen = strlen(key);
	charsWritten = send(socketFD, key, keyLen+1, MSG_WAITALL);
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(key)) printf("CLIENT: WARNING: Not all data written to socket!\n");
*/
	close(socketFD);	// Socket is closed

	return 0;
}
