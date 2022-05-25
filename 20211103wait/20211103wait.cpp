// 20211103wait.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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
    HANDLE hThrd[5];
    DWORD threadId;
    int i;

    for (i = 0; i < 5; i++)
    {
        hThrd[i] = CreateThread(NULL, // 스레드KO(Kernel Object) 생성 후 핸들을 반환한다.
            0,
            ThreadFunc, // 함수 포인터
            (LPVOID)i, // 함수의 매개변수, 단 하나만 허락
            0,
            &threadId);

        CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, 0, &threadId);
        if (hThrd[i])
        {
            printf("Thread launched %d\n", i);
            // CloseHandle(hThrd);
        }
        else { // 스레드 생성에 실패하면 hThrd == NULL
            printf("Error! while creating a thread\n");
            return 0;
        }
    }
    // Wait for the threads to complete.
    // We'll see a better way of doing this later.
    // Sleep(2000); // 이 방법이 정답은 아님
    // 스레드가 제대로 종료하였는 지 알수 있는 방법
    // 1. 모든 KO는 두 가지 상태(state)를 가진다: signaled / nonsignaled
    // 2. state의 의미는 KO마다 다르다.
    // 3. 스레드 KO의 상태 : s -> 종료, ns -> 수행중
    // 4. KO의 상태를 알 수 있는 API : WaitforSingleObject() / WaitforMultipleObjects()
    // WaitForSingleObject(hThrd[4], INFINITE); // hThrd[4] 가 signaled될 때까지 INFINITE(ms) 만큼 대기
    WaitForMultipleObjects(5, hThrd, TRUE,INFINITE); // 모든 스레드가 종료되어야 넘어감, 배열 이름을 바로 쓰면 첫번 째 데이터값의 주소, 3번째 인자 true면 5개 전부, false면 or -> 1개
    for (i = 0; i < 5; i ++) {
        CloseHandle(hThrd[i]); // 1. 스레드 실행 종료 2. CloseHandle() 스레드 KO를 소멸
    }
    return EXIT_SUCCESS;
}


DWORD WINAPI ThreadFunc(LPVOID in)
{
    int i;
    int n = (int)in;
    for (i = 0; i < 10; i++)
        printf("%d%d%d%d%d%d%d%d\n", n, n, n, n, n, n, n, n);
    return 0;
}