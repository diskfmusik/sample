#include<stdio.h>
#include<time.h>
#include<Windows.h>


void main()
{
	srand((unsigned char)time(NULL));

	const int max = 100;
	int array[max];

	/* ��芸�����A1~100���i�[���� */
	for (int i = 0; i < max; i++)
	{
		array[i] = i + 1;
	}

	/* �\�[�g�̗v�̂ŃV���b�t������ */
	for (int i = 0; i < max; i++)
	{
		int num = rand() % max;// ������ɔz��ԍ����擾
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