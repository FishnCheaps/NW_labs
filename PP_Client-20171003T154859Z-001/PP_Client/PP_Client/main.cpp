#include <iostream>
#include <sstream>
#include <string>
#include <array>

#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>


#pragma comment(lib, "Ws2_32.lib")

using std::cerr;

int main(){
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cerr << "WSAStartup failed: " << result << "\n";
		return result;
	}

	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		cerr << "Error at socket: " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(12345);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	connect(client_socket, (sockaddr*)&sa, sizeof(sa));
	char Buffer[4];
	while (true){
	int k=recv(client_socket, Buffer, 4, 0);
	if (k!=-1)
	for (auto i = 0; i < 4; i++)
		std::cout << Buffer[i];
}
	//closesocket(client_socket);

}