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

	// �R���X�g���N�^�e���v���[�g
	template<class U>
	Point(const Point<U>& _r) : m_x(_r.m_x), m_y(_r.m_y) {}
};


void main()
{
	// �N���X�e���v���[�g�Ԃ̕ϊ����o����悤�ɂȂ�
	Point<int>ip(12, 34);
	Point<long>lp(ip);

	getchar();
}