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
			case -1:// 平地
				DrawBox(0 + x * 40, 0 + y * 40, 40 + x * 40, 40 + y * 40, GetColor(255, 255, 255), FALSE);
				break;
			case -2:// 森
				DrawBox(0 + x * 40, 0 + y * 40, 40 + x * 40, 40 + y * 40, GetColor(0, 255, 0), TRUE);
				break;
			}


			/* 移動範囲の数値 */
			if (player_access[y * WIDTH + x] == -1)
				DrawFormatString(20 + x * 40, 20 + y * 40, GetColor(255, 255, 255), "%d", player_access[y * WIDTH + x]);
			else
				DrawFormatString(20 + x * 40, 20 + y * 40, GetColor(0, 0, 255), "%d", player_access[y * WIDTH + x]);

		}
	}
}


// 移動先が移動可能な場所であるかを判定する関数
void access_judge(int _moveRange, const int _cx, const int _cy){
	int coord = (_cy / 40) * 10 + (_cx / 40);

	if (player_access[coord] < _moveRange + map_cost[coord])
		player_access[coord] = _moveRange + map_cost[coord];
}


void route_search(int _moveRange, const int _cx, const int _cy){
	if (_cx < 40 || _cy < 40 || _cx > 360 || _cy > 360)return;


	/* 今いる座標の周り */
	int upCoord = ((_cy - 40) / 40) * 10 + (_cx / 40);
	int downCoord = ((_cy + 40) / 40) * 10 + (_cx / 40);
	int leftCoord = (_cy / 40) * 10 + ((_cx - 40) / 40);
	int rightCoord = (_cy / 40) * 10 + ((_cx + 40) / 40);

	// 上
	access_judge(_moveRange, _cx, _cy - 40);
	// 下
	access_judge(_moveRange, _cx, _cy + 40);
	// 左
	access_judge(_moveRange, _cx - 40, _cy);
	// 右
	access_judge(_moveRange, _cx + 40, _cy);


	if (_moveRange < 1){
		return;
	}
	else{
		// 上
		route_search(_moveRange + map_cost[upCoord], _cx, _cy - 40);
		// 下
		route_search(_moveRange + map_cost[downCoord], _cx, _cy + 40);
		// 左
		route_search(_moveRange + map_cost[leftCoord], _cx - 40, _cy);
		// 右
		route_search(_moveRange + map_cost[rightCoord], _cx + 40, _cy);
	}

}


//ループで必ず行う３大処理
bool ProcessLoop(){
	if (ProcessMessage() != 0)return false;//プロシージャがエラーなら-1を返す
	if (ClearDrawScreen() != 0)return false;//画面クリア処理がエラーなら-1を返す
	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	SetOutApplicationLogValidFlag(FALSE);

	SetGraphMode(640, 480, 16);
	ChangeWindowMode(TRUE);
	SetWindowSizeExtendRate(1.5);

	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0)return -1;// 初期化と裏画面化

	SetMouseDispFlag(TRUE);// マウスを表示状態にする


	while (ProcessLoop()){

		GetMousePoint(&mouseX, &mouseY);// マウスの位置を取得


		const int Move = 3;

		access_init();
		player_access[(mouseY / 40) * 10 + (mouseX / 40)] = Move;
		route_search(Move, mouseX, mouseY);
		draw_map();


		ScreenFlip();// 裏画面反映
	}


	DxLib_End();
	return 0;
}