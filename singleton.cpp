#include<stdio.h>


class Hoge
{
private:
	Hoge() {}

public:
	static Hoge* GetInstance();

	void func() { printf("Hoge_func"); }
};


Hoge* Hoge::GetInstance()
{
	static Hoge* instance = nullptr;

	if (instance == nullptr)
	{
		instance = new Hoge();
	}

	return instance;
}


void main()
{
	Hoge::GetInstance()->func();

	getchar();
}