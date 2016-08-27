#include <stdio.h>
#include <time.h>

namespace mso
{

	template < class T >
	class Property
	{
	public:
		Property() { }
		Property(const T& var) { var_ = var; }
		~Property() { }

		inline const T& operator () () { return var_; } // getter

		inline void operator () (const T& var) { var_ = var; } // setter

	private:
		inline Property<T>& operator = (const T&) { } // 直接代入禁止

		T var_;
	};

} // namespace mso

class Object
{
public:
	Object() { isActive_(true); }
	~Object() { }

	mso::Property< bool > isActive_;

};

class Object_2
{
public:
	Object_2() { isActive_ = true; }
	~Object_2() { }

	bool IsActive()const { return isActive_; }

private:
	bool isActive_;

};

//#define process1

void main()
{
	Object obj;
	Object_2 obj_2;

	long start = clock();

#ifdef process1
	for (int i = 0; i < 1000000; i++) // 0.026 0.025 0.03
	{
		if (obj.isActive_()) {

		}
		else {

		}
	}
#else
	for (int i = 0; i < 1000000; i++)
	{
		//if (obj_2.isActive_) { // 0.002 0.003
		if (obj_2.IsActive()) { // 0.026

		}
		else {

		}
	}
#endif

	long end = clock();

	printf("sec : %f", (float)(end - start) / CLOCKS_PER_SEC);


	// public でセッター関数を作るのと、
	// templateプロパティクラスを使うのと、ほぼ同じ

	// まあ、データに直でアクセスする方が早いわな

	getchar();
}