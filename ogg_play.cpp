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
	// �f�o�C�X���I�[�v�����A�R���e�L�X�g���擾����
	device_ = alcOpenDevice(NULL);

	// �f�o�C�X����A�R���e�L�X�g���쐬����
	context_ = alcCreateContext(device_, NULL);

	// ���݂̃R���e�L�X�g��ݒ肷��
	alcMakeContextCurrent(context_);

	ALenum error = alGetError();
	const ALchar *errorString = alGetString(error);
	printf("�R���e�L�X�g:%s\n", errorString);
}


void ogg_play(const char* fileName)
{
	OggVorbis_File vf;

	if (ov_fopen(fileName, &vf) != 0)
		return;

	// Ogg�t�@�C���̉����t�H�[�}�b�g���
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

		// ���t���[���Ď��̕p�x�ł�낵���̂��낤��
		// �ł��A buffer �͂���ȏ㑝�₵�����͂Ȃ���
		Sleep(20);


		// �Đ����I����� buffer �̐�
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
				// �t�@�C���G���h�ɒB����
				// ���[�v����ꍇ�ǂݍ��݈ʒu���ŏ��ɖ߂�
				ov_time_seek(&vf, 0.0);

				// ������x�A�ǂݒ���
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
