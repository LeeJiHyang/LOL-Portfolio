#include "stdafx.h"
#include <iostream>
#include <wchar.h>
#include "CGameServer.h"

INT uniqueRoomNum = 0;

CGameServer::CGameServer()
{
	InitializeGameServer();
}

CGameServer::~CGameServer()
{
	ReleaseGameServer();
}

void CGameServer::InitializeGameServer()
{
	InitializeSRWLock(&clientLocker);
	InitializeSRWLock(&roomLocker);
	InitializeSRWLock(&nickLocker);

	jobPool = new MemoryPool<CJob>(500, 500, FALSE);
	jobQueue = new CircularList<IJob*>(1000);

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, UpdateThread, this, 0, &ID[0]);

	//lanLoginClient = new CLanLoginClient("127.0.0.1", 17000);
	//lanGameLogicClient = new CLanGameLogicClient("127.0.0.1", 18000);
}

void CGameServer::ReleaseGameServer()
{
	releaseFlag = true;

	for (int i = 0; i < dfMAX_THREAD_GAMESERVER; i++)
	{
		CloseHandle(hThread[i]);
	}

	for (auto iter : roomMap)
	{
		delete iter.second;
	}
	roomMap.clear();
}

void CGameServer::OnRecv(SESSION_ID sessionID, CPacket* pPacket)
{

	CPacket* pack = PacketPool::Alloc();
	pack->Clear();
	pack->Enqueue(pPacket->GetBufferPtr(), pPacket->GetUseSize());
	PacketPool::AddRefCount(pack);

	IJob* pJob = jobPool->Alloc();
	pJob = new(pJob)CJobOnRecv{ sessionID, pack, this };
	jobQueue->Enqueue(pJob);
	PacketPool::Free(pack);
}

void CGameServer::OnSend(SESSION_ID sessionID)
{
}

void CGameServer::OnJoin(SESSION_ID sessionID, SOCKADDR_IN sockaddr)
{
	CreateClient(sessionID, sockaddr);
}

void CGameServer::OnLeave(SESSION_ID sessionID)
{
	DeleteClient(sessionID);
}

void CGameServer::OnError(SESSION_ID sessionID)
{
}

unsigned __stdcall CGameServer::UpdateThread(LPVOID arg)
{
	CGameServer* pGameServer = (CGameServer*)arg;
	pGameServer->Update();
	return 0;
}

void CGameServer::Update()
{
	printf("CGameServer.UpdateThread Start...\n");
	while (!releaseFlag)
	{
		int useSize = jobQueue->GetUsedSize();
		while (useSize > 0)
		{
			IJob* job;
			jobQueue->Dequeue(&job);
			job->Excute();
			job->Clear();
			jobPool->Free((CJob*)job);
			--useSize;
		}
	}
	printf("CGameServer.UpdateThread End...\n");
}



Client* CGameServer::CreateClient(SESSION_ID sessionID, SOCKADDR_IN sockaddr)
{
	Client* client = new Client;
	if (client == nullptr)
	{
		printf("Ŭ���̾�Ʈ ���� �� �޸� ���� ����!!\n");
		return nullptr;
	}
	client->sessionID = sessionID;
	client->sockaddr = sockaddr;

	AcquireSRWLockExclusive(&clientLocker);
	clientMap[sessionID] = client;
	printf("Ŭ���̾�Ʈ ���� [ID : %lld]\n", sessionID);
	ReleaseSRWLockExclusive(&clientLocker);
	return client;
}

void CGameServer::DeleteClient(SESSION_ID sessionID)
{
	Client* target = nullptr;

	AcquireSRWLockExclusive(&clientLocker);

	auto client = clientMap.find(sessionID);
	if (client == clientMap.end())
	{
		ReleaseSRWLockExclusive(&clientLocker);
		return;
	}

	// �������� �г��� ����
	auto nick = nickMap.find(client->second->nickname);
	if (nick != nickMap.end())
	{
		nickMap.erase(client->second->nickname);
	}
	
	delete client->second;
	clientMap.erase(client);
	printf("Ŭ���̾�Ʈ ���� [ID : %lld]\n", sessionID);

	ReleaseSRWLockExclusive(&clientLocker);


}

Client* CGameServer::FindClient(SESSION_ID sessionID)
{
	map<SESSION_ID, Client*>::iterator iter;
	Client* client = nullptr;
	AcquireSRWLockExclusive(&clientLocker);

	iter = clientMap.find(sessionID);
	if (iter != clientMap.end())
	{
		client = (*iter).second;
	}

	ReleaseSRWLockExclusive(&clientLocker);
	return client;
}

Client* CGameServer::FindClientByAccountNO(INT64 accountNo)
{
	map<SESSION_ID, Client*>::iterator iter;
	Client* client = nullptr;
	AcquireSRWLockExclusive(&clientLocker);
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		if (iter->second->accountNo == accountNo)
		{
			client = iter->second;
			break;
		}
	}
	ReleaseSRWLockExclusive(&clientLocker);
	return client;
}

void CGameServer::PacketProc(SESSION_ID sessionID, CPacket* pPacket)
{
	Client* pClient = FindClient(sessionID);

	if (pClient == nullptr)
	{
		printf("[Error] Client Not Found! /SessionID : %lld\n", sessionID);
		return;
	}

	WORD type;
	*pPacket >> type;
	switch (type)
	{
	case GAME_REQ_NICK:
		ReqNick(pClient, pPacket);
		break;
	case GAME_REQ_LOGIN:
		ReqLogin(pClient, pPacket);
		break;
	case GAME_REQ_JOIN_GAME:
		ReqJoinGame(pClient, pPacket);
		break;
	case GAME_REQ_SELECT_CHAMP:
		ReqSelectChamp(pClient, pPacket);
		break;
	case GAME_REQ_READY:
		ReqReady(pClient, pPacket);
		break;
	case GAME_REQ_MOVE_MY_PLAYER:
		ReqMovePlayer(pClient, pPacket);
		break;
	case GAME_REQ_FOLLOW_TARGET:
		ReqFollowTarget(pClient, pPacket);
		break;
	case GAME_REQ_ATTACK:
		ReqAttack(pClient, pPacket);
		break;
	default:
		printf("[Warning] ���ǵ��� ���� ��Ŷ Ÿ�� ����\n");
		break;
	}
}


void CGameServer::ResTest(Client* pClient)
{

}

void CGameServer::ReqNick(Client* pClient, CPacket* pPacket)
{
	WCHAR nick[20] = {};

	pPacket->Dequeue((char*)nick, sizeof(nick));
	pClient->nickname = nick;

	auto find = nickMap.find(nick);
	if (find != nickMap.end())
	{
		// �ߺ� �г���
		pClient->loginStatus = LoginResult::ID_ALREADY_LOGGED_IN;
		ResNick(pClient);
		return;
	}

	pClient->loginStatus = LoginResult::SUCCEED;
	wprintf(L"[INFO] �г��� �ߺ�üũ ���� :[ID : %lld] [�г��� : %s]\n", pClient->sessionID, pClient->nickname.c_str());
	ResNick(pClient);
}

void CGameServer::ResNick(Client* pClient)
{
	CPacket* pPacket = PacketPool::Alloc();
	*pPacket << (WORD)GAME_RES_NICK;
	pPacket->Enqueue((char*)pClient->nickname.c_str(), sizeof(pClient->nickname));
	*pPacket << pClient->loginStatus;

	SendUnicast(pClient->sessionID, pPacket);
}

void CGameServer::ReqLogin(Client* pClient, CPacket* pPacket)
{
	WCHAR nick[20] = {};

	pPacket->Dequeue((char*)nick, sizeof(nick));
	pClient->nickname = nick;

	AcquireSRWLockExclusive(&nickLocker);
	auto find = nickMap.find(nick);
	if (find != nickMap.end())
	{
		// �ߺ� �г���
		pClient->loginStatus = LoginResult::ID_ALREADY_LOGGED_IN;
		ResLogin(pClient);
		ReleaseSRWLockExclusive(&nickLocker);
		return;
	}
	
	nickMap[pClient->nickname.c_str()] = pClient->sessionID;
	ReleaseSRWLockExclusive(&nickLocker);

	pClient->loginStatus = LoginResult::SUCCEED;
	wprintf(L"[INFO] �α��� ���� :[ID : %lld] [�г��� : %s]\n", pClient->sessionID, pClient->nickname.c_str());
	
	ResLogin(pClient);
	// TODO : ���� ���������
}

void CGameServer::ResLogin(Client* pClient)
{
	CPacket* pPacket = PacketPool::Alloc();
	*pPacket << (WORD)GAME_RES_LOGIN;
	pPacket->Enqueue((char*)pClient->nickname.c_str(), sizeof(pClient->nickname));
	*pPacket << pClient->sessionID << pClient->loginStatus;
	
	SendUnicast(pClient->sessionID, pPacket);
}

void CGameServer::ReqJoinGame(Client* pClient, CPacket* pPacket)
{
	// ��⿭�� �߰�
	readyQ.push(pClient);
	wprintf(L"��⿭ �߰� Count : %d\n", readyQ.size());

	// ��⿭�� 10�� �̻��̸�
	while (readyQ.size() >= dfROOM_MAX_USER_COUNT)
	{
		// ���� ����
		AcquireSRWLockExclusive(&roomLocker);
		Room* room = new Room();
		long roomNum = uniqueRoomNum++;
		room->number = roomNum;
		roomMap[roomNum] = room;
		
		for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
		{
			// ��⿭���� ���� �Ѹ� �̾Ƽ�
			Client* user = readyQ.front();
			readyQ.pop();
			// �濡 ����
			room->users[i] = user;
			user->roomNum = roomNum;
			user->numInRoom = i;
		}

		// �濡 �����ߴٴ� ��Ŷ ����
		for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
		{
			Client* user = room->users[i];
			ResJoinGame(user, room);
		}

		ReleaseSRWLockExclusive(&roomLocker);
	}
}

void CGameServer::ResJoinGame(Client* pClient, Room* room)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_JOIN_GAME;
	for (UINT i = 0; i < dfROOM_MAX_USER_COUNT; i++)
	{
		Client* user = room->users[i];
		pack->Enqueue((char*)user->nickname.c_str(), 40);
		*pack << i;

		
	}

	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ReqSelectChamp(Client* pClient, CPacket* pPacket)
{
	// è�� ���� ��û
	BYTE champ;
	*pPacket >> champ;

	AcquireSRWLockExclusive(&roomLocker);
	auto find = roomMap.find(pClient->roomNum);
	if (find == roomMap.end())
	{
		// �濡 �ҼӵǾ� ���� ���� Ŭ���̾�Ʈ
		ReleaseSRWLockExclusive(&roomLocker);
		return;
	}

	Room* room = find->second;
	for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
	{
		ResSelectChamp(room->users[i], pClient->numInRoom, champ);
	}
	ReleaseSRWLockExclusive(&roomLocker);
}

void CGameServer::ResSelectChamp(Client* pClient, INT userNum, BYTE champ)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_SELECT_CHAMP << userNum << champ;
	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ReqReady(Client* pClient, CPacket* pPacket)
{
	AcquireSRWLockExclusive(&roomLocker);
	auto find = roomMap.find(pClient->roomNum);
	if (find == roomMap.end())
	{
		// �濡 �ҼӵǾ� ���� ���� Ŭ���̾�Ʈ
		ReleaseSRWLockExclusive(&roomLocker);
		return;
	}

	Room* room = find->second;
	room->ready[pClient->roomNum] = true;
	ReleaseSRWLockExclusive(&roomLocker);

	for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
	{
		if (room->ready[i] == false) return;
	}
	// ��� ���� ���¸� ���� ����
	for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
	{
		ResReady(room->users[i]);
	}
}

void CGameServer::ResReady(Client* pClient)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_READY;

	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ResCreatePlayer(Client* pClient)
{
	CPacket* pack = PacketPool::Alloc();
	//TODO : ���߿� ��񿡼� �÷��̾� ��ġ �޾ƿ;� ��
	*pack << (WORD)GAME_RES_CREATE_PLAYER << pClient->accountNo;
	pack->Enqueue((char*)pClient->nickname.c_str(), sizeof(pClient->nickname));
	*pack << pClient->x << pClient->y << pClient->z;

	SendBroadcast(pack);
}

void CGameServer::ResCreateOtherPlayer(Client* pClient)
{
	map<SESSION_ID, Client*>::iterator iter;
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		Client* other = iter->second;
		if (other == pClient) continue;
		CPacket* pack = PacketPool::Alloc();
		*pack << (WORD)GAME_RES_CREATE_PLAYER << other->accountNo;
		pack->Enqueue((char*)other->nickname.c_str(), sizeof(other->nickname));
		*pack << other->x << other->y << other->z;
		SendUnicast(pClient->sessionID, pack);
	}
}

void CGameServer::ResDeletePlayerToClients(INT64 accountNo)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_DELETE_PLAYER << accountNo;

	SendBroadcast(pack);
}

void CGameServer::ReqMovePlayer(Client* pClient, CPacket* pPacket)
{
	//printf("[�̵���û][AccountNo : %lld]\n", pClient->accountNo);
	// TODO : ���⼭ �����ӷ���Ŭ���̾�Ʈ�� �̵����� ���� Ȯ���ؾ���
	INT64 accountNo;
	float x, y, z;
	*pPacket >> accountNo >> x >> y >> z;

	if (pClient->accountNo != accountNo)
	{
		printf("[Warning] AccountNo ����ġ\n");
		Disconnect(pClient->sessionID);
		return;
	}
	pClient->x = x;
	pClient->y = y;
	pClient->z = z;

	ResMovePlayer(pClient);
}

void CGameServer::ResMovePlayer(Client* pClient)
{
	//printf("[�̵�����][AccountNo : %lld]\n", pClient->accountNo);
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_MOVE_PLAYER << pClient->accountNo << pClient->x << pClient->y << pClient->z;
	
	// TODO : ���� ��ε� ĳ��Ʈ�� �ٲ����
	SendBroadcast(pack);
}

void CGameServer::ReqFollowTarget(Client* pClient, CPacket* pPacket)
{
	INT64 accountNo;
	INT64 targetNo;
	*pPacket >> accountNo >> targetNo;

	if (pClient->accountNo != accountNo)
	{
		printf("[Warning]���󰡱� AccountNo ����ġ[S:%lld][C:%lld]\n",pClient->accountNo,accountNo);
		Disconnect(pClient->sessionID);
		return;
	}

	ResFollowTarget(accountNo, targetNo);
}

void CGameServer::ResFollowTarget(INT64 accountNo, INT64 targetNo)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_FOLLOW_TARGET << accountNo << targetNo;

	// TODO : ���� ��ε� ĳ��Ʈ�� �ٲ����
	SendBroadcast(pack);
}

void CGameServer::ReqAttack(Client* pClient, CPacket* pPacket)
{
	INT64 accountNo;
	INT64 targetNo;
	*pPacket >> accountNo >> targetNo;

	if (pClient->accountNo != accountNo)
	{
		printf("[Warning]���� AccountNo ����ġ[S:%lld][C:%lld]\n", pClient->accountNo, accountNo);
		Disconnect(pClient->sessionID);
		return;
	}

	Client* target = FindClientByAccountNO(targetNo);
	if (target == nullptr)
	{
		printf("[Warning] ���� ��� ���ݽõ�\n");
		return;
	}
	target->hp -= pClient->damage;
	if (target->hp < 0)
	{
		target->hp = 100;
	}

	ResDamage(targetNo, target->hp);
}

void CGameServer::ResDamage(INT64 targetNo, int hp)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_DAMAGE << targetNo << hp;

	// TODO : ���� ��ε� ĳ��Ʈ�� �ٲ����
	SendBroadcast(pack);
}




void CGameServer::Monitor()
{
	static DWORD curtime = 0;
	static DWORD oldtime = 0;
	static DWORD delay = 0;
	curtime = timeGetTime();
	if (oldtime == 0)
	{
		oldtime = curtime;
		return;
	}
	DWORD time = curtime - oldtime;
	delay += time;
	if (delay >= 1000)
	{
		delay = 0;
		printf(
			"============================\nThread Loop : %lld\nConnect Count : %lld\nConnect Total : %lld\nConnect Fail : %lld\n\nSession : %u\nClient : %lld\n\nSendPacketTPS : %lld\nRecvPacketTPS : %lld\n"
			, threadLoopCount, connectCount, connectTotal, connectFail, GetSessionSize(), clientMap.size(), sendTPS, recvTPS
		);
		threadLoopCount = 0;
		connectCount = 0;
		sendTPS = 0;
		recvTPS = 0;
	}
	oldtime = curtime;
}
