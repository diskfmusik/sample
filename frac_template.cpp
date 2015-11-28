#include <stdio.h>

// �v���C�}���e���v���[�g
template<int N>
struct factorial
{
	static const int value = N * factorial<N - 1>::value;
};

// �����I���ꉻ
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