#include "stdafx.h"
#include "Debug.h"

#include <stdarg.h>
#include <strsafe.h>

static Debug* instance;

Debug::Debug()
{
	sprintf_s(saveDirectory,"%s", "LogFiles");
	swprintf(saveDirectoryW, sizeof(saveDirectoryW) / sizeof(WCHAR), L"%s", L"LogFiles");
	if (_wmkdir(saveDirectoryW) != 0)
	{
		// ���� ������ ������ ������ ��� ������ �����ص� ��
		//printf("[Error] Creating Log Directory Failed!!\n");
	}
}

Debug::~Debug()
{
	
}

Debug* Debug::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Debug;
	}
	return instance;
}



void Debug::Print(const char* _text, ...)
{
	va_list ap;
	char buf[dfMAX_BUF_SIZE] = { 0, };
	va_start(ap, _text);
	vsprintf_s(buf, _text, ap);
	va_end(ap);

	fprintf(stdout, "%s", buf);
}

void Debug::PrintLine(const char* _text, ...)
{
	va_list ap;
	char buf[dfMAX_BUF_SIZE] = { 0, };
	va_start(ap, _text);
	vsprintf_s(buf, _text, ap);
	va_end(ap);

	fprintf(stdout, "%s\n", buf);
}

void Debug::Log(LogLevel level, const char* title, const char* format, ...)
{
	Debug* debug = Debug::GetInstance();
	// �α� ī��Ʈ (��Ȯ�� ����)
	DWORD logCnt = InterlockedIncrement(&debug->logCount);
	// �ý��� �ð� 
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	// �α� ����
	CHAR logText[1000] = { 0, };
	int maxLogLen = strlen(logText);
	//�α� �⺻�� DEBUG
	CHAR logType[10];
	switch (level)
	{
	case LogLevel::Debug:
		sprintf_s(logType, "DEBUG");
		break;
	case LogLevel::Warning:
		sprintf_s(logType, "WARNING");
		break;
	case LogLevel::Error:
		sprintf_s(logType, "ERROR");
		break;
	case LogLevel::System:
		sprintf_s(logType, "SYSTEM");
		break;
	}

	// �������� 
	va_list va;
	va_start(va, format);
	int result = StringCchVPrintfA(logText, 1000, format, va);

	if (FAILED(result))
	{
		int count = 0;
		while (SUCCEEDED(result))
		{
			if (count >= 10)
				break;
			maxLogLen = maxLogLen / 2;
			count++;
		}

		if (count >= 10)
		{
			printf("[Error] Debug.Log() Error\n");
			return;
		}
	}
	va_end(va);

	// ���� �̸� �����
	char fileName[128];
	sprintf_s(fileName, "%s\\%d_%02d_%s.txt", debug->saveDirectory, sysTime.wYear, sysTime.wMonth, title);
	// ���� ����
	FILE* fp;
	int err = fopen_s(&fp, fileName, "a+");
	if (fp == NULL)
	{
		printf("[Error] Log File Open Failed / Code : %d\n", err);
		return;
	}
	// ���� �α� �����
	fprintf(fp, "[%s][%d/%02d/%02d][%02d : %02d : %02d][ %s / %09d] - %s\n",
		title, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, logType, logCnt, logText);


	fclose(fp);
	// �ܼ� �α� �����
	printf("[%s] %s\n", title, logText);
}

void Debug::Log(const char* title, const char* format, ...)
{
	// �α� ����
	CHAR logText[1000] = { 0, };
	int maxLogLen = strlen(logText);
	// �������� 
	va_list va;
	va_start(va, format);
	int result = StringCchVPrintfA(logText, 1000, format, va);

	if (FAILED(result))
	{
		int count = 0;
		while (SUCCEEDED(result))
		{
			if (count >= 10)
				break;
			maxLogLen = maxLogLen / 2;
			count++;
		}

		if (count >= 10)
		{
			printf("[Error] Debug.Log() Error\n");
			return;
		}
	}
	va_end(va);

	Debug::Log(LogLevel::Debug, title, format);
}

void Debug::LogWarning(const char* title, const char* format, ...)
{
	// �α� ����
	CHAR logText[1000] = { 0, };
	int maxLogLen = strlen(logText);
	// �������� 
	va_list va;
	va_start(va, format);
	int result = StringCchVPrintfA(logText, 1000, format, va);

	if (FAILED(result))
	{
		int count = 0;
		while (SUCCEEDED(result))
		{
			if (count >= 10)
				break;
			maxLogLen = maxLogLen / 2;
			count++;
		}

		if (count >= 10)
		{
			printf("[Error] Debug.Log() Error\n");
			return;
		}
	}
	va_end(va);

	Debug::Log(LogLevel::Warning, title, format);
}

void Debug::LogError(const char* title, const char* format, ...)
{
	// �α� ����
	CHAR logText[1000] = { 0, };
	int maxLogLen = strlen(logText);
	// �������� 
	va_list va;
	va_start(va, format);
	int result = StringCchVPrintfA(logText, 1000, format, va);

	if (FAILED(result))
	{
		int count = 0;
		while (SUCCEEDED(result))
		{
			if (count >= 10)
				break;
			maxLogLen = maxLogLen / 2;
			count++;
		}

		if (count >= 10)
		{
			printf("[Error] Debug.Log() Error\n");
			return;
		}
	}
	va_end(va);

	Debug::Log(LogLevel::Error, title, format);
}

void Debug::LogSystem(const char* title, const char* format, ...)
{
	// �α� ����
	CHAR logText[1000] = { 0, };
	int maxLogLen = strlen(logText);
	// �������� 
	va_list va;
	va_start(va, format);
	int result = StringCchVPrintfA(logText, 1000, format, va);

	if (FAILED(result))
	{
		int count = 0;
		while (SUCCEEDED(result))
		{
			if (count >= 10)
				break;
			maxLogLen = maxLogLen / 2;
			count++;
		}

		if (count >= 10)
		{
			printf("[Error] Debug.Log() Error\n");
			return;
		}
	}
	va_end(va);

	Debug::Log(LogLevel::System, title, format);
}
