#include "DxLib.h"

#define WIDTH (10)
#define HEIGHT (10)

int mouseX, mouseY;


int map_cost[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -2, -1, -1, -1, -1, -1, -2, -2, -1,
	-1, -2, -1, -1, -1, -1, -1, -1, -2, -1,
	-1, -1, -1, -1, -2, -2, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -2, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -2, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -2, -1, -1, -1, -1, -2, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -2, -1,
};

int player_access[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};


void access_init(){
	int i;
	for (i = 0; i < WIDTH * HEIGHT; i++){
		player_access[i] = -1;
	}
}


void draw_map(){
	int x, y;

	for (y = 0; y < HEIGHT; y++){
		for (x = 0; x < WIDTH; x++){

			/* map */
			switch (map_cost[y * WIDTH + x]){
			case -1:// ���n
				DrawBox(0 + x * 40, 0 + y * 40, 40 + x * 40, 40 + y * 40, GetColor(255, 255, 255), FALSE);
				break;
			case -2:// �X
				DrawBox(0 + x * 40, 0 + y * 40, 40 + x * 40, 40 + y * 40, GetColor(0, 255, 0), TRUE);
				break;
			}


			/* �ړ��͈͂̐��l */
			if (player_access[y * WIDTH + x] == -1)
				DrawFormatString(20 + x * 40, 20 + y * 40, GetColor(255, 255, 255), "%d", player_access[y * WIDTH + x]);
			else
				DrawFormatString(20 + x * 40, 20 + y * 40, GetColor(0, 0, 255), "%d", player_access[y * WIDTH + x]);

		}
	}
}


// �ړ��悪�ړ��\�ȏꏊ�ł��邩�𔻒肷��֐�
void access_judge(int _moveRange, const int _cx, const int _cy){
	int coord = (_cy / 40) * 10 + (_cx / 40);

	if (player_access[coord] < _moveRange + map_cost[coord])
		player_access[coord] = _moveRange + map_cost[coord];
}


void route_search(int _moveRange, const int _cx, const int _cy){
	if (_cx < 40 || _cy < 40 || _cx > 360 || _cy > 360)return;


	/* ��������W�̎��� */
	int upCoord = ((_cy - 40) / 40) * 10 + (_cx / 40);
	int downCoord = ((_cy + 40) / 40) * 10 + (_cx / 40);
	int leftCoord = (_cy / 40) * 10 + ((_cx - 40) / 40);
	int rightCoord = (_cy / 40) * 10 + ((_cx + 40) / 40);

	// ��
	access_judge(_moveRange, _cx, _cy - 40);
	// ��
	access_judge(_moveRange, _cx, _cy + 40);
	// ��
	access_judge(_moveRange, _cx - 40, _cy);
	// �E
	access_judge(_moveRange, _cx + 40, _cy);


	if (_moveRange < 1){
		return;
	}
	else{
		// ��
		route_search(_moveRange + map_cost[upCoord], _cx, _cy - 40);
		// ��
		route_search(_moveRange + map_cost[downCoord], _cx, _cy + 40);
		// ��
		route_search(_moveRange + map_cost[leftCoord], _cx - 40, _cy);
		// �E
		route_search(_moveRange + map_cost[rightCoord], _cx + 40, _cy);
	}

}


//���[�v�ŕK���s���R�又��
bool ProcessLoop(){
	if (ProcessMessage() != 0)return false;//�v���V�[�W�����G���[�Ȃ�-1��Ԃ�
	if (ClearDrawScreen() != 0)return false;//��ʃN���A�������G���[�Ȃ�-1��Ԃ�
	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	SetOutApplicationLogValidFlag(FALSE);

	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);
	SetWindowSizeExtendRate(1.5);

	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0)return -1;// �������Ɨ���ʉ�

	SetMouseDispFlag(TRUE);// �}�E�X��\����Ԃɂ���


	while (ProcessLoop()){

		GetMousePoint(&mouseX, &mouseY);// �}�E�X�̈ʒu���擾


		const int Move = 3;

		access_init();
		player_access[(mouseY / 40) * 10 + (mouseX / 40)] = Move;
		route_search(Move, mouseX, mouseY);
		draw_map();


		ScreenFlip();// ����ʔ��f
	}


	DxLib_End();
	return 0;
}