#include <stdio.h>
#include <vector>


//int型変数を引数として取り、
//引数で渡された値の桁の数値を表示する
//関数を作成しなさい。


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