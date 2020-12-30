#include "stdafx.h"
#include "CLanGameLogicClient.h"

CLanGameLogicClient::CLanGameLogicClient(const char* ip, WORD port) :CLanClient(ip,port)
{
	Connect();
}

CLanGameLogicClient::~CLanGameLogicClient()
{
}

void CLanGameLogicClient::OnEnter()
{
	printf("[LAN_GAMELOGIC_CLIENT] LanGameLogicServer ����...\n");
	CPacket packet;
	WORD type = LAN_GAMELOGIC_REQ_SERVER_LOGIN;
	WORD serverType = GAME_SERVER;

	packet << type << serverType;
	SendPacket(&packet);
}

void CLanGameLogicClient::OnLeave()
{
}

void CLanGameLogicClient::OnRecv(CPacket* packet)
{
	WORD type;
	*packet >> type;
	switch (type)
	{
	default:
		printf("[Warning]���� ���� ���� ��Ŷ Ÿ��\n");
		break;
	}
}

void CLanGameLogicClient::OnSend()
{
}

void CLanGameLogicClient::OnError()
{
}
