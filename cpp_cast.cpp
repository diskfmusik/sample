#include <stdio.h>

// static_cast
// dynamic_cast
// const_cast			const���O��
// reinterpret_cast		�|�C���^���m�̃L���X�g

class Base
{
	virtual void draw() {}
};

class Child : public Base
{
	virtual void draw() {}
};

void main()
{
	char c = 5;
	int n = static_cast<int>(c);

	Base *bp;
	Child *cp = new Child();
	bp = cp;
	Child *cp2 = dynamic_cast<Child*>(bp);

	getchar();
}