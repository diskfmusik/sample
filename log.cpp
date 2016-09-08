#include "DxLib.h"
#include <memory>
#include <queue>

#include "mso_library/KeyBoard.h"
#include "mso_library/Color.h"
#include "mso_library/Font.h"
#include "mso_library/FpsController.h"
#include "game/sceneManager/SceneManager.h"


void system_init()
{
	SetOutApplicationLogValidFlag(FALSE);

	SetGraphMode(640, 480, 16);
	ChangeWindowMode(true);
	SetWindowSizeExtendRate(1.5);

	SetMainWindowText("log_test");
}


void game_init()
{
	mso::Font::Init();
	mso::Color::Init();
	mso::FpsController::Init(60);
}


const bool ProcessLoop()
{
	if (ProcessMessage() != 0) return false; // �v���V�[�W�����G���[�Ȃ�
	if (ClearDrawScreen() != 0) return false; // ��ʃN���A�������G���[�Ȃ�
	return true;
}


class Log;
typedef std::shared_ptr< Log > LogSP;

class Log
{
public:
	Log(const char* str) :
		str_(str),
		strSize_(strlen(str))
	{}

	int getStrSize()const { return strSize_; }

	void print(int viewStrSize)const
	{
		char s[100];
		strncpy_s(s, str_, viewStrSize);
		DrawString(100, 200, s, GetColor(255, 255, 255));
	}

private:
	const char* str_;
	const int strSize_;
};


class LogManager
{
public:
	LogManager(int logSpd) :
		viewStrSize_(0),
		logSpd_(logSpd)
	{}

	void push(const LogSP& log)
	{
		queue_.push(log);
	}

	bool isEmpty()
	{
		return queue_.empty();
	}

	void process()
	{
		if (isEmpty()) return;

		static int time_ = 0;

		auto log = queue_.front();
		if (time_ % logSpd_ == 0)
		{
			if (viewStrSize_ < log->getStrSize())
				++viewStrSize_;
		}

		log->print(viewStrSize_);

		if (mso::KeyBoard::GetPressMoment(KEY_INPUT_RETURN))
		{
			queue_.pop();
			viewStrSize_ = 0;
			time_ = 0;
		}
		++time_;
	}

private:
	LogManager() {}

	int logSpd_; // ��frame���� log ��i�߂Ă�����
	int viewStrSize_;
	std::queue< LogSP > queue_;
};


LogManager manager(10);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	system_init();
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0)return -1; // �������Ɨ���ʉ�

	game_init();


	LogSP log1 = LogSP(new Log("����������"));
	LogSP log2 = LogSP(new Log("����������"));
	LogSP log3 = LogSP(new Log("����������"));
	manager.push(log1);
	manager.push(log2);
	manager.push(log3);


	while (ProcessLoop())
	{
		mso::KeyBoard::SetNowState();
	
		manager.process();

		mso::FpsController::Wait();
		mso::FpsController::Draw();

		ScreenFlip(); // ����ʔ��f

		mso::KeyBoard::KeepPrevState();
	}


	DxLib_End();
	return 0;
}