# Program Name: Server Portion of Client/Server Messanger Application
# Programmer Name: Ricky Salinas
# Course Name: OSU CS372
# Last Modified: 9 FEB 2020


import socket
import sys


# Pre Conditions: This program does not require any pre-conditions aide that the
# workstation running this program has some form of network connectivity. This program
# will be used by the a client to esstablish a client/server messanger relationship.

# Post Conditions: After executing the program correctly, the program will listen on
# a specific port until a connection is established. Once this occurs, a client/server
# relationship will be created until one host desides to terminate the connection. This program
# continue to listen for new connections until the progrma is terminated manualy with a SIGNAL.

but can be re-run to re-establish the connection.
*/
def main():
	portNumber = sys.argv[1]					# Port Number Is Retrieved From commandline
	host ='localhost'							# Host is set to localhost
	portNumber = int(portNumber)				# Port Number is converted to integer

# Socket module is used to set up and maintain network socket connections
# Connection is established with server using socket function
# Socket listens to 1 connection at a time
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.bind(('localhost', portNumber))
	sock.listen(1)
	print("Listening on Port", portNumber)


# Once the program is listening, it wait for a connection. After a connectio is established by a client,
# the client IP is displayed to the console and the client/server messanger relationship is established.

# The server first waits for user to send the first message. The message is sent following the client's chatHandle.
# After the message retrieved, it is checked for the termination option 'quit'. If TRUE, the connection with the client ends.
# If FALSE, the chatHandle and message are displayed. A response is then requested from the user. The response is checked for then
# termination option 'quit' and sent to the client. If the connection is closed with the current client, a seperate client can
# connect with the server.
	while True:
		connection, clientAddress = sock.accept()	# Connection is establihed with CLIENT
		print('Established Connection To Client: ', clientAddress)		# Connection is displayed to user
		while True:
			handle = b''												# Handle variable is  initialized
			while len(handle) < 11:										# Handle is retrieved
				handle += connection.recv(11)
			handle = handle.decode('utf-8')								# Handle is decoded and saved

			message = b''												# Message variable is initialized
			while len(message) < 500:									# Message is retrieved
				message += connection.recv(500)
			message = message.decode('utf-8')							# Message is decoded and saved

# Message is checked for "\quit" termination message
			if (message[0] == "\\" and message[1] == "q" and message[2] == "u" and message[3] == "i" and message[4] == "t"):
				print("Connection Terminated By Client")
				break

			print(handle + "> ", end="")								# Handle is displayed
			print(message)												# Message is displayed

			response = input("Enter Message:>> ")						# User is asked to enter a response
			if (response == "\\quit"):									# response is checked for "\quit" termination message
				connection.sendall(response.encode('utf-8'))			# "Quit" response is sent to client, connection is terminated
				print("Terminating Connection")
				break

			connection.sendall(response.encode('utf-8'))				# Response is sent to client and waits for response
			print("Waiting on response...")

main()
