#pragma once
#include <iostream>
#include <Windows.h>

#define dfPACKET_DEFAULT_SIZE 1024
#define dfPACKET_CODE 0x77
#define dfPACKET_FIXKEY 0x32

#pragma pack(push,1)
struct NetHeader
{
	BYTE code;
	WORD len;
	BYTE randKey;
	BYTE checkSum;
};
#pragma pack(pop)

class CPacket
{
public:
	CPacket();
	CPacket(int iBufferSize);
	CPacket(const CPacket& clSrcPacket);
	virtual ~CPacket();

protected:
	void Initialize(int iBufferSize = dfPACKET_DEFAULT_SIZE);
	void Release(void);
	bool Resize(unsigned int resize);

public:
	void Clear(void);
	// ���� �� ũ��
	int GetBufferSize(void);
	// ����ϰ� �ִ� ũ��
	int GetUseSize(void);
	// ���� 0�� ������
	char* GetBufferPtr(void);
	// ���� read��ġ ������
	char* GetReadPtr(void);
	// ���� write��ġ ������
	char* GetWritePtr(void);
	// read ��
	int GetReadPos(void);
	// write ��
	int GetWritePos(void);
	int MoveWritePos(int iSize);
	int MoveReadPos(int iSize);

	void SetHeader(WORD& header);
	void SetHeader(NetHeader& header);

	void Encryption();
	bool Decryption(NetHeader& header);

	CPacket& operator = (const CPacket& pack);

	CPacket& operator << (bool value);
	CPacket& operator << (char value);
	CPacket& operator << (short	value);
	CPacket& operator << (int value);
	CPacket& operator << (long value);
	CPacket& operator << (float	value);
	CPacket& operator << (double value);
	CPacket& operator << (__int64 value);
	CPacket& operator << (unsigned char value);
	CPacket& operator << (unsigned short value);
	CPacket& operator << (unsigned int value);
	CPacket& operator << (unsigned long value);
	CPacket& operator << (unsigned __int64 value);

	CPacket& operator >> (bool& value);
	CPacket& operator >> (char& value);
	CPacket& operator >> (short& value);
	CPacket& operator >> (int& value);
	CPacket& operator >> (long& value);
	CPacket& operator >> (float& value);
	CPacket& operator >> (double& value);
	CPacket& operator >> (__int64& value);
	CPacket& operator >> (unsigned char& value);
	CPacket& operator >> (unsigned short& value);
	CPacket& operator >> (unsigned int& value);
	CPacket& operator >> (unsigned long& value);
	CPacket& operator >> (unsigned __int64 value);

	int Enqueue(char* chpSrc, int iSize);
	int Dequeue(char* chpDest, int iSize);

	void PrintBuffer()
	{
		//printf("-----------------------\n");
		//printf("\nGetUseSize : %d\nGetFreeSize : %d\nFront : %d\nRear : %d\n\n", GetUseSize(), GetFreeSize(), front, rear);
		printf("{ ");
		for (unsigned int i = 0; i < size; i++)
		{
			if (i == read) { printf(">"); }
			if (i == write) { printf("<"); }
			printf("%d", buffer[i]);
		}
		printf(" }\n");
		//printf("-----------------------\n");
	}

protected:
	char* buffer = nullptr;
	unsigned int read = 0;
	unsigned int write = 0;
	unsigned int size = 0;
	unsigned int useSize = 0;
	bool isEncryption = false;
	unsigned char fixationKey = dfPACKET_FIXKEY;
};

