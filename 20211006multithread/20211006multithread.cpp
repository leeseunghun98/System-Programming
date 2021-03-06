// 20211006multithread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// createThread함수에 의해 호출되는 함수는
// 반드시 아래와 같은 함수형을 가져야 한다.
// 1, 매개변수는 하나이고 자료형은 LPVOID 즉, void*
// 2. 반환형 DWORD
// 3. calling convention(함수 호출방법) WINAPI
DWORD WINAPI ThreadFunc(LPVOID); // LPVOID ==> void*

int main()
{
    HANDLE hThrd;
    DWORD threadId;
    int i;

    for (i = 0; i < 5; i++)
    {
        hThrd = CreateThread(NULL, // 실한가닥 만들고 ThreadFunc 실행 , 매개변수 더 보내려면 구조체포인터를 전달
            0,
            ThreadFunc, // 함수 포인터
            (LPVOID)i, // 함수의 매개변수, 단 하나만 허락
            0,
            &threadId);

            CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, 0, &threadId);
        if (hThrd)
        {
            printf("Thread launched %d\n", i);
            CloseHandle(hThrd);
        }
    }
    // Wait for the threads to complete.
    // We'll see a better way of doing this later.
    Sleep(2000);

    return EXIT_SUCCESS;
}

// 20초가 다되면 시간이 다돼서 다른 thread가 돌아감, 어떤 thread가 돌아갈진 모름

DWORD WINAPI ThreadFunc(LPVOID in)
{
    int i;
    int n = (int)in;
    for (i = 0; i < 10; i++)
        printf("%d%d%d%d%d%d%d%d\n", n, n, n, n, n, n, n, n);
    return 0;
}