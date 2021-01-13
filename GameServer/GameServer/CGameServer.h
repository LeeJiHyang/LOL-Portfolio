#pragma once
#include "CNetwork.h"
#include "CLanLoginClient.h"
#include "CLanGameLogicClient.h"
#include "GameRoom.h"

#define dfMAX_THREAD_GAMESERVER 1
#define dfROOM_MAX_USER_COUNT 2

using namespace std;

typedef __int64 SESSION_ID;
typedef __int64 CLIENT_ID;

struct Client
{
	SESSION_ID	sessionID = -1;
	SOCKADDR_IN	sockaddr;
	wstring		nickname;
	INT			roomNum = -1; // ���ȣ
	INT			numInRoom = -1; // ��ȿ��� ��ȣ(0~9)
	BYTE		loginStatus = false;
	BYTE		champ;
	INT			progress = 0;
	bool		isCompleteLoading = false;
	BYTE		spell1 = 6;
	BYTE		spell2 = 3;
};

struct Account
{
	wstring nickname;
	INT		roomNum = -1;
	INT		number = -1;
	BYTE	champ = 0;
	BYTE	spell1 = 6;
	BYTE	spell2 = 3;
};

struct Room
{
	INT number;
	Client* users[dfROOM_MAX_USER_COUNT] = {};
	bool ready[dfROOM_MAX_USER_COUNT] = {};
};

class CGameServer : public CNetwork
{
public:
	CGameServer();
	virtual ~CGameServer();
public:
	virtual void Shutdown();
private:
	void InitializeGameServer();
	void ReleaseGameServer();
public:

	virtual void OnRecv(SESSION_ID sessionID, CPacket* pPacket);
	virtual void OnSend(SESSION_ID sessionID);
	virtual void OnJoin(SESSION_ID sessionID, SOCKADDR_IN sockaddr);
	virtual void OnLeave(SESSION_ID sessionID);
	virtual void OnError(SESSION_ID sessionID);


	// ������
	static unsigned WINAPI UpdateThread(LPVOID arg);
	void Update();

	// Ŭ���̾�Ʈ ����
	Client* CreateClient(SESSION_ID sessionID, SOCKADDR_IN sockaddr);
	void	DeleteClient(SESSION_ID sessionID);
	Client* FindClient(SESSION_ID sessionID);

	void PacketProc(SESSION_ID sessionID, CPacket* pPacket);

	void ReqTest(Client* pClient, CPacket* pPacket);
	void ResTest(Client* pClient);

	void ReqNick(Client* pClient, CPacket* pPacket);
	void ResNick(Client* pClient);
	void ReqLogin(Client* pClient, CPacket* pPacket);
	void ResLogin(Client* pClient);
	void ReqJoinGame(Client* pClient, CPacket* pPacket);
	void ResJoinGame(Client* pClient, Room* room);
	void ReqSelectChamp(Client* pClient, CPacket* pPacket);
	void ResSelectChamp(Client* pClient, INT userNum, BYTE champ);
	void ReqReady(Client* pClient, CPacket* pPacket);
	void ResReady(Client* pClient);
	void ResStart(Client* pClient);
	// ������� �ΰ��� ����
	void ReqEnterGame(Client* pClient, CPacket* pPacket);
	void ResEnterGame(Client* pClient, GameRoom* pRoom);
	void ReqLoading(Client* pClient, CPacket* pPacket);
	void ResLoading(Client* pClient, int progress);
	void ReqCompleteLoading(Client* pClient, CPacket* pPacket);
	void ResCompleteLoading(Client* pClient);


public:
	void Monitor();

private:
	
	// ������ ����
	HANDLE hLogicThread[dfMAX_THREAD_GAMESERVER];
	unsigned threadID[dfMAX_THREAD_GAMESERVER];
	bool releaseFlag = false;

	// Ŭ���̾�Ʈ ����
	map<SESSION_ID, Client*> clientMap;
	SRWLOCK clientLocker;
	map<wstring, SESSION_ID> nickMap;
	SRWLOCK nickLocker;
	// �г��ӿ� ���� ��������
	// �ʱ����ӽ� ����
	// ���� ������ ����Ǿ ����(��������� ����)
	map<wstring, Account> accountMap;
	SRWLOCK accountLocker;
	// ��
	// ��⿭
	queue<Client*> readyQ;
	map<INT, Room*> roomMap;
	SRWLOCK roomLocker;

	map<INT, GameRoom*> gameroomMap;
	SRWLOCK gameroomLocker;


	// �� ����
	MemoryPool<CJob>* jobPool;
	CircularList<IJob*>* jobQueue;

public:

};

