#include <stdio.h>
#include <vector>


//int�^�ϐ��������Ƃ��Ď��A
//�����œn���ꂽ�l�̌��̐��l��\������
//�֐����쐬���Ȃ����B


void func(const int _a){
	int i;
	int temp = _a;
	std::vector<int> a;


	while (1){
		int s = temp % 10;
		if (s == 0)break;

		a.push_back(s);
		temp = temp / 10;
	}

	for (i = a.size() - 1; i >= 0; i--){
		printf("%d\n", a[i]);
	}
}


void main(){
	int a = 123456;

	func(a);

	getchar();
}