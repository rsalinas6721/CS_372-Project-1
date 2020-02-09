import socket
import sys

def main():

# Port Number, Handle, And Server Ip Address Are Retrieved From User
	portNumber = input("Enter Port Number: ")
	HANDLE = input("Enter Handle: ")
	serverAddress = input("Enter Server IP: ")
# Port Number is converted to integer
	portNumber = int(portNumber)
# Connection is established with server using socket function
	establishedConnectionFD = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	establishedConnectionFD.connect((serverAddress, portNumber))
	while(True):
# Message Is Retrieved From User
		message = input("Enter Message:>> ")
# \quit is checked. Program exits if TRUE. If TRUE, \quit message is sent to server.
		if (message == "\\quit"):
			establishedConnectionFD.sendall(message.encode('utf-8'))
			print("Terminating Connection")
			break
# Handle is concatenated to messages
		message = HANDLE + ":> " + message
# Message is sent to Server
		establishedConnectionFD.sendall(message.encode('utf-8'))
		print("Waiting on response...")
		bytes = b''
# Response is taken from server and converted to  a string.
		while len(bytes) < 500:
			bytes += establishedConnectionFD.recv(500)
		bytes = bytes.decode('utf-8')
# Response checked for "\quit". Program exits in TRUE.
		if (bytes[0] == "\\" and bytes[1] == "q" and bytes[2] == "u" and bytes[3] == "i" and bytes[4] == "t"):
			print("Server Terminated Connection")
			exit(0)
		print(bytes)

main()
