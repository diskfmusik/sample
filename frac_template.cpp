#include <stdio.h>

// プライマリテンプレート
template<int N>
struct factorial
{
	static const int value = N * factorial<N - 1>::value;
};

// 明示的特殊化
template<>
struct factorial<0>
{
	static const int value = 1;
};


void main()
{
	int result = factorial<3>::value;

	printf("%d", result);

	getchar();
}