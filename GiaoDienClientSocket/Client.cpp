#include "pch.h"
#include "Client.h"

int Client::recMessagePort()
{
	char receive_mess[4096];
	memset(receive_mess, 0, sizeof receive_mess);
	int iStat = 0;
	iStat = recv(nSocket, receive_mess, 4096, 0);
	
	if (iStat < 0) {
		return -1;
	}
	else {
		this->receive_message = string(receive_mess);
		return 1;
	}
}
