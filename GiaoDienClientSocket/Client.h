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

	//Client();
	//~Client();
	//void init(string ipAddress, int port);
	//int sendMessagePort(WCHAR* message, int len);
	int recMessagePort();

};

