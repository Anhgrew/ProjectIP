#include "pch.h"
#include "CConnectedSocket.h"

CConnectedSocket::CConnectedSocket() { }

CConnectedSocket::~CConnectedSocket() { }

void CConnectedSocket::OnReceive(int nErrorCode)
{
	char buff[100];
	Receive(buff, sizeof(buff));
	if (mSocketListener != 0)
	{
		mSocketListener->OnRecept(CString(buff));
	}
}

void CConnectedSocket::SetSocketListener(IMySocketListener* listener)
{
	this->mSocketListener = listener;
}

