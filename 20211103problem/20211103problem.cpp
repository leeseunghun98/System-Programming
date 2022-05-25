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

typedef struct node {
	int data;
	struct node* pNext;
}Node;

// 함수 : createNode()
// 입력 : data
// 출력 : 생성한 노드의 포인터
Node* createNode(int value) {
	Node* ptr;
	ptr = (Node*)malloc(sizeof(Node));
	ptr->data = value;
	ptr->pNext = NULL;
	return ptr;
}

// 함수 : insertHead()
// 입력 : 헤드노드포인터, 새 노드 포인터
// 출력 : 없음
void insertHead(Node** pHead, Node* newNode) {
	newNode->pNext = *pHead; // #1
	*pHead = newNode; // #2
}

// 함수 : countNode()
// 입력 : pHead
// 출력 : 노드 갯수
int countNode(Node* pHead) {
	Node* ptr = pHead;
	int cnt = 0;
	while (ptr != NULL) {
		cnt++;
		ptr = ptr->pNext;
	}
	return cnt;
}

int main()
{
	HANDLE hThrd[5];
	DWORD threadId;
	int i;
	Node* pHead = NULL;

	for (i = 0; i < 5; i++)
	{
	    hThrd[i] = CreateThread(NULL, // 스레드KO(Kernel Object) 생성 후 핸들을 반환한다.
	        0,
	        ThreadFunc, // 함수 포인터
	        (LPVOID)&pHead, // 함수의 매개변수, 단 하나만 허락
	        0,
	        &threadId);
;
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
	WaitForMultipleObjects(5, hThrd, TRUE, INFINITE); // 모든 스레드가 종료되어야 넘어감, 배열 이름을 바로 쓰면 첫번 째 데이터값의 주소, 3번째 인자 true면 5개 전부, false면 or -> 1개
	for (i = 0; i < 5; i++) {
	    CloseHandle(hThrd[i]); // 1. 스레드 실행 종료 2. CloseHandle() 스레드 KO를 소멸
	}
	//ThreadFunc(&pHead);
	printf("the number of Nodes is %d", countNode(pHead));
	return EXIT_SUCCESS;
}

// weight condition - 500만개 다 안뜨는 이유
DWORD WINAPI ThreadFunc(LPVOID ptr)
{
	Node** ppHead = (Node**)ptr;
	for (int i = 0; i < 1000000; i++)
		insertHead(ppHead, createNode(1));
	return 0;
}