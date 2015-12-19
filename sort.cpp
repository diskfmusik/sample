#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

void main(){
	//ソートプログラム
	int x = 0;
	int a[10] = { 95, 123, 62, 6, 0, 5, 22, 3, 9, 10 };
	for (int i = 0; i < 10; i++){
		for (int k = 1 + i; k < 10; k++){
			if (a[i] > a[k])continue;
			x = a[i];
			a[i] = a[k];
			a[k] = x;
		}
		printf("a[%d]=%d\n", i, a[i]);
	}
	_getch();
}