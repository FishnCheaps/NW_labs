#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <map>
#include <thread>

#include "Packager.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "9898"
#define DEFAULT_BUFLEN 512



template <typename T, typename F>
void reciveMult(SOCKET sock, F* server)
{
	T buf;
	buf.meta = "";
	int res;
	while (buf.meta!="CLOSE") {
		res = 0;
		res = server->receiveMessage<T>(sock, &buf);
		if (res != 0)
		{
			std::cout << "recived: " << buf.toString() << std::endl;
		}
	}
	std::cout << "Closed " << std::endl;
}

class Server
{
public:
	Server();
	~Server();

	template <typename T>
	int sendMessage(SOCKET curSocket, T* message);

	template <typename T>
	int receiveMessage(SOCKET curSocket, T* buffer);
	bool acceptNewClient(int& id);

	void update();
	template<typename T>
	std::thread* updateMult();


	template <typename T>
	bool reciveFromAll(T* buf);
private:

	SOCKET listenSocket;
	SOCKET clientSocket;
	std::map<int, SOCKET> sessions;

	int status;
	int clientId = 0;
};

Server::Server()
{
	WSADATA wsaData;

	listenSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	status= WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0) {
		printf("WSAStartup failed with error: %d\n", status);
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	status= getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (status != 0) {
		printf("getaddrinfo failed with error: %d\n", status);
		WSACleanup();
		exit(1);
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	status = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (status == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		exit(1);
	}
	freeaddrinfo(result);

	status = listen(listenSocket, SOMAXCONN);

	if (status == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		exit(1);
	}
}

Server::~Server()
{
}

template <typename T>
int Server::receiveMessage(SOCKET curSocket, T* buffer)
{
	int size = sizeof(T);
	if (size > 512)
	{
		printf("error: package is too big\n");
		exit(1);
	}
	char buff[512];
	int st=recv(curSocket, buff, size, 0);
	memcpy(buffer, buff, size);

	return st;
}

template <typename T>
int Server::sendMessage(SOCKET curSocket, T* message)
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

bool Server::acceptNewClient(int& id)
{
	clientSocket = accept(listenSocket, NULL, NULL);

	if (clientSocket != INVALID_SOCKET)
	{
		char value = 1;
		setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

		sessions.insert(std::pair<int, SOCKET>(id, clientSocket));

		return true;
	}

	return false;
}


void Server::update()
{
	if (acceptNewClient(clientId))
	{
		printf("Client %d connected to the server\n", clientId);

		clientId++;
	}
}
template <typename T>
std::thread* Server::updateMult()
{
	if (acceptNewClient(clientId))
	{
		printf("Client %d connected to the server\n", clientId);
		SOCKET sock = sessions.at(clientId);
		clientId++;
		std::thread thr(reciveMult<T,Server>,sock,this);
		thr.detach();
		return &thr;
	}
}

template <typename T>
	bool Server::reciveFromAll(T* buf)
	{

		std::map<int, SOCKET>::iterator iter;
		int res = 0;
		for (iter = sessions.begin(); iter != sessions.end(); iter++)
		{
			res = receiveMessage<T>(iter->second, buf);
			if (res != 0)
			{
				std::cout << "recived" << std::endl;
				return true;
			}
		}
		return false;
	}