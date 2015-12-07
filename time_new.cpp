#include <stdio.h>
#include <Windows.h>
#include <time.h>


class Hoge
{
public:
	int a; int b; int c; int d;	int e;

	Hoge(const int _a, const int _b, const int _c, const int _d, const int _e)
	{
		a = _a; b = _b; c = _c; d = _d; e = _e;
	}
};


class Piyo
{
public:
	int a; int b; int c; int d; int e;

	Piyo(const int _a, const int _b, const int _c, const int _d, const int _e) :
		a(_a), b(_b), c(_c), d(_d), e(_e)
	{ }
};


void main()
{
	long a = clock();

#if 0
	for (int i = 0; i < 10000000; i++)// 2358, 2317, 2244, 2264
	{
		Hoge* hoge = new Hoge(i, i, i, i, i);
	}
#endif

	/* ‰Šú‰»ŽqƒŠƒXƒg */
#if 1
	for (int i = 0; i < 10000000; i++)// 2241, 2225, 2242, 2290
	{
		Piyo* piyo = new Piyo(i, i, i, i, i);
	}
#endif

	long b = clock();

	printf("sec : %lu", b - a);
}