/*
Simple udp client
*/
#include<WinSock2.h>
#include<iostream>
#include<Ws2tcpip.h>
#ifdef linux
#include  <arpa/inet.h>
#endif

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8051   //The port on which to listen for incoming data

using namespace std;

int main() {

	// init
	WSAData wsaData; // The WinSocket object
	WORD DllVersion; // DDL version  for WSAData
	SOCKADDR_IN destination_addr; // Address to be binded to our connection socket
	int sizeOfAddr; // Size of server address
	int server_length;
	SOCKET thisSocket;  // Server's socket
	char* buffer; // Received message
	int bufferlength; // Received message length

	// Setup WinSocket
	DllVersion = MAKEWORD(2, 1);
	// catch any errors
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winstock startup failed", "Error", MB_OK | MB_ICONERROR);
		system("pause");
		WSACleanup();
		exit(10);
	}

	thisSocket = socket(AF_INET, SOCK_DGRAM, NULL); // Set up the socket connection
	if (thisSocket == INVALID_SOCKET) {
		printf("Could not create socket");
		WSACleanup();
		exit(0);
	}

	// Setup Socket address configuration
	destination_addr.sin_family = AF_INET; // It is an IPv4 Socket. You could also use AF_INET6 for IPv6 Sockets
	inet_pton(AF_INET, SERVER, &(destination_addr.sin_addr)); // Use server address - using localhost only
	destination_addr.sin_port = htons(PORT); // The port

	// Bind the server's socket to make sure that it is available
	if (bind(thisSocket, (SOCKADDR*)&destination_addr, sizeof(destination_addr)) == SOCKET_ERROR) {
		cerr << "ServerSocket: Failed to connect\n";
		system("pause");
		WSACleanup();
		exit(14);
	}

	while (true) {
		server_length = sizeof(destination_addr);

		// Receive the buffer length value
		if (recvfrom(thisSocket, (char*)&bufferlength, sizeof(int), NULL, (SOCKADDR*)&destination_addr, &server_length) < 0) {
			cout << "Failed to receive message" << endl;
		}
		else {
			buffer = new char[bufferlength];
			recvfrom(thisSocket, buffer, bufferlength, NULL, (SOCKADDR*)&destination_addr, &server_length);
			cout << "Message received: " << buffer << endl;
		}
	}

	system("pause");

	return 0;
}