#include<stdio.h>


class Base {
public:
	Base() {
		printf("Base_コンストラクタ\n");
	}

	~Base() {
		printf("Base_デストラクタ\n");
	}

	virtual void func() {
		printf("Base_func\n");
	}
};




class Hoge :public Base {
public:
	Hoge() {
		printf("Hoge_コンストラクタ\n");
	}

	~Hoge() {
		printf("Hoge_デストラクタ\n");
	}

	virtual void func() {
		printf("Hoge_func\n");
	}
};




void main() {
	Hoge *hoge = new Hoge();
	Base *base = hoge;//アップキャスト

	base->func();

	delete hoge;

	getchar();
}