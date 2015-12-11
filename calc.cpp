#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

// トークンの種類
enum TOK_TYPE
{
	TOK_NUM,	// 数字
	TOK_PLUS,	// operator +
	TOK_MINUS,	// operator -
	TOK_MUL,	// operator *
	TOK_INVALID,// 予期せぬトークン
};

#define BUF_SIZE (16)

TOK_TYPE get_num(FILE *_fp, int *_num)
{
	int c;
	char buf[BUF_SIZE];
	int i = 0;

	while ((c = fgetc(_fp)) != '\n')
	{
		buf[i++] = c;

		if (!isdigit(c))
		{
			return TOK_INVALID;
		}
	}
	buf[i] = '\0';

	*_num = strtol(buf, NULL, 10);

	return TOK_NUM;
}

TOK_TYPE get_tok(int _c)
{

	if (_c == '+')
	{
		printf("+\n");
		return TOK_PLUS;
	}
	else if (_c == '-')
	{
		printf("-\n");
		return TOK_MINUS;
	}
	else if (_c == '*')
	{
		printf("*\n");
		return TOK_MUL;
	}

	return TOK_INVALID;
}

void main()
{
	printf("input first num  : ");
	int a;
	if (get_num(stdin, &a) == TOK_INVALID)
	{
		printf("invalid num\n");
		return;
	}


	printf("input operator   : ");
	TOK_TYPE op;
	op = get_tok(_getch());

	if (op == TOK_INVALID)
	{
		printf("invalid operator\n");
		return;
	}


	printf("input second num : ");
	int b;
	if (get_num(stdin, &b) == TOK_INVALID)
	{
		printf("invalid num\n");
		return;
	}


	if (op == TOK_PLUS)
	{
		printf("first + second   = %d\n", a + b);
	}
	else if (op == TOK_MINUS)
	{
		printf("first - second   = %d\n", a - b);
	}
	else if (op == TOK_MUL)
	{
		printf("first * second   = %d\n", a * b);
	}
}