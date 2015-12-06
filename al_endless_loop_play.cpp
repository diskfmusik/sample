#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <assert.h>
#include "glut.h"

#include "AL/al.h"
#include "AL/alc.h"
#pragma comment (lib,"OpenAL32.lib")


ALuint g_bid;
ALuint g_source;


struct WavFileHeader
{
	char m_fileID[4];	// ファイル識別子 : 'R','I','F','F'
	int m_fileSize;		// ファイルのサイズ : ファイルサイズ - 8(ファイルの先頭の 8 バイトを除く)
	char m_riffID[4];	// RIFF識別子 : 'W', 'A', 'V', 'E'
};

struct WavFmtHeader
{
	char m_chunkID[4];				// fmtチャンク識別子 : 'f', 'm', 't', ' ' (最後はスペース)
	int m_chunkSize;				// fmtチャンクのサイズ : 16(先頭の 8 バイトを除く)
	unsigned short m_formatType;	// フォーマットの種類 : 1(PCM)
	unsigned short m_channelType;	// チャンネル数 : 1(モノラル) or 2(ステレオ)
	int m_frequency;				// 周波数 : 音楽CDは 44100Hz(44.1kHz)
	int m_dataSize_perSec;			// 1秒あたりのデータサイズ : 周波数 * ブロックサイズ
	unsigned short m_blockSize;		// ブロックサイズ : チャンネル数 * ビット数 / 8
	unsigned short m_bitType;		// ビット数 : 8 ビット or 16 ビット
};

struct WavDataHeader
{
	char m_chunkID[4];	// dataチャンク識別子 : 'd', 'a', 't', 'a'
	int m_dataSize;		// dataチャンクのサイズ : dataチャンクのサイズ - 8(先頭の 8 バイトを除く)。波形データのサイズ。
};


WavFileHeader wavFileHeader;
WavFmtHeader wavFmtHeader;
WavDataHeader wavDataHeader;


void wav_load(const char *_fileName)
{
	FILE *fp;

	fopen_s(&fp, _fileName, "rb");

	if (fp == NULL)
	{
		printf("mapが　ありません！\n");
		assert(NULL);
	}

	fread(&wavFileHeader, sizeof(WavFileHeader), 1, fp);
	printf("%s\n", wavFileHeader.m_fileID);
	printf("%d\n", wavFileHeader.m_fileSize);
	printf("%s\n", wavFileHeader.m_riffID);

	fread(&wavFmtHeader, sizeof(WavFmtHeader), 1, fp);
	printf("fmtチャンクのサイズ:%d\n", wavFmtHeader.m_chunkSize);
	printf("フォーマットの種類:%hu\n", wavFmtHeader.m_formatType);
	printf("チャンネル数:%hu\n", wavFmtHeader.m_channelType);
	printf("周波数:%d\n", wavFmtHeader.m_frequency);
	printf("1秒あたりのデータサイズ:%d\n", wavFmtHeader.m_dataSize_perSec);
	printf("ブロックサイズ:%hu\n", wavFmtHeader.m_blockSize);
	printf("ビット数:%hu\n", wavFmtHeader.m_bitType);

	fread(&wavDataHeader, sizeof(WavDataHeader), 1, fp);
	printf("%d\n", wavDataHeader.m_dataSize);

	// 波形データ読み込み
	ALsizei size = wavDataHeader.m_dataSize * sizeof(short);

	short *wav_data = (short*)malloc(size);

	fread(wav_data, size, 1, fp);

	fclose(fp);

	alBufferData(
		g_bid,				// ALuint bid
		AL_FORMAT_STEREO16,	// ALenum format
		wav_data,			// const ALvoid* data
		size,				// ALsizei size
		wavFmtHeader.m_frequency);   // ALsizei freq

	free(wav_data);

	ALenum error = alGetError();
	const ALchar *errorString = alGetString(error);	// ALenum param
	printf("バッファ:%s\n", errorString);
}


void al_set_device()
{
	// デバイスをオープンし、コンテキストを取得する
	ALCdevice *device = alcOpenDevice(NULL);

	// デバイスから、コンテキストを作成する
	ALCcontext *context = alcCreateContext(
		device, // ALCdevice *device
		NULL);  // const ALCint* attrlist

	// 現在のコンテキストを設定する
	alcMakeContextCurrent(context);// ALCcontext *context

	ALenum error = alGetError();
	const ALchar *errorString = alGetString(error);	// ALenum param
	printf("コンテキスト:%s\n", errorString);
}


// 現在の再生位置を「秒」単位で取得する
const float getOffset(ALuint &_source)
{
	float offset;

	alGetSourcef(_source, AL_SEC_OFFSET, &offset);

	return offset;
}


void al_offset_reset()
{
	// 01.07.419
	const float shouldResetPos = 67.419f;// 秒に直した

	if (getOffset(g_source) >= shouldResetPos)
	{
		alSourceStop(g_source);

		/* 再生位置の設定 */
		const float initPos = 22.622f;
		alSourceRewind(g_source);
		alSourcef(g_source, AL_SEC_OFFSET, initPos);

		/* また再生させる */
		alSourcei(g_source, AL_LOOPING, AL_TRUE);
		alSourcePlay(g_source);
	}
}


void timer(int value)
{
	glutPostRedisplay();

	al_offset_reset();

	glutTimerFunc(1000 / 90, timer, 0);
}


void display(void)
{
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		90, // GLdouble fovy
		1,  // GLdouble aspect
		.1, // GLdouble zNear
		10);// GLdouble zFar

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 0, 2,  // eye
		0, 0, 0,  // center
		0, 1, 0); // up

	glFlush();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("無限ループ再生");

	al_set_device();

	alGenBuffers(1, &g_bid);

	alGenSources(1, &g_source);

	wav_load("test.wav");

	alSourcei(g_source, AL_BUFFER, g_bid);// set_source
	alSourcei(g_source, AL_LOOPING, AL_TRUE);
	alSourcePlay(g_source);


	glutDisplayFunc(display);
	glutIgnoreKeyRepeat(GL_TRUE);

	glutTimerFunc(0, timer, 0);

	glutMainLoop();
}