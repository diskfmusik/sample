#include <stdio.h>

template<int N>
struct increment
{
	static const int value = N + 1;
};


void main()
{
	// �e���v���[�g�̃C���X�^���X���𗘗p���āA
	// �R���p�C�����ɏ������s���N���X�e���v���[�g��
	// ���^�֐��Ƃ����B
	int n = increment<3>::value;

	getchar();
}