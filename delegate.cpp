#include <stdio.h>


class DelegateBase {
public:
	DelegateBase() {}
	~DelegateBase() {}

	// 戻り値 : void
	// 引数 : void
	// の関数のように呼べるようにする
	virtual void operator()(void) = 0;
};


template < class T >
class Delegate : public DelegateBase {

public:
	Delegate() {}
	~Delegate() {}

	typedef void (T::*EventFunc)(void);


	// CreateDelegator の引数で指定したオブジェクトの関数を実行
	virtual void operator()(void) {
		(m_obj->*m_func)();
	}


	// オブジェクト と メソッド の登録
	void Set(T* obj, EventFunc func) {
		m_obj = obj;
		m_func = func;
	}


	// デリゲータ生成関数
	// 親の型で戻す
	// 内部では特定のクラス T のオブジェクト と メソッド を保持している
	static DelegateBase* CreateDelegator(T* obj, void (T::*func)(void)) {
		Delegate* dg = new Delegate;
		dg->Set(obj, func);
		return dg;
	}


private:
	T* m_obj;               // オブジェクト
	EventFunc m_func;       // 関数ポインタ
};


class Hoge
{
public:
	void func()
	{
		printf("Hoge_func\n");
	}
};


class Piyo
{
public:
	void func()
	{
		printf("Piyo_func\n");
	}
};


void main()
{
	Hoge hoge;
	Piyo piyo;


	// デリゲータ作成
	DelegateBase* delegate_hoge = Delegate<Hoge>::CreateDelegator(&hoge, &Hoge::func);
	DelegateBase* delegate_piyo = Delegate<Piyo>::CreateDelegator(&piyo, &Piyo::func);


	// 親クラス DelegateBase が持つ 演算子() を通して特定のメソッドを実行することが出来る
	(*delegate_hoge)();
	(*delegate_piyo)();


	getchar();
}