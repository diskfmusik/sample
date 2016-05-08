#include <stdio.h>
#include <functional>


// ƒ‰ƒ€ƒ_®‚ğˆø”‚É
void func(std::function< void() > _f)
{
	_f();
}


void func(std::function< int(int _n) > _f)
{
	//_f(_n);
}


void main()
{

#if 0
	int a = 0;

	// ƒ‰ƒ€ƒ_® –³–¼ŠÖ”
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