#include <stdio.h>


class DelegateBase {
public:
	DelegateBase() {}
	~DelegateBase() {}

	// �߂�l : void
	// ���� : void
	// �̊֐��̂悤�ɌĂׂ�悤�ɂ���
	virtual void operator()(void) = 0;
};


template < class T >
class Delegate : public DelegateBase {

public:
	Delegate() {}
	~Delegate() {}

	typedef void (T::*EventFunc)(void);


	// CreateDelegator �̈����Ŏw�肵���I�u�W�F�N�g�̊֐������s
	virtual void operator()(void) {
		(m_obj->*m_func)();
	}


	// �I�u�W�F�N�g �� ���\�b�h �̓o�^
	void Set(T* obj, EventFunc func) {
		m_obj = obj;
		m_func = func;
	}


	// �f���Q�[�^�����֐�
	// �e�̌^�Ŗ߂�
	// �����ł͓���̃N���X T �̃I�u�W�F�N�g �� ���\�b�h ��ێ����Ă���
	static DelegateBase* CreateDelegator(T* obj, void (T::*func)(void)) {
		Delegate* dg = new Delegate;
		dg->Set(obj, func);
		return dg;
	}


private:
	T* m_obj;               // �I�u�W�F�N�g
	EventFunc m_func;       // �֐��|�C���^
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


	// �f���Q�[�^�쐬
	DelegateBase* delegate_hoge = Delegate<Hoge>::CreateDelegator(&hoge, &Hoge::func);
	DelegateBase* delegate_piyo = Delegate<Piyo>::CreateDelegator(&piyo, &Piyo::func);


	// �e�N���X DelegateBase ������ ���Z�q() ��ʂ��ē���̃��\�b�h�����s���邱�Ƃ��o����
	(*delegate_hoge)();
	(*delegate_piyo)();


	getchar();
}