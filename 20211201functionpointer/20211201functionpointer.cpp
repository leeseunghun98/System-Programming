//제목:파일포인터 
//기능: 파일포인터사용법을 연습
//파일이름: functionpointer.cpp
//수정날짜: 2016년12월14일
//작성자: 최성종
#include <stdio.h>
int sum(int a, int b) {
	return (a + b);
}
int minus(int a, int b) {
	return (a - b);
}
//함수포인터의 타입을 정의한다.
//즉, myFunctType은 int를 반환하고, 매개변수가 2개이고, 각각 int형인
//함수포인터형니다.
typedef int (myFunctType)(int, int);

//함수 포인터를 매개변수로 사용할 경우 함수선언이 일치해야 한다.
int what(int a, int b, myFunctType pFcnt) {
	//int what(int a, int b, int (pFcnt) (int, int) ) {
	int c;
	c = pFcnt(a, b);
	return c;
}

void ppp(int a, int b) {
	a = a - b;
}

int main() {
	int x = 100, y = 50, z;
	//함수 포인터: 함수가 기계어로 번역되어 수행하기 위해 메모리에 올라가는데
	//이 함수가 시작하는 메모리의 첫 주소가 함수 포인터가 된다.

	//함수의 이름만을 사용하면 함수의 포인터가 된다.
	//함수 포인터를 매개변수로 사용할 경우 함수선언이 일치해야 한다.
	z = what(x, y, sum);
	printf("The sum is %d\n", z);

	z = what(x, y, minus);
	printf("The minus is %d\n", z);

	//error: 함수선언이 일치하지 않는다.
	//z = what(x, y, ppp); 

	return 0;
}


