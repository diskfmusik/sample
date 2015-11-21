#include <stdio.h>
#include <string.h>


char *str = "abc def  ghi  jk(100,200);";


void main(){

	while (*str != NULL){

		/* スペース */
		if (*str == ' '){

			if (*(str + 1) == ' '){
				str++;
				continue;
			}
			printf("\n");
			str++;
			continue;
		}
		/* (スペース以外の)記号 */
		else if (*str == '(' || *str == ')' || *str == ',' || *str == ';'){

			if (*(str + 1) == '(' || *(str + 1) == ')' || *(str + 1) == ',' || *(str + 1) == ';'){
				//printf("\n%c", *str);
				printf("\n");
				putchar(*str);
				//次の記号で改行する
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
		/* 文字 */
		putchar(*str);
		str++;
	}

	getchar();
}