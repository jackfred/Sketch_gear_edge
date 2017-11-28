#include <WinSock2.h>
#include <string>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class Socket_client
{
private:


public:
	SOCKET sConnect;
	//�ŧi�� socket �ϥΪ� sockadder_in ���c
	SOCKADDR_IN addr;
	string confirm;
	WSAData wsaData;
	WORD DLLVersion;
	int r;
	int s;
	//�]�w socket

	int addlen = sizeof(addr);
	const char* ip_address = "192.168.100.1";
	int Port = 4000;
	Socket_client();
	~Socket_client();

	void readMessage(SOCKET curSocket, char * buffer, int bufSize);
	void sendMessage(SOCKET curSocket, const char * message, int messageSize);
	void Message(SOCKET curSocket, char * buffer, char * out, const int bufSize, bool print = true);
};

