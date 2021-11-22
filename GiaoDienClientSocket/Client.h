#pragma once
#include <iostream>
#include <winsock2.h>
#pragma warning(disable:4996) 
using namespace std;
class Client
{
public:


	WSADATA w;
	int res = 0;
	int nSocket;
	sockaddr_in srv;
	int index;

	string receive_message;
	Client() {};
	Client(const Client& p1) { 
		w = p1.w;
		res = p1.res;
		nSocket = p1.nSocket;
		sockaddr_in srv = p1.srv;
		receive_message = p1.receive_message;
	}
	~Client() {};
	
	//void init(string ipAddress, int port);
	//int sendMessagePort(WCHAR* message, int len);
	int recMessagePort();

};

