#include<stdio.h>


class Base {
public:
	Base() {
		printf("Base_�R���X�g���N�^\n");
	}

	~Base() {
		printf("Base_�f�X�g���N�^\n");
	}

	virtual void func() {
		printf("Base_func\n");
	}
};




class Hoge :public Base {
public:
	Hoge() {
		printf("Hoge_�R���X�g���N�^\n");
	}

	~Hoge() {
		printf("Hoge_�f�X�g���N�^\n");
	}

	virtual void func() {
		printf("Hoge_func\n");
	}
};




void main() {
	Hoge *hoge = new Hoge();
	Base *base = hoge;//�A�b�v�L���X�g

	base->func();

	delete hoge;

	getchar();
}