// ...

enum SceneID
{
	Title,
	GameMain,
	Result,
};

SceneID g_sceneID = Title;// ������ : �^�C�g����ʂ���X�^�[�g


void timer()
{
	// ...

	switch (g_sceneID)
	{
	case Title:
		/* �^�C�g������ */
		break;

	case GameMain:
		/* �Q�[�����C������ */
		break;

	case Result:
		/* ���U���g���� */
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
		/* �^�C�g������ */
		break;

	case GameMain:
		/* �Q�[�����C������ */
		break;

	case Result:
		/* ���U���g���� */
		break;
	}

	// ...
}


int main()
{
	// ...

	return 0;
}
