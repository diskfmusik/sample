#include<stdio.h>
#include<memory>//�X�}�[�g�|�C���^


//�X�}�[�g�|�C���^

class Hoge;//�N���X�̑O���錾
typedef std::shared_ptr<Hoge> HogeSP;
typedef std::weak_ptr<Hoge> HogeWP;

class Piyo;//�N���X�̑O���錾
typedef std::shared_ptr<Piyo> PiyoSP;
class Fuga;//�N���X�̑O���錾
typedef std::shared_ptr<Fuga> FugaSP;
class Foo;//�N���X�̑O���錾
typedef std::shared_ptr<Foo> FooSP;


class Piyo{
public:

	Piyo(){
		printf("Piyo����������܂���\n");
	}
	~Piyo(){
		printf("Piyo���������܂���\n");
	}
};
class Fuga{
public:

	Fuga(){
		printf("Fuga����������܂���\n");
	}
	~Fuga(){
		printf("Fuga���������܂���\n");
	}
};
class Foo{
public:

	Foo(){
		printf("Foo����������܂���\n");
	}
	~Foo(){
		printf("Foo���������܂���\n");
	}
};


class Hoge{
public:
	int _a;

	/*SP�ւ̋��n��*/
	HogeWP my_weak_ptr;//this�|�C���^�̑���
	PiyoSP piyo;
	FugaSP fuga;
	FooSP foo;

	static HogeSP create();

	Hoge(){
		piyo = PiyoSP(new Piyo());
		printf("Hoge����������܂���\n");
	}
	~Hoge(){
		printf("Hoge���������܂���\n");
	}
	void piyoyo(HogeSP p){}
	void func(){
		HogeSP p = my_weak_ptr.lock();//WP�����SP
		if (NULL != p){
			piyoyo(p);
		}
	}
};


HogeSP Hoge::create(){
	HogeSP p = HogeSP(new Hoge());
	my_weak_ptr = p;//weak_ptr�͎Q�ƃJ�E���^�������Ȃ�
	return p;
}



void main(){
	HogeSP p1;

	{//�X�R�[�v�͈͓�
		HogeSP p2 = HogeSP(new Hoge());
		p1 = p2;
	}
	p1->_a = 10;


	{
		HogeSP p = Hoge::create();
	}




	getchar();
}