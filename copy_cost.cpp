#include <stdio.h>
#include <time.h>

//#define process1
//#define process2
#define calc_time

struct DammyData
{
	int a, b, c, d, e;
	int ary[10000];
};

void func(DammyData _a)
{
	_a.a = 1;
	_a.b = 2;
	_a.c = 3;
	_a.d = 4;
	_a.e = 5;
}

void func(DammyData *_a)
{
	_a->a = 1;
	_a->b = 2;
	_a->c = 3;
	_a->d = 4;
	_a->e = 5;
}

// メモリの節約
// (関数の引数に渡すとき)コピーコストの節約
void main()
{

	{
#ifdef calc_time
		DammyData instance;
		DammyData *pt = &instance;

		long start = clock();
#if 0
		for (int i = 0; i < 1000000; i++)
		{
			func(instance);
		}
#endif

#if 1
		for (int i = 0; i < 1000000; i++)
		{
			func(pt);
		}
#endif
		long goal = clock();

		printf("sec : %f", (float)(goal - start) / CLOCKS_PER_SEC);
#endif // #ifdef calc_time
	}


	{
#ifdef process1
		int a = 10;
		int aa = 100;
		int b = a;
		int& rf = aa; // rf は aa と結合する
					  // int rf = *aa; // イメージ的には

		++b;
		++rf;
		//++*pt;
		printf("a : %d\n", a);
		printf("aa : %d\n", aa);
		printf("b : %d\n", b);
		printf("rf : %d\n", rf);
#endif // #ifdef process1
	}


	{
#ifdef process2
		int a = 10;
		// 参照のアドレス設定は初期化の時のみ
		// ポインタは何時でもアドレス設定が出来る
		int& rf = a;
		int* pt = &a;
		printf("a  : %d\n", a);
		printf("rf : %d\n", rf);
		printf("pt : %d\n", *pt);

		// 参照と設定したオブジェクトアドレスとの結合は、
		// プログラムが終了するまで解かれる事は無い。
		// rf = &a はずっと結合されたまま
		rf = 100;
		//*pt = 100;
		printf("a  : %d\n", a);
		printf("rf : %d\n", rf);
		printf("pt : %d\n", *pt);


		//rf = &a; // エラー
		pt = &a;


		// 要するに、
		// アドレスの操作が出来るか出来ないかの差。
#endif // #ifdef process2
	}


	getchar();
}
