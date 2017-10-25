#include <iostream>
#include "Server.h"
#include "Client.h"
#include "Packager.h"
#include <process.h>
#include <vector>

//#include <sstream>
//#include <string>
//#include <vector>
//#include <omp.h>
//
//#define _WIN32_WINNT 0x501
//
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//
//#pragma comment(lib, "Ws2_32.lib")
//
//using std::cerr;
//
//int main() {
//	WSADATA wsaData;
//	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (result != 0) {
//		cerr << "WSAStartup failed: " << result << "\n";
//		return result;
//	}
//
//	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
//	if (listen_socket == INVALID_SOCKET) {
//		cerr << "Error at socket: " << WSAGetLastError() << "\n";
//		WSACleanup();
//		return 1;
//	}
//
//	struct sockaddr_in sa;
//	sa.sin_family = AF_INET;
//	sa.sin_port = htons(12345);
//	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//	result = bind(listen_socket, (struct sockaddr*)&sa, sizeof(sa));
//	if (result == SOCKET_ERROR) {
//		cerr << "bind failed with error: " << WSAGetLastError() << "\n";
//		closesocket(listen_socket);
//		WSACleanup();
//		return 1;
//	}
//
//	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
//		cerr << "listen failed with error: " << WSAGetLastError() << "\n";
//		closesocket(listen_socket);
//		WSACleanup();
//		return 1;
//	}
//	char buf[] = "mess";
//	std::vector<int> Clients;
//	std::vector<int> No_Conection;
//
//	while (true)
//	{
//		auto client_socket = accept(listen_socket, NULL, NULL);
//		if (client_socket == INVALID_SOCKET) {
//			cerr << "accept failed: " << WSAGetLastError() << "\n";
//			closesocket(listen_socket);
//			WSACleanup();
//			return 1;
//		}
//		Clients.push_back(client_socket);
//#pragma omp for
//		for (int i = 0; i < Clients.size(); i++) {
//			if (send(Clients[i], buf, 4, 0) == -1) {
//#pragma omp critical no_c
//				{
//					No_Conection.push_back(i);
//				}
//			}
//		}
//		if (No_Conection.size()>0) {
//			for (int i = No_Conection.size() - 1; i >= 0; i++)
//				Clients.erase(Clients.begin() + i);
//			No_Conection.clear();
//		}
//
//	}
//}
Server * server;
Client * client;
Client * client2;

void serverLoop2(void *)
{
	std::vector<std::thread*> trd;
	while (true)
	{
		//server->update();
		trd.push_back(server->updateMult<Packager>());
		//trd[0]->join();
		//trd[1]->join();
		
	}
}
void serverLoop(void *)
{
	Packager pk;
	while (true)
	{
		//if(server->reciveFromAll<Packager>(&pk))
			//std::cout<<pk.a<<" "<<pk.b<<std::endl;
	}
}
int main()
{
	server = new Server();
	_beginthread(serverLoop2, 0, (void*)12);
	_beginthread(serverLoop, 0, (void*)12);

	client = new Client();
	client2 = new Client();
	Packager B;
	B.a = 4;
	B.b = 'c';
	client->sendMessage<Packager>(client->connectSocket, &B);
	B.a = 3;
	B.b = 'd';
	client2->sendMessage<Packager>(client->connectSocket, &B);
	B.meta = "CLOSE";
	client2->sendMessage<Packager>(client->connectSocket, &B);
	while (true)
	{
		;
	}
}