// 20211006functionpointer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
// 함수 선언
// 1. sum은 함수의 이름
// 2. 매개변수의 갯수와 각각의 자료형
// 3. 반환형
int sum(int a, int b) {
    return a + b;
}


int main()
{
    int a = 10; // 변수 선언: 1. a는 변수의 이름 2. a의 자료형은 int
    int* ptr, *varptr;
    ptr = &a;
    *ptr = 100;
    printf("a = %d\n", a);

    // function pointer
    // 함수 포인터 선언
    // 1. fptr : 함수포인터변수 : 함수가 시작하는 주소를 가리킨다.
    // 2. 함수의 매개변수의 갯수와 각각의 자료형
    // 3. 반환형
    int (*fptr) (int, int);//
    fptr = sum;// fptr = &sum;
    int c;
    c = fptr(10, 20);
    printf("c = %d\n", c);

    // void*
    void* vptr;
    vptr = ptr; // OK
    // varptr = vptr; // Error
    varptr = (int*) vptr;
    vptr = (void*) a;


    return 0;
}