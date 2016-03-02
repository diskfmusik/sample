#include <stdio.h>
#include <time.h>
#include <vector>
#include <list>

struct Hoge
{
	int a, b, c, d, e;
	int ary[100];
};

std::vector< Hoge > g_hoge;
std::vector< Hoge* > g_pHoge;
//std::list< Hoge > g_hoge;
//std::list< Hoge* > g_pHoge;

//#define process1
#define process2

void main()
{
	long start = clock();
	{
#ifdef process1
		// vector 2.741 2.597 2.605 (s)
		// list   2.42  2.464 2.43  (s)

		for (int i = 0; i < 1000000; i++)
		{
			Hoge hoge;
			g_hoge.push_back(hoge);
		}
#endif

#ifdef process2
		// vector 1.482 1.568 1.501 (s)
		// list   2.646 2.687 2.67  (s)

		for (int i = 0; i < 1000000; i++)
		{
			g_pHoge.push_back(new Hoge());
		}
#endif
	}
	long goal = clock();

	printf("sec : %f", (float)(goal - start) / CLOCKS_PER_SEC);

	getchar();
}