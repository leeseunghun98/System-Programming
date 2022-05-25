#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

// 에러가 났지만 프로젝트 속성 - C/C++ - 언어 - 준수 모드 -> 아니오로 변경하니 실행된다.
// MAX_THREADS인 3개 만큼의 worker thread를 만들어 MyThreadFunction을 수행하게 한다. 이때, 핸들러가 오류가 나면 ErrorHandler함수를 호출한다.

#define MAX_THREADS 3 // 최대 쓰레드 3개
#define BUF_SIZE 255 // 버퍼크기 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam); // 함수 MyThreadFunction의 선언
void ErrorHandler(LPTSTR lpszFunction); // 팜수 ErrorHandler의 선언

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).

typedef struct MyData { // MyData라는 구조체 정의
    int val1; // 구조체의 값1
    int val2; // 구조체의 값2
} MYDATA, * PMYDATA; // struct MyData를 MYDATA와 *PMYDATA라고도 부를 수 있다


int _tmain()
{
    PMYDATA pDataArray[MAX_THREADS]; // PMYDATA 타입의 배열 선언
    DWORD   dwThreadIdArray[MAX_THREADS]; // DWORD 타입의 dwThreadIdArray 선언
    HANDLE  hThreadArray[MAX_THREADS]; // HANDLE 타입의 hThreadArray 선언

    // Create MAX_THREADS worker threads.

    for (int i = 0; i < MAX_THREADS; i++) // 반복문 시작
    {
        // Allocate memory for thread data.

        pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, // pDataArray를 위한 heap공간 생성
            sizeof(MYDATA));

        if (pDataArray[i] == NULL) // pDataArray[i]가 없을경우 ExitProcess함수에 매개변수 2를 넣고 실행
        {
            // If the array allocation fails, the system is out of memory
            // so there is no point in trying to print an error message.
            // Just terminate execution.
            ExitProcess(2); 
        }

        // Generate unique data for each thread to work with.

        pDataArray[i]->val1 = i; // i번째 pDataArray의 val1, val2에 각각 i, i+100으로 초기화
        pDataArray[i]->val2 = i + 100;

        // Create the thread to begin execution on its own.

        hThreadArray[i] = CreateThread( // Worker thread생성 후, 생성된 worker thread는 MyThreadFunction함수(매개변수 pDataArray[i])를 수행한다.
            NULL,                   // default security attributes
            0,                      // use default stack size  
            MyThreadFunction,       // thread function name
            pDataArray[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 


        // Check the return value for success.
        // If CreateThread fails, terminate execution. 
        // This will automatically clean up threads and memory. 

        if (hThreadArray[i] == NULL) // i번째 쓰레드가 없을 경우 -> 쓰레드의 생성이 제대로 되지 않았을 경우
        {
            ErrorHandler(TEXT("CreateThread")); // ErrorHandler 함수 호출
            ExitProcess(3); // ExitProcess 함수호출, 프로세스 종료 코드 3
        }
    } // End of main thread creation loop.

    // Wait until all threads have terminated.
    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE); // 모든(TRUE라서) 쓰레드가 종료되어야 넘어간다. 끝나지 않으면 시간은 무한정 기다린다.

    // Close all thread handles and free memory allocations.

    for (int i = 0; i < MAX_THREADS; i++) // MAX_THREADS의 갯수만큼 반복문 실행
    {
        CloseHandle(hThreadArray[i]); // 반복문을 이용해 모든 Handle을 닫아준다
        if (pDataArray[i] != NULL) // 만약, pDataArray가 존재할 경우
        {
            HeapFree(GetProcessHeap(), 0, pDataArray[i]); // 존재하는 heap공간을 free해준다.
            pDataArray[i] = NULL;    // free해준 후 NULL을 대입한다.
        }
    }

    return 0; // 메인 함수 종료
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam) // 위에서 선언했던 MyThreadFunction 함수 정의
{
    HANDLE hStdout; // 핸들 타입의 변수 선언
    PMYDATA pDataArray; // PMYDATA 타입의 변수 선언

    TCHAR msgBuf[BUF_SIZE]; // TCAHR 타입의 변수 선언
    size_t cchStringSize; // size_t 타입의 변수 선언
    DWORD dwChars; // DWORD 타입의 변수 선언

    // Make sure there is a console to receive output results. 

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); // 핸들 타입의 변수 hStdout에 handle을 달아줌
    if (hStdout == INVALID_HANDLE_VALUE) // 핸들이 정상적이지 않을 경우
        return 1; // 함수 종료 후 1반환

    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because 
    // it was checked for NULL before the thread was created.

    pDataArray = (PMYDATA)lpParam; // pDataArray라는 PMYDATA타입의 변수에 매개변수를 형변환 후 초기화

    // Print the parameter values using thread-safe functions.

    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"), // msgBuf 버퍼 변수에, 저장할 버퍼의 총 크기=BUF_SIZE, 문자열 포맷
        pDataArray->val1, pDataArray->val2); // pDataArray의 val1, val2에 저장한다.
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize); // msgBuf의 문자열 크기를 체크
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL); // Console창에 msgBuf에 포함된 문자 출력, 쓸 문자 수 : (DWORD)cchStringSize

    return 0; // 함수종료
}



void ErrorHandler(LPTSTR lpszFunction) // ErrorHandler 함수의 정의
{
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf; // LPVOID=void* 형 변수 선언
    LPVOID lpDisplayBuf; // LPVOID=void* 형 변수 선언
    DWORD dw = GetLastError(); // 가장 최근의 에러를 dw변수에 담는다

    FormatMessage( // FormatMessage 함수를 이용하여 에러 코드를 에러 메시지로 변환
        FORMAT_MESSAGE_ALLOCATE_BUFFER | // 에러 메시지를 저장할 공간을 해당 함수가 알아서 할당
        FORMAT_MESSAGE_FROM_SYSTEM | // 운영체제로 부터 오류 메시지를 가져옴
        FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL,
        dw, // 에러 메세지로 변환할 에러 코드는 dw
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 에러 메시지를 표현할 언어
        (LPTSTR)&lpMsgBuf, // 에러 메시지의 시작 주소 = &IpMsgBuf
        0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, // IpDisplayBuf를 위한 공간 확보
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); // (크기는 IpMsgBuf의 길이 + IpszFunction의 길이 + 40)*TCHAR의크기
    StringCchPrintf((LPTSTR)lpDisplayBuf, // IpDisplayBuf에 2번째 인자만큼 저장하며, TEXT에 있는 문자열에 dw의 에러코드를 붙힌다.
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); // 메세지 창이 나오게한다. 창의 이름이 Error이며, 글 내용이 IpDisplayBuf의 문자열이다.

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf); // lpMsgBuf 할당 공간 해제
    LocalFree(lpDisplayBuf); // lpDisplayBuf 할당 공간 해제
}