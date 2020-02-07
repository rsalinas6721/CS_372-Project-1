import socket
import sys
import ctypes
import binascii

def error():
	print("An Error Occured!")
	exit(0)


def main():
	PORT = input("Enter Port Number: ")
	PORT = int(PORT)
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(('localhost', PORT))
	while(True):
		message = input(">>> ")
		if (message == "quit"):
			break
		sock.sendall(message.encode('utf-8'))
		bites = b''
		while len(bites) < 500:
			bites += sock.recv(500)
		bites = bites.decode('utf-8')
		print(bites, end = '')



main()
