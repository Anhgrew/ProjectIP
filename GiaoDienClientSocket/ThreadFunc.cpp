#pragma once
#include "pch.h"
#include "ThreadFunc.h"

#include <windows.h>

Client client;

UINT  recMessageThread(LPVOID lParam)
{
	while (1)
	{
		if (client.recMessagePort())
			break;
	}
	return 0;
}