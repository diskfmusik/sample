#include<stdio.h>


class Hoge
{
public:

	Hoge(){
		func = &Hoge::title;// 初期化
	}

	// 関数ポインタ
	void(Hoge::*func)();

	void exe(){
		(this->*func)();
	}

	void title(){
		printf("タイトル画面\n");
		getchar();
		func = &Hoge::play;
	}

	void play(){
		printf("プレイ画面\n");
		getchar();
		func = &Hoge::result;
	}

	void result(){
		printf("リザルト画面\n");
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