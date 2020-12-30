#include "stdafx.h"
#include "CLanLoginClient.h"

CLanLoginClient::CLanLoginClient(const char* ip, WORD port) :CLanClient(ip,port)
{
	InitializeSRWLock(&srwToken);

	Connect();
}

CLanLoginClient::~CLanLoginClient()
{
}

void CLanLoginClient::OnEnter()
{
	printf("[LAN_LOGIN_CLIENT] LanLoginServer ����...\n");
	CPacket packet;
	WORD type = LAN_LOGIN_REQ_SERVER_LOGIN;
	WORD serverType = GAME_SERVER;

	packet << type << serverType;
	SendPacket(&packet);
}

void CLanLoginClient::OnLeave()
{
}

void CLanLoginClient::OnRecv(CPacket* packet)
{
	WORD type;
	*packet >> type;
	switch (type)
	{
	case LAN_LOGIN_REQ_NEW_CLIENT:
		ReqCreateNewClient(packet);
		break;
	default:
		printf("[Warning]���� ���� ���� ��Ŷ Ÿ��\n");
		break;
	}
}

void CLanLoginClient::OnSend()
{
}

void CLanLoginClient::OnError()
{
}

void CLanLoginClient::ReqCreateNewClient(CPacket* pPacket)
{
	printf("[LAN] ���ο� Ŭ���̾�Ʈ ���� ��û ����\n");
	WORD type = LAN_LOGIN_RES_NEW_CLIENT;
	WORD serverType = GAME_SERVER;
	INT64 accountNo;
	CHAR* sessionKey = new CHAR[64];
	INT64 parameter;

	*pPacket >> accountNo;
	pPacket->Dequeue(sessionKey, 64);
	*pPacket >> parameter;

	AcquireSRWLockExclusive(&srwToken);
	tokenMap[accountNo] = sessionKey;
	ReleaseSRWLockExclusive(&srwToken);

	printf("[LAN] ���ο� Ŭ���̾�Ʈ ���� ���� ����\n");
	CPacket pack;
	pack << type << serverType << accountNo << parameter;
	SendPacket(&pack);
}

void CLanLoginClient::ReqAccountLogout(INT64 accountNo)
{
	printf("[LAN] �α��� ������ �α׾ƿ���û [%lld]\n", accountNo);
	WORD type = LAN_LOGIN_REQ_ACCOUNT_LOGOUT;
	CPacket pack;
	pack << type << accountNo;
	SendPacket(&pack);
}

bool CLanLoginClient::TokenCheck(INT64 accountNo, CHAR* sessionKey)
{
	bool result = true;
	AcquireSRWLockExclusive(&srwToken);
	
	auto token = tokenMap.find(accountNo);
	if (token == tokenMap.end())
	{
		printf("[Warning] Token Not Found\n");
		result = false;
	}
	else if (memcmp(token->second, sessionKey, 64) != 0)
	{
		printf("[Warning] Token is Different\n");
		result = false;
	}

	ReleaseSRWLockExclusive(&srwToken);
	return result;
}

void CLanLoginClient::DeleteToken(INT64 accountNo, CHAR* sessionKey)
{
	AcquireSRWLockExclusive(&srwToken);
	auto token = tokenMap.find(accountNo);
	if (token == tokenMap.end())
	{
		printf("[Warning] Token Not Found\n");
		ReleaseSRWLockExclusive(&srwToken);
		return;
	}

	if (memcmp(token->second, sessionKey, 64) != 0)
	{
		printf("[Warning] Token is Different\n");
		ReleaseSRWLockExclusive(&srwToken);
		return;
	}

	delete token->second;
	tokenMap.erase(token);
	ReleaseSRWLockExclusive(&srwToken);
}
