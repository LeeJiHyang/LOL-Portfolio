#pragma once


#define dfMAX_WORKER 8
#define dfMAX_THREAD (2+dfMAX_WORKER)


using namespace std;

class SessionList;
typedef __int64 SESSION_ID;
typedef unsigned short HEADER;


class CNetwork
{
public:
	CNetwork();
	virtual ~CNetwork();

private:
	bool InitializeNetwork();
	void ReleaseNetwork();

	// ������
	static unsigned __stdcall WorkerThread(void* arg);
	static unsigned __stdcall AcceptThread(void* arg);
	static unsigned __stdcall SendThread(void* arg);

	// ��Ʈ��ũ
	int PostSend(Session* pSession);
	int PostRecv(Session* pSession);
protected:
	virtual void OnRecv(SESSION_ID, CPacket*) = 0;
	virtual void OnSend(SESSION_ID) = 0;
	virtual void OnJoin(SESSION_ID, SOCKADDR_IN) = 0;
	virtual void OnLeave(SESSION_ID) = 0;
	virtual void OnError(SESSION_ID) = 0;
public:
	void RecvProc(Session* pSession);
	int SendUnicast(SESSION_ID sessionID, CPacket* pPacket);
	int SendBroadcast(CPacket* pPacket);
	bool Disconnect(SESSION_ID sessionID);
	bool Disconnect(Session* pSession);

	// ����͸��� ���� ����
	unsigned GetSessionSize(); 
private:
	// ������ ����
	HANDLE hThread[dfMAX_THREAD];
	unsigned ID[dfMAX_THREAD];
	// ���� ����
	SessionList sessionList;
	// ��Ʈ��ũ ����
	SOCKET listenSock = INVALID_SOCKET;
	HANDLE hIOCP = INVALID_HANDLE_VALUE;

protected:
	// ��ŶǮ
	PacketPool* packPool;
	
public:
	// ����� ��
	__int64 threadLoopCount = 0;
	__int64 connectCount = 0;
	__int64 connectTotal = 0;
	__int64 connectFail = 0;
	__int64 recvTPS = 0;
	__int64 sendTPS = 0;

};




