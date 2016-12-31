#include <stdio.h>

void fibonacci()
{
	int answer = 0;
	int prev2 = 0;
	int prev = 1;

	for (int i = 2; i <= 100; i++)
	{
		answer = prev2 + prev;
		printf("(%d):%d\n", i, answer);

		prev2 = prev;
		prev = answer;

	}

	printf("(i==100):%d\n", answer);

}

//void fib2(int left, int right, int count, int endCount)
void fib2(int endCount, int count = 2, int left = 0, int right = 1)
{
	if (count >= endCount) return;

	int next = left + right;
	printf("(%d):%d\n", count, next);

	//fib2(right, next, count + 1, endCount);
	fib2(endCount, count + 1, right, next);

}

int fib3(int n)
{
	if (n == 0) return 0;
	if (n == 1) return 1;

	return fib3(n - 2) + fib3(n - 1);

}

void main()
{

	fib2(10);

	printf("-----------------------------\n");

	for (int i = 2; i < 10; i++)
	{
		printf("(%d):%d\n", i, fib3(i));
	}

	getchar();
}