#include "stdafx.h"
#include "Socket_client.h"


Socket_client::Socket_client()
{
	//開始 Winsock-DLL
	DLLVersion = MAKEWORD(2, 1);
	r = WSAStartup(DLLVersion, &wsaData);

	//AF_INET: internet-family
	//SOCKET_STREAM: connection-oriented socket
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	//設定 addr 資料
	addr.sin_addr.s_addr = inet_addr(ip_address);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);

	//Connecting socket or closing the application if there's no connection within 3 seconds.
	LARGE_INTEGER t1, t2, ts;
	QueryPerformanceFrequency(&ts);
	QueryPerformanceCounter(&t1);
	double wait_time;
	int res;
	while (1)
	{
		res = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
		QueryPerformanceCounter(&t2);
		wait_time = (t2.QuadPart - t1.QuadPart) / (double)(ts.QuadPart);
		if (wait_time > 3)
		{
			cout << "No connection!!" << endl << "Application will be closed in 3 seconds....";
			Sleep(3000);
			exit(0);
		}
		if (res == 0)
		{
			break;
		}
	}



}

void Socket_client::sendMessage(SOCKET curSocket, const char * message, int messageSize)
{
	send(curSocket, message, messageSize, 0);
	cout << "Send message:" << message << endl;
}

void Socket_client::readMessage(SOCKET curSocket, char * buffer, int bufSize)
{
	recv(curSocket, buffer, bufSize, 0);
	cout << "Read message:" << buffer << endl;
}

void Socket_client::Message(SOCKET curSocket, char * buffer, char * out, int bufSize, bool print)
{
	bufSize = strlen(buffer) + 1;
	int temp_size = strlen(buffer) + 1;
	while (bufSize > 0)
	{
		int i = send(curSocket, buffer, bufSize, 0);
		buffer += i;
		bufSize -= i;
	}
	char *ptr_send = buffer - temp_size;
	if (print){
		printf_s("Send : %s\n", ptr_send, 0);
	}
	char* ptr = out;
	int temp_size_read = 200;
	int i = 1;
	recv(curSocket, ptr, temp_size_read, 0);
}

char* Socket_client::Message(char *command_in, bool print)
{
	SOCKET curSocket = sConnect;
	const int bufSize = 200;
	char buffer[bufSize], out[bufSize];
	memset(buffer, 0, bufSize);
	memset(out, 0, bufSize);
	sprintf_s(buffer, sizeof(bufSize), "%s", command_in);

	int string_len = strlen(buffer) + 1;
	int temp_size = strlen(buffer) + 1;
	while (bufSize > 0)
	{
		int i = send(curSocket, buffer, string_len, 0);
		//buffer += i;
		string_len -= i;
	}
	char *ptr_send = buffer - temp_size;
	if (print){
		printf_s("Send : %s\n", ptr_send, 0);
	}
	char* ptr = out;
	int temp_size_read = 200;
	int i = 1;
	recv(curSocket, ptr, temp_size_read, 0);

	return ptr;
}


Socket_client::~Socket_client()
{
	//若之後不再使用，可用 closesocket 關閉連線
	closesocket(sConnect);
}


void Socket_client::send_something(std::string message)
{
	boost::asio::io_service ios;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip_address), Port);
	boost::asio::ip::tcp::socket socket(ios);
	socket.connect(endpoint);
	boost::array<char, 128> buf;
	std::copy(message.begin(), message.end(), buf.begin());
	boost::system::error_code error;
	socket.write_some(boost::asio::buffer(buf, message.size()), error);
	socket.close();
}
