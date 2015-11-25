#include<stdio.h>


template<class T>
class Stack
{
	T* m_data;
	int m_index;
	int m_capacity;

public:
	Stack(int _n);
	~Stack();

	void Push(T _x);
	T Top()const;
	void Pop();
};


template<class T>
Stack<T>::Stack(int _n) : m_capacity(_n), m_index(0)
{
	m_data = new T[m_capacity];
}


template<class T>
Stack<T>::~Stack()
{
	delete[] m_data;
}


template<class T>
void Stack<T>::Push(T _x)
{
	if (m_index < m_capacity)
	{
		m_data[m_index++] = _x;
	}
}


template<class T>
T Stack<T>::Top()const
{
	return m_data[m_index - 1];
}


template<class T>
void Stack<T>::Pop()
{
	if (m_index > 0)
	{
		--m_index;
	}
}




void main()
{
	Stack<int> i_stack(5);

	i_stack.Push(1);
	i_stack.Push(2);
	i_stack.Push(3);
	i_stack.Push(4);
	i_stack.Push(5);

	printf("%d\n", i_stack.Top());

	i_stack.Pop();
	printf("%d\n", i_stack.Top());

	getchar();
}