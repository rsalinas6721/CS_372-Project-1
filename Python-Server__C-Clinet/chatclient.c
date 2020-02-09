/*
Program Name: Client Portion of Client/Server Messenger Application
Programmer Name: Ricky Salinas
Course Name: OSU CS372
Last Modified: 9 FEB 2020
*/

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

/*
Pre Conditions: This program and main function requires that the server python script
is running and listening to a high port. This port will be used to establish the connection
and establish the clientserver relationship.

Post Conditions: After executing the program correctly, a client/server relationship will be established
the server python script until one decides to terminate the connection. This program will then and
but can be re-run to re-establish the connection.
*/

int main(int argc, char *argv[]){
	int BUFFSIZE = 500;					// Message Size is Established
	char mess[BUFFSIZE];				// Message Char Array is Initialized
	char response[BUFFSIZE];		// Response Char Array is Initialized
	int HANDLESIZE = 11;				// Chat Handle Size is Established
	char clientHandle[11];			// Chat Handle Char Array is Initialized

// Program calls the required socket structures required to set up network socket
	int sock, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

// PRogram verifies that the program contains the required arguments
	if (argc < 3) {
		fprintf(stderr,"Please Run %s Followed By The Server IP And Port Number\n", argv[0]); exit(1);
	}

// Socket is set up. A portion of the source code was used from an assignment prevously created in class Operating Systems-CS344
// The program will be submitted with assignment to prove the following source code was not plagiarized
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[2]);									// Port Number is taken from commandline
	serverAddress.sin_family = AF_INET;					// Socket uses IPv4 IP family
	serverAddress.sin_port = htons(portNumber);	// Socket sets up server IP address
	serverHostInfo = gethostbyname(argv[1]);		// Server IP is taken from commandline

// Program verifies that the host is accessible
	if (serverHostInfo == NULL) {
		fprintf(stderr, "No Such Host\n");
		exit(0);
	}

// Program continues to set up socket and verifies that the  socket was successfully opened.
// If an error is identified, the program exits
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		printf("%s \n", "An Error Occured");
		exit(1);
	}
	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

// Program connects to identified server and port
// If an error is identified, the program exits
	if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
		printf("%s \n", "An Error Occured");
		exit(1);
	}

// Chat Handle is requested from user
	printf("%s\n", "Enter Chat Handle: ");
	fgets(clientHandle, HANDLESIZE, stdin);
	strtok(clientHandle, "\n");

/*
Message loop is establihed and is continued until the server or client sends "\quit"
message. The client, first takes the message from the user. If the message is "\quit",
the client sends an exit message to the server and closes the connection. If the message
is a normal message, the client send its clientHandle followed by the message. Once this
is complete, the program waits for a response from the server and reviews it for "\quit"
before looping through the process again.
*/
	while (1){
		bzero(mess, BUFFSIZE);										// Variables are cleared
		bzero(response, BUFFSIZE);								// Variables are cleared
		printf("%s ", "Enter Message:>>");
		fgets(response, BUFFSIZE, stdin);
		strtok(response, "\n");
		if (strcmp(response, "\\quit") == 0){						// '\quit' is checked
			write(sock, clientHandle, HANDLESIZE);		// Chat handle is sent to server
			write(sock, response, BUFFSIZE);					// Quit message is sent to server
			printf("%s \n", "Terminating Connection");		// Shuts down connection if TRUE
			break;																				// Loop exits
		}

		if (strcmp(response, "\\quit") != 0){
			write(sock, clientHandle, HANDLESIZE);		// Chat handle is sent to server
			write(sock, response, BUFFSIZE);					// Message is sent to server
			printf("%s \n", "Waiting on response...");		// Program Output
		}

		read(sock, mess, BUFFSIZE);									// Client reads message from server
		strtok(mess, "\n");
		if (strcmp(mess, "\\quit") == 0){								// "\quit" message is checked. Program exits if TRUE.
			printf("%s \n", "Connection Terminated By Server");
			break;
		}
		printf("%s\n", mess);														// Message is displayed
	}
	close(sock);		// Connection is Closed
	return 0;
}
