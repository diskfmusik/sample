// ...

enum SceneID
{
	Title,
	GameMain,
	Result,
};

SceneID g_sceneID = Title;// 初期化 : タイトル画面からスタート


void timer()
{
	// ...

	switch (g_sceneID)
	{
	case Title:
		/* タイトル処理 */
		break;

	case GameMain:
		/* ゲームメイン処理 */
		break;

	case Result:
		/* リザルト処理 */
		break;
	}

	// ...
}


void display()
{
	// ...

	switch (g_sceneID)
	{
	case Title:
		/* タイトル処理 */
		break;

	case GameMain:
		/* ゲームメイン処理 */
		break;

	case Result:
		/* リザルト処理 */
		break;
	}

	// ...
}


int main()
{
	// ...

	return 0;
}
