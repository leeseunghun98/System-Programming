// 20210929structure.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>

// 구조체 선언
//struct complex {
//    double real; // 멤버변수
//    double imag;
//};
//typedef struct complex Complex; // 앞으로 자료형(struct complex)를 Complex로 부름

typedef struct complex {
    double real; // 멤버변수
    double imag;
}Complex, complex1;

int main()
{
    // 주석 ctrl k,c
    //struct complex c1, c2; // 구조체변수 선언
    //c1.real = 3.0; // 구조체변수, 멤버변수
    //c1.imag = 3.0;
    struct complex* ptr1;
    ptr1 = (struct complex*)malloc(sizeof(struct complex));
    // 즉, 모든 구조체는 포인터변수를 통해 접근한다.
    complex1* ptr2;
    ptr2 = (complex1*)malloc(sizeof(Complex));
    ptr1->real = 3.0; // 구조체 포인터 변수 -> 멤버 변수
    ptr1->imag = 4.0;
    ptr2->real = 3.0;
    ptr2->imag = 5.0;
    printf("%f, %f", ptr2->real, ptr2->imag);

    return 0;
}


