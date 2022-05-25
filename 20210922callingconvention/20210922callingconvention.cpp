// 20210922callingconvention.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <windows.h>
int sum(int, int);
int WINAPI wsum(int, int);

int main()
{
	int a, b, c;
	a = 10;
	b = 20;
	c = a + b;
	c = sum(a, b);
	c = wsum(a, b);

	return 0;
}

int sum(int x, int y) {
	int z;
	z = x + y;
	return z;
}
int WINAPI wsum(int x, int y) { // 모든 Windows API
	int z;
	z = x + y;
	return z;
}