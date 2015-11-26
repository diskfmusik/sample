#include <stdio.h>

template<int N>
struct increment
{
	static const int value = N + 1;
};


void main()
{
	// テンプレートのインスタンス化を利用して、
	// コンパイル時に処理を行うクラステンプレートを
	// メタ関数という。
	int n = increment<3>::value;

	getchar();
}