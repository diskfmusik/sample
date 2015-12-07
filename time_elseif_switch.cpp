#include <stdio.h>
#include <Windows.h>
#include <time.h>


void main()
{
	long a = clock();

	/* else if */
#if 1
	for (int i = 0; i < 10000; i++)// 16052, 15935, 16072, 16012
	{
		a = rand() % 5;

		if (a == 1) {
			printf("%d\n", a);
		}
		else if (a == 2) {
			printf("%d\n", a);
		}
		else if (a == 3) {
			printf("%d\n", a);
		}
		else if (a == 4) {
			printf("%d\n", a);
		}
		else {
			printf("%d\n", a);
		}
	}
#endif

	/* switch */
#if 0
	for (int i = 0; i < 10000; i++)// 15999, 16317, 15979, 16015
	{
		a = rand() % 5;

		switch (a)
		{
		case 1:
			printf("%d\n", a);
			break;
		case 2:
			printf("%d\n", a);
			break;
		case 3:
			printf("%d\n", a);
			break;
		case 4:
			printf("%d\n", a);
			break;
		default:
			printf("%d\n", a);
			break;
		}
	}
#endif

	long b = clock();

	printf("sec : %lu", b - a);
}