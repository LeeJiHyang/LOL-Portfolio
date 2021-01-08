#include "stdafx.h"
#include "GameRoom.h"

GameRoom::GameRoom()
{
	InitializeSRWLock(&userLocker);
}

GameRoom::~GameRoom()
{
	releaseFlag = true;

	if (isStarted)
	{
		CloseHandle(hGameLogicThread);
	}
}

void GameRoom::StartGame()
{
	if (isStarted) return;
	isStarted = true;
	hGameLogicThread = (HANDLE)_beginthreadex(NULL, NULL, GameLogicThread, this, 0, &threadID);
}

unsigned __stdcall GameRoom::GameLogicThread(LPVOID arg)
{
	GameRoom* room = (GameRoom*)arg;
	room->UpdateGameLogic();
	return 0;
}

void GameRoom::UpdateGameLogic()
{
	printf("[INFO] ���ӽ��� [���ȣ : %d]", number);
	while (!releaseFlag)
	{

	}
	printf("[INFO] �������� [���ȣ : %d]", number);
}

void GameRoom::LockUser()
{
	AcquireSRWLockExclusive(&userLocker);
}

void GameRoom::UnlockUser()
{
	ReleaseSRWLockExclusive(&userLocker);
}
