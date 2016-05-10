#include <stdio.h>
#include <functional>


// ラムダ式を引数に
void func(std::function< void() > _f)
{
	_f();
}


void func(std::function< void(int) > _f, int _n)
{
	_f(_n);
}


void main()
{
    func( [](int n) { printf("%d\n", n); }, 10 );

#if 0
	int a = 0;

	// ラムダ式 無名関数
	auto func = [&]()
	{
		a = 50;
		printf("test\n");
	};

	func();
	printf("a = %d\n", a);
#endif


#if 0
	int* b = &a;

	auto func2 = [&]()
	{
		int c = 100;
		b = &c;
	};

	func2();
	printf("b = %d\n", *b);
#endif


	auto f1 = []()
	{
		printf("f1\n");
	};
	auto f2 = []()
	{
		printf("f2\n");
	};

	func(f1);
	func(f2);


	getchar();
}
