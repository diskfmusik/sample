#include<stdio.h>
#include<memory>//スマートポインタ


//スマートポインタ

class Hoge;//クラスの前方宣言
typedef std::shared_ptr<Hoge> HogeSP;
typedef std::weak_ptr<Hoge> HogeWP;

class Piyo;//クラスの前方宣言
typedef std::shared_ptr<Piyo> PiyoSP;
class Fuga;//クラスの前方宣言
typedef std::shared_ptr<Fuga> FugaSP;
class Foo;//クラスの前方宣言
typedef std::shared_ptr<Foo> FooSP;


class Piyo{
public:

	Piyo(){
		printf("Piyoが生成されました\n");
	}
	~Piyo(){
		printf("Piyoが解放されました\n");
	}
};
class Fuga{
public:

	Fuga(){
		printf("Fugaが生成されました\n");
	}
	~Fuga(){
		printf("Fugaが解放されました\n");
	}
};
class Foo{
public:

	Foo(){
		printf("Fooが生成されました\n");
	}
	~Foo(){
		printf("Fooが解放されました\n");
	}
};


class Hoge{
public:
	int _a;

	/*SPへの橋渡し*/
	HogeWP my_weak_ptr;//thisポインタの代わり
	PiyoSP piyo;
	FugaSP fuga;
	FooSP foo;

	static HogeSP create();

	Hoge(){
		piyo = PiyoSP(new Piyo());
		printf("Hogeが生成されました\n");
	}
	~Hoge(){
		printf("Hogeが解放されました\n");
	}
	void piyoyo(HogeSP p){}
	void func(){
		HogeSP p = my_weak_ptr.lock();//WPからのSP
		if (NULL != p){
			piyoyo(p);
		}
	}
};


HogeSP Hoge::create(){
	HogeSP p = HogeSP(new Hoge());
	my_weak_ptr = p;//weak_ptrは参照カウンタが増えない
	return p;
}



void main(){
	HogeSP p1;

	{//スコープ範囲内
		HogeSP p2 = HogeSP(new Hoge());
		p1 = p2;
	}
	p1->_a = 10;


	{
		HogeSP p = Hoge::create();
	}




	getchar();
}