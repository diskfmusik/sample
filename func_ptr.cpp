#include<stdio.h>


class Hoge
{
public:

	Hoge(){
		func = &Hoge::title;// ������
	}

	// �֐��|�C���^
	void(Hoge::*func)();

	void exe(){
		(this->*func)();
	}

	void title(){
		printf("�^�C�g�����\n");
		getchar();
		func = &Hoge::play;
	}

	void play(){
		printf("�v���C���\n");
		getchar();
		func = &Hoge::result;
	}

	void result(){
		printf("���U���g���\n");
		getchar();
		func = &Hoge::title;
	}
};


void main()
{
	Hoge *hoge = new Hoge();

	while (1)
	{
		hoge->exe();
	}

	getchar();
}