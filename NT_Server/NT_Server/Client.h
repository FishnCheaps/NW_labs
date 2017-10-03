#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdio.h> 

#include "Packager.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "9898"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Client
{
public:
	Client();
	~Client();

	template <typename T>
	int sendMessage(SOCKET curSocket, T* message);

	template <typename T>
	int receiveMessage(SOCKET curSocket, T* buffer);
	SOCKET connectSocket;
private:
	int status;
};

Client::Client()
{
	WSADATA wsaData;
	connectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	status = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (status != 0) {
		printf("WSAStartup failed with error: %d\n", status);
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	status = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (status != 0)
	{
		printf("getaddrinfo failed with error: %d\n", status);
		WSACleanup();
		exit(1);
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (connectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		// Connect to server.
		status = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (status == SOCKET_ERROR)
		{
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}
	}

	freeaddrinfo(result);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1);
	}
}

Client::~Client()
{
}


template<typename T>
int Client::receiveMessage(SOCKET curSocket, T * buffer)
{
	int size = sizeof(T);
	if (size > 512)
	{
		printf("error: package is too big\n");
		exit(1);
	}
	char buff[512];
	int st = recv(curSocket, buff, size, 0);
	memcpy(buffer, buff, size);

	return st;
}


template<typename T>
 int Client::sendMessage(SOCKET curSocket, T * message)
{
	 int size = sizeof(T);
	 if (size > 512)
	 {
		 printf("error: package is too big\n");
		 exit(1);
	 }
	 char buff[512];
	 memcpy(buff, message, size);
	 return send(curSocket, buff, size, 0);
}