#pragma once
#include "pch.h"
#include "ThreadFunc.h"

#include <windows.h>

Client client;

UINT  recMessageThread(LPVOID lParam)
{

	while (1)
	{
		if (client.recMessagePort() == -1) {
			Sleep(500);
		}
		else {
			
		}
			
	}
	return 0;
}