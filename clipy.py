import socket
import sys


def main():
	portNumber = input("Enter Port Number: ")
	HANDLE = input("Enter Handle: ")
	serverAddress = input("Enter Server IP: ")
	portNumber = int(portNumber)
	establishedConnectionFD = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	establishedConnectionFD.connect((serverAddress, portNumber))
	while(True):
		message = input("Enter Message:>> ")
		if (message == "\\quit"):
			establishedConnectionFD.sendall(message.encode('utf-8'))
			print("Terminating Connection")
			break
		message = HANDLE + ":> " + message
		establishedConnectionFD.sendall(message.encode('utf-8'))
		print("Waiting on response...")
		bytes = b''
		while len(bytes) < 500:
			bytes += establishedConnectionFD.recv(500)
		bytes = bytes.decode('utf-8')
		if (bytes[0] == "\\" and bytes[1] == "q" and bytes[2] == "u" and bytes[3] == "i" and bytes[4] == "t"):
			print("Server Terminated Connection")
			exit(0)
		print(bytes)

main()
