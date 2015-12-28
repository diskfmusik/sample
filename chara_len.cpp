#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


void main()
{
	setlocale(LC_CTYPE, "jpn");

	char fname[100];
	char *str;
	int length = 0;

	printf("文字数を調べたいファイルをドラッグ＆ドロップして下さい。\n");
	printf("--調べたい箇所を『$』で囲って下さい。--\n");


	fscanf(stdin, "%s", fname);
#if 0
	printf("%s\n", fname);
#endif
	FILE *fp = fopen(fname, "rb");

	if (fp == NULL)
	{
		printf("ファイルがありません。");
		return;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
#if 0
	printf("size:%d\n", size);
#endif
	str = (char*)malloc(size);
	fseek(fp, 0, SEEK_SET);
	//fscanf(fp, "%s", str);
	fread(str, size, 1, fp);
#if 0
	printf("%s\n", str);
#endif
	fclose(fp);


	/* 最初の $ マーク */
	while (1)
	{
		if (*str == '$')
		{
			*str++;
			break;
		}
		else
		{
			*str++;
		}
	}

	/* 最後の $ マーク */
	while (*str != /*'\0'*/ '$')
	{
		int byte = mblen(str, MB_CUR_MAX);
#if 0
		printf("byte:%d\n", byte);
#endif
		/* 関係ない文字だったら飛ばす */
		if (*str == ' ' || *str == '　' || *str == '/' || *str == '*' || *str == '\n')
		{
			*str++;
			if (byte == 2)*str++; // 2バイト文字なら、もう一つ進める
			continue;
		}

		/* 文字だったらカウンタを増やす */
		length++;

		/* 次の文字を見に行く */
		*str++;
		if (byte == 2)*str++; // 2バイト文字なら、もう一つ進める
	}
	printf("文字数 : %d\n", length);

	printf("\n終了 : q");
	while (_getch() != 'q');
	getchar();
}
