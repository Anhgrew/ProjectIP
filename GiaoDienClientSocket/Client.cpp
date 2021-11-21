#include "pch.h"
#include "Client.h"

int Client::recMessagePort()
{
	char acRetData[4096];
	int iStat = 0;
	iStat = recv(nSocket, acRetData, 4096, 0);
	if (iStat == -1)
		return 1;
	else {
		
	}
}
