﻿// 20211117samethread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int main()
{
	CRITICAL_SECTION cs1;
	InitializeCriticalSection(&cs1); // 수행 후 깃발이 내려가 있음(들어올 수 있음)
	EnterCriticalSection(&cs1); // 수행 후 깃발이 올라가 있음(못 들어옴)
	printf("we are taking cs1\n");
	EnterCriticalSection(&cs1); // Mutex랑 CriticalSection에서는 동일 thread에 대해 들어갈 수 있음
	printf("we are taking cs1?????\n");

	//Mutex
	HANDLE hM;
	hM = CreateMutex(NULL, FALSE, NULL); // FALSE는 처음에 깃발 내린상태
	WaitForSingleObject(hM, INFINITE);
	printf("we are taking hM\n");
	WaitForSingleObject(hM, INFINITE);
	printf("we are taking hM?????\n");

	
	return 0;
}
