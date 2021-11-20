#pragma once
#include <afx.h>

class IMySocketListener
{
public:
	virtual void OnAccepted(CString strIpAddress, int port) = 0;
	virtual void OnRecept(CString msg) = 0;
};

