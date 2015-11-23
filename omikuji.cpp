#include<stdio.h>
#include<time.h>
#include<Windows.h>


void main()
{
	srand((unsigned char)time(NULL));

	const int max = 100;
	int array[max];

	/* 取り敢えず、1~100を格納する */
	for (int i = 0; i < max; i++)
	{
		array[i] = i + 1;
	}

	/* ソートの要領でシャッフルする */
	for (int i = 0; i < max; i++)
	{
		int num = rand() % max;// 無造作に配列番号を取得
		int temp = array[num];
		array[num] = array[i];
		array[i] = temp;
	}

	for (int i = 0; i < max; i++)
	{
		printf("%d\n", array[i]);
	}

	getchar();
}