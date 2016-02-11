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

// �������̐ߖ�
// (�֐��̈����ɓn���Ƃ�)�R�s�[�R�X�g�̐ߖ�
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
		int& rf = aa; // rf �� aa �ƌ�������
					  // int rf = *aa; // �C���[�W�I�ɂ�

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
		// �Q�Ƃ̃A�h���X�ݒ�͏������̎��̂�
		// �|�C���^�͉����ł��A�h���X�ݒ肪�o����
		int& rf = a;
		int* pt = &a;
		printf("a  : %d\n", a);
		printf("rf : %d\n", rf);
		printf("pt : %d\n", *pt);

		// �Q�ƂƐݒ肵���I�u�W�F�N�g�A�h���X�Ƃ̌����́A
		// �v���O�������I������܂ŉ�����鎖�͖����B
		// rf = &a �͂����ƌ������ꂽ�܂�
		rf = 100;
		//*pt = 100;
		printf("a  : %d\n", a);
		printf("rf : %d\n", rf);
		printf("pt : %d\n", *pt);


		//rf = &a; // �G���[
		pt = &a;


		// �v����ɁA
		// �A�h���X�̑��삪�o���邩�o���Ȃ����̍��B
#endif // #ifdef process2
	}


	getchar();
}
