#include <stdio.h>
#include <string.h>


char *str = "abc def  ghi  jk(100,200);";


void main(){

	while (*str != NULL){

		/* �X�y�[�X */
		if (*str == ' '){

			if (*(str + 1) == ' '){
				str++;
				continue;
			}
			printf("\n");
			str++;
			continue;
		}
		/* (�X�y�[�X�ȊO��)�L�� */
		else if (*str == '(' || *str == ')' || *str == ',' || *str == ';'){

			if (*(str + 1) == '(' || *(str + 1) == ')' || *(str + 1) == ',' || *(str + 1) == ';'){
				//printf("\n%c", *str);
				printf("\n");
				putchar(*str);
				//���̋L���ŉ��s����
				str++;
				continue;
			}
			//printf("\n%c\n", *str);
			printf("\n");
			putchar(*str);
			printf("\n");
			str++;
			continue;
		}
		/* ���� */
		putchar(*str);
		str++;
	}

	getchar();
}