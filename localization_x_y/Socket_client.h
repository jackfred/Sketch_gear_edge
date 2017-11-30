#include <WinSock2.h>
#include <string>
#include <iostream>
#include <WS2tcpip.h>
//For tcp/ip
#include <boost/asio.hpp>
#include <boost/array.hpp>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class Socket_client
{
private:


public:
	SOCKET sConnect;
	//宣告給 socket 使用的 sockadder_in 結構
	SOCKADDR_IN addr;
	string confirm;
	WSAData wsaData;
	WORD DLLVersion;
	int r;
	int s;
	//設定 socket

	int addlen = sizeof(addr);

	//Real mode 
	//Need to set IP of this PC to 192.168.100.1 and then set control PC to 192.168.100.2
	//const char* ip_address = "192.168.100.1";
	//Simulation mode
	const char* ip_address = "127.0.0.1";

	int Port = 4000;
	Socket_client();
	~Socket_client();

	void readMessage(SOCKET curSocket, char * buffer, int bufSize);
	void sendMessage(SOCKET curSocket, const char * message, int messageSize);
	void Message(SOCKET curSocket, char * buffer, char * out, const int bufSize, bool print = true);
	char* Message(char * command_in, bool print = false);
	void send_something(std::string message);
};

