#pragma comment ( lib, "OpenAL32.lib" )

#include "vorbisfile.h"
#include "al.h"
#include "alc.h"
#include <Windows.h>

typedef unsigned int uint;

//---------------------------------------------------------
// var

ALCdevice *device_;
ALCcontext *context_;

const int BufSize = 32;
uint source_;
uint buffers_[BufSize];
int format_;

//---------------------------------------------------------
// func

void al_init();
void ogg_play(const char* fileName);


int main()
{
	al_init();
	ogg_play("test.ogg");

	return 0;
}


void al_init()
{
	// デバイスをオープンし、コンテキストを取得する
	device_ = alcOpenDevice(NULL);

	// デバイスから、コンテキストを作成する
	context_ = alcCreateContext(device_, NULL);

	// 現在のコンテキストを設定する
	alcMakeContextCurrent(context_);

	ALenum error = alGetError();
	const ALchar *errorString = alGetString(error);
	printf("コンテキスト:%s\n", errorString);
}


void ogg_play(const char* fileName)
{
	OggVorbis_File vf;

	if (ov_fopen(fileName, &vf) != 0)
		return;

	// Oggファイルの音声フォーマット情報
	vorbis_info* info = ov_info(&vf, -1);

	if (info->channels == 1)
	{
		format_ = AL_FORMAT_MONO16;
	}
	else /*if (info->channels == 2)*/
	{
		format_ = AL_FORMAT_STEREO16;
	}


	alGenBuffers(BufSize, buffers_);

	alGenSources(1, &source_);

	for (int i = 0; i < BufSize; i++)
	{
		char data[4096];
		long readSize = ov_read(&vf, data, sizeof(data), 0, 2, 1, 0);
		printf("readSize:%d\n", readSize);

		alBufferData(buffers_[i], format_, data, readSize, info->rate);
		alSourceQueueBuffers(source_, 1, &buffers_[i]);
	}


	alSourcePlay(source_);

	uint totalBuffersProcessed = 0;


	// buffers update
	while (1)
	{
		unsigned long msPerFrame = 16.67;

		// 毎フレーム監視の頻度でよろしいのだろうか
		// でも、 buffer はこれ以上増やしたくはないし
		Sleep(20);


		// 再生し終わった buffer の数
		int endPlayingBufferNum = 0;
		alGetSourcei(source_, AL_BUFFERS_PROCESSED, &endPlayingBufferNum);

#if 0
		totalBuffersProcessed += endPlayingBufferNum;
		printf("Buffers Processed:%d\n", totalBuffersProcessed);
#endif

		while (endPlayingBufferNum)
		{
			uint uiBuffer = 0;
			alSourceUnqueueBuffers(source_, 1, &uiBuffer);


			char data[4096];
			long readSize = ov_read(&vf, data, sizeof(data), 0, 2, 1, 0);
			//printf("readSize:%d\n", readSize);

			if (readSize == 0)
			{
				// ファイルエンドに達した
				// ループする場合読み込み位置を最初に戻す
				ov_time_seek(&vf, 0.0);

				// もう一度、読み直す
				readSize = ov_read(&vf, data, sizeof(data), 0, 2, 1, 0);
			}

			alBufferData(uiBuffer, format_, data, readSize, info->rate);
			alSourceQueueBuffers(source_, 1, &uiBuffer);


			--endPlayingBufferNum;
		}

		//int state;
		//alGetSourcei(source_, AL_SOURCE_STATE, &state);
		//if (state != AL_PLAYING)
		//{
		//	break;

		//	//int iQueuedBuffers;

		//	//// If there are Buffers in the Source Queue then the Source was starved of audio
		//	//// data, so needs to be restarted (because there is more audio data to play)
		//	//alGetSourcei(source_, AL_BUFFERS_QUEUED, &iQueuedBuffers);
		//	//if (iQueuedBuffers)
		//	//{
		//	//	alSourcePlay(source_);
		//	//}
		//	//else
		//	//{
		//	//	// Finished playing
		//	//	break;
		//	//}
		//}

	} // while (1)


	ov_clear(&vf);

	alSourceStop(source_);
	alSourcei(source_, AL_BUFFER, 0);

	alDeleteSources(1, &source_);
	alDeleteBuffers(BufSize, buffers_);

	alcDestroyContext(context_);
	alcCloseDevice(device_);

} // void ogg_play()
