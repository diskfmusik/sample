#include <stdio.h>
#include <time.h>
#include <vector>
#include <list>

struct Hoge
{
	Hoge(const int _a) { a = _a; }
	int a, b, c, d, e;
	int ary[100];
};

//std::vector< Hoge > g_hoge;
//std::vector< Hoge* > g_pHoge;
std::list< Hoge > g_hoge;
std::list< Hoge* > g_pHoge;

//#define process1
#define process2

void main()
{
#ifdef process1
	// vector 2.741 2.597 2.605 (s)
	// list   2.42  2.464 2.43  (s)

	for (int i = 0; i < 1000000; i++)
	{
		Hoge hoge(i % 5);
		g_hoge.push_back(hoge);
	}
#endif

#ifdef process2
	// vector 1.482 1.568 1.501 (s)
	// list   2.646 2.687 2.67  (s)

	for (int i = 0; i < 1000000; i++)
	{
		g_pHoge.push_back(new Hoge(i % 5));
	}
#endif


	long start = clock();
	{
#ifdef process1
		// vector(配列アクセス) 0.373 0.379 0.38
		// vector(イテレータ)   1.789 1.793 1.792
		// list                 2.001 1.999 1.992

		// ↓ erase付き
		// vector 計測不可能
		// list   2.773 2.785 2.783


		//for (int i = 0; i < 1000000; i++)
		//{
		//	g_hoge[i].a++;
		//}

		auto it = g_hoge.begin();
		while (it != g_hoge.end())
		{
			if ((*it).a == 1)
			{
				it = g_hoge.erase(it);
				continue;
			}

			(*it).a++;
			it++;
		}

#endif

#ifdef process2
		// vector(配列アクセス) 0.445 0.366 0.369
		// vector(イテレータ)   1.81  1.825 1.808
		// list                 2.112 2.013 2.001

		// ↓ erase付き
		// vector 22.782 22.132 22.923
		// list   2.898  2.890  2.885


		//for (int i = 0; i < 1000000; i++)
		//{
		//	g_pHoge[i]->a++;
		//}

		auto it = g_pHoge.begin();
		while (it != g_pHoge.end())
		{
			if ((*it)->a == 1)
			{
				delete (*it);
				it = g_pHoge.erase(it);
				continue;
			}

			(*it)->a++;
			it++;
		}

#endif
	}
	long goal = clock();

	printf("sec : %f", (float)(goal - start) / CLOCKS_PER_SEC);

	getchar();
}