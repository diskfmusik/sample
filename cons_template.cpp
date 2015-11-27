#include<stdio.h>

template<class T>
class Point
{
	template<class> friend class Point;

private:
	T m_x;
	T m_y;

public:
	Point(T _x = T(), T _y = T()) : m_x(_x), m_y(_y) {}

	// コンストラクタテンプレート
	template<class U>
	Point(const Point<U>& _r) : m_x(_r.m_x), m_y(_r.m_y) {}
};


void main()
{
	// クラステンプレート間の変換が出来るようになる
	Point<int>ip(12, 34);
	Point<long>lp(ip);

	getchar();
}