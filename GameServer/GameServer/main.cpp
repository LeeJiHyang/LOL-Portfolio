#include "stdafx.h"
#include <iostream>
#include "CGameServer.h"

CGameServer server;

int main()
{
	_wsetlocale(LC_ALL, L"korean");
	
	for (;;)
	{
		//if (GetAsyncKeyState(VK_ESCAPE))
		//{
		//	printf("���� ����\n");
		//	server.Shutdown();
		//	break;
		//}

		//server.Monitor();
		Sleep(10);
	}

	return 0;
}