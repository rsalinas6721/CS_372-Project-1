import socket
import sys

def error():
	print("An Error Occured!")
	exit(0)


def main():
	HOST = 'localhost'
	PORT = 2001
	addr = ('localhost', 2002)
	message = 'HELLO'
	print("TEST")
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print("Connecting")
	s.connect(addr)
	print("Connected")
	s.sendall(message.encode('utf-8'))
	print("SENT")
main()

'''
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char mess[500];
//	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = 2000;
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

	while (1){
		printf(">> ");
		fgets(mess, 500, stdin);
		printf("%s", mess);
		charsWritten = send(socketFD, mess, 500, MSG_WAITALL);
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
		if (charsWritten < strlen(mess)) printf("CLIENT: WARNING: Not all data written to socket!\n");

	}
	close(socketFD);
'''
