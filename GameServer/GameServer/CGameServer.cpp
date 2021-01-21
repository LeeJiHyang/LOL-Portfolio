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

void CGameServer::Shutdown()
{
	ReleaseGameServer();
	CNetwork::Shutdown();
}

void CGameServer::InitializeGameServer()
{
	InitializeSRWLock(&clientLocker);
	InitializeSRWLock(&roomLocker);
	InitializeSRWLock(&nickLocker);
	InitializeSRWLock(&gameroomLocker);
	InitializeSRWLock(&accountLocker);

	jobPool = new MemoryPool<CJob>(500, 500, FALSE);
	jobQueue = new CircularList<IJob*>(1000);

	hLogicThread[0] = (HANDLE)_beginthreadex(NULL, 0, UpdateThread, this, 0, &threadID[0]);

	//lanLoginClient = new CLanLoginClient("127.0.0.1", 17000);
	//lanGameLogicClient = new CLanGameLogicClient("127.0.0.1", 18000);
}

void CGameServer::ReleaseGameServer()
{
	printf("���Ӽ��� ����\n");
	releaseFlag = true;

	for (int i = 0; i < dfMAX_THREAD_GAMESERVER; i++)
	{
		if (hLogicThread[i] == INVALID_HANDLE_VALUE) continue;
		CloseHandle(hLogicThread[i]);
		hLogicThread[i] = INVALID_HANDLE_VALUE;
	}

	for (auto iter : roomMap)
	{
		delete iter.second;
	}
	roomMap.clear();
	accountMap.clear();
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
	case TEST_REQ:
		ReqTest(pClient, pPacket);
		break;
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
	case GAME_REQ_ENTER_GAME:
		ReqEnterGame(pClient, pPacket);
		break;
	case GAME_REQ_LOADING:
		ReqLoading(pClient, pPacket);
		break;
	case GAME_REQ_COMPLETE_LOADING:
		ReqCompleteLoading(pClient, pPacket);
		break;
	case GAME_REQ_TIME:
		ReqTime(pClient, pPacket);
		break;
	case GAME_REQ_MOVE:
		ReqMove(pClient, pPacket);
		break;
	default:
		printf("[Warning] ���ǵ��� ���� ��Ŷ Ÿ�� ����\n");
		break;
	}
}

void CGameServer::ReqTest(Client* pClient, CPacket* pPacket)
{
	wprintf(L"[INFO] �׽�Ʈ ��û [Nick : %s]\n");
	ResTest(pClient);
}

void CGameServer::ResTest(Client* pClient)
{
	wprintf(L"[INFO] �׽�Ʈ ���� [Nick : %s]\n");
	CPacket* pPacket = PacketPool::Alloc();
	*pPacket << (WORD)TEST_RES;

	SendUnicast(pClient->sessionID, pPacket);

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

	// �������
	AcquireSRWLockExclusive(&accountLocker);
	accountMap[pClient->nickname.c_str()].nickname = pClient->nickname.c_str();
	ReleaseSRWLockExclusive(&accountLocker);

	pClient->loginStatus = LoginResult::SUCCEED;
	wprintf(L"[INFO] �α��� ���� :[ID : %lld] [�г��� : %s]\n", pClient->sessionID, pClient->nickname.c_str());
	
	ResLogin(pClient);
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
	*pack << (WORD)GAME_RES_JOIN_GAME << (UINT)dfROOM_MAX_USER_COUNT;
	
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

	pClient->champ = champ;
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
	int spell1, spell2;

	*pPacket >> spell1 >> spell2;

	pClient->spell1 = spell1;
	pClient->spell2 = spell2;

	AcquireSRWLockExclusive(&roomLocker);
	auto find = roomMap.find(pClient->roomNum);
	if (find == roomMap.end())
	{
		// �濡 �ҼӵǾ� ���� ���� Ŭ���̾�Ʈ
		ReleaseSRWLockExclusive(&roomLocker);
		return;
	}

	Room* room = find->second;
	room->ready[pClient->numInRoom] = true;
	ReleaseSRWLockExclusive(&roomLocker);

	AcquireSRWLockExclusive(&accountLocker);
	accountMap[pClient->nickname.c_str()].roomNum = pClient->roomNum;
	accountMap[pClient->nickname.c_str()].number = pClient->numInRoom;
	accountMap[pClient->nickname.c_str()].champ = pClient->champ;
	accountMap[pClient->nickname.c_str()].spell1 = pClient->spell1;
	accountMap[pClient->nickname.c_str()].spell2 = pClient->spell2;
	ReleaseSRWLockExclusive(&accountLocker);

	ResReady(room->users[pClient->numInRoom]);

	for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
	{
		if (room->ready[i] == false) return;
	}
	// ��� ���� ���¸� ���� ����
	// ���ӹ� ����
	GameRoom* gameRoom = new GameRoom();
	AcquireSRWLockExclusive(&gameroomLocker);
	gameroomMap[room->number] = gameRoom;
	gameRoom->number = room->number;
	ReleaseSRWLockExclusive(&gameroomLocker);

	// ���� �ο� ������� ���ӽ��� ��Ŷ ������
	for (int i = 0; i < dfROOM_MAX_USER_COUNT; i++)
	{
		ResStart(room->users[i]);
	}
}

void CGameServer::ResReady(Client* pClient)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_READY;

	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ResStart(Client* pClient)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_START;

	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ReqEnterGame(Client* pClient, CPacket* pPacket)
{
	WCHAR nick[20] = {};

	pPacket->Dequeue((char*)nick, 40);
	wprintf(L"[INFO] �ΰ��� ���� ��û [Nick : %s]\n", nick);
	// �г������� ���� ���̵� ã��
	AcquireSRWLockExclusive(&accountLocker);
	auto findNick = accountMap.find(nick);
	if (findNick == accountMap.end())
	{
		wprintf(L"[Warning] �������� �ʴ� �г���(call : ReqEnterGame())\n");
		ReleaseSRWLockExclusive(&accountLocker);
		return;
	}

	// ���ο� ������ ���� ����
	pClient->nickname = nick;
	pClient->roomNum = accountMap[nick].roomNum;
	pClient->numInRoom = accountMap[nick].number;
	pClient->champ = accountMap[nick].champ;
	pClient->spell1 = accountMap[nick].spell1;
	pClient->spell2 = accountMap[nick].spell2;
	ReleaseSRWLockExclusive(&accountLocker);

	// ���ȣ�� �ΰ��ӹ� ã��
	AcquireSRWLockExclusive(&gameroomLocker);
	auto find = gameroomMap.find(pClient->roomNum);
	if (find == gameroomMap.end())
	{
		ReleaseSRWLockExclusive(&gameroomLocker);
		wprintf(L"[Warning] ���� ���ȣ (call :ReqEnterGame())\n");
		return;
	}
	ReleaseSRWLockExclusive(&gameroomLocker);

	// �ΰ��� �濡�� ���ο� ���� ���� ����
	GameRoom* room = find->second;
	room->LockUser();
	auto user = room->users.find(pClient->numInRoom);
	if (user == room->users.end())
	{
		room->users[pClient->numInRoom] = pClient;

	}
	wprintf(L"[INFO] �ΰ��� ���� ��û ���� [Nick : %s]\n", nick);
	// �ΰ��� ���� ��� �ο��� �� ��Ŷ�� ���ȴٸ�
	// �� ���� ��� �ο����� ���� ������ �ѱ�
	if (room->users.size() == dfROOM_MAX_USER_COUNT)
	{
		for (auto iter : room->users)
		{
			Client* user = iter.second;
			ResEnterGame(user, room);
		}
	}

	room->UnlockUser();


}

void CGameServer::ResEnterGame(Client* pClient, GameRoom* pRoom)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_ENTER_GAME;
	*pack << (INT)pRoom->users.size();
	for (auto iter : pRoom->users)
	{
		pack->Enqueue((char*)iter.second->nickname.c_str(), 40);
		*pack << iter.second->numInRoom;
		*pack << iter.second->champ;
		*pack << iter.second->spell1;
		*pack << iter.second->spell2;
	}
	
	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ReqLoading(Client* pClient, CPacket* pPacket)
{
	int progress;
	*pPacket >> progress;

	pClient->progress = progress;

	ResLoading(pClient, progress);
}

void CGameServer::ResLoading(Client* pClient, int progress)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_LOADING << pClient->numInRoom << progress;
	AcquireSRWLockExclusive(&gameroomLocker);
	auto find = gameroomMap.find(pClient->roomNum);
	if (find == gameroomMap.end())
	{
		ReleaseSRWLockExclusive(&gameroomLocker);
		wprintf(L"[Warning] ���� ���ȣ (call :ReqEnterGame())\n");
		return;
	}
	ReleaseSRWLockExclusive(&gameroomLocker);

	GameRoom* room = find->second;
	for (auto iter : room->users)
	{
		Client* user = iter.second;
		SendUnicast(user->sessionID, pack);
	}
}

void CGameServer::ReqCompleteLoading(Client* pClient, CPacket* pPacket)
{
	wprintf(L"[INFO] �ΰ��� �ε� �Ϸ� ��û [Nick : %s]\n",pClient->nickname.c_str());
	pClient->isCompleteLoading = true;
	
	// ��ã��
	AcquireSRWLockExclusive(&gameroomLocker);
	auto find = gameroomMap.find(pClient->roomNum);
	if (find == gameroomMap.end())
	{
		ReleaseSRWLockExclusive(&gameroomLocker);
		wprintf(L"[Warning] ���� ���ȣ (call :ReqEnterGame())\n");
		return;
	}
	ReleaseSRWLockExclusive(&gameroomLocker);

	GameRoom* room = find->second;

	for (auto iter : room->users)
	{
		Client* user = iter.second;
		if (user->isCompleteLoading == false) return;
	}

	for (auto iter : room->users)
	{
		Client* user = iter.second;
		ResCompleteLoading(user);
	}
}

void CGameServer::ResCompleteLoading(Client* pClient)
{
	wprintf(L"[INFO] �ΰ��� �ε� �Ϸ� ���� [Nick : %s]\n", pClient->nickname.c_str());

	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_COMPLETE_LOADING;
	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ReqTime(Client* pClient, CPacket* pPacket)
{
	DWORD time;
	*pPacket >> time;
	ResTime(pClient, time);
}

void CGameServer::ResTime(Client* pClient, DWORD time)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_TIME << time;
	SendUnicast(pClient->sessionID, pack);
}

void CGameServer::ReqMove(Client* pClient, CPacket* pPacket)
{
	INT gameID, pathCount;
	float x, y, z;
	list<Vector3> path;

	*pPacket >> gameID >> pathCount;
	
	for (int i = 0; i < pathCount; i++)
	{
		*pPacket >> x >> y >> z;
		Vector3 layover = Vector3(x, y, z);
		path.push_back(layover);
	}
	// ��ã��
	GameRoom* room = gameroomMap[pClient->roomNum];
	for (auto iter : room->users)
	{
		ResMove(iter.second, gameID, path);
	}
}

void CGameServer::ResMove(Client* pClient, int gameID, list<Vector3>& path)
{
	CPacket* pack = PacketPool::Alloc();
	*pack << (WORD)GAME_RES_MOVE << gameID << (int)path.size();
	for (auto iter : path)
	{
		*pack << iter.x << iter.y << iter.z;
	}

	SendUnicast(pClient->sessionID, pack);
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
