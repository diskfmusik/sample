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

	printf("�������𒲂ׂ����t�@�C�����h���b�O���h���b�v���ĉ������B\n");
	printf("--���ׂ����ӏ����w$�x�ň͂��ĉ������B--\n");


	fscanf(stdin, "%s", fname);
#if 0
	printf("%s\n", fname);
#endif
	FILE *fp = fopen(fname, "rb");

	if (fp == NULL)
	{
		printf("�t�@�C��������܂���B");
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


	/* �ŏ��� $ �}�[�N */
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

	/* �Ō�� $ �}�[�N */
	while (*str != /*'\0'*/ '$')
	{
		int byte = mblen(str, MB_CUR_MAX);
#if 0
		printf("byte:%d\n", byte);
#endif
		/* �֌W�Ȃ��������������΂� */
		if (*str == ' ' || *str == '�@' || *str == '/' || *str == '*' || *str == '\n')
		{
			*str++;
			if (byte == 2)*str++; // 2�o�C�g�����Ȃ�A������i�߂�
			continue;
		}

		/* ������������J�E���^�𑝂₷ */
		length++;

		/* ���̕��������ɍs�� */
		*str++;
		if (byte == 2)*str++; // 2�o�C�g�����Ȃ�A������i�߂�
	}
	printf("������ : %d\n", length);

	printf("\n�I�� : q");
	while (_getch() != 'q');
	getchar();
}
