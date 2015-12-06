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
	char m_fileID[4];	// �t�@�C�����ʎq : 'R','I','F','F'
	int m_fileSize;		// �t�@�C���̃T�C�Y : �t�@�C���T�C�Y - 8(�t�@�C���̐擪�� 8 �o�C�g������)
	char m_riffID[4];	// RIFF���ʎq : 'W', 'A', 'V', 'E'
};

struct WavFmtHeader
{
	char m_chunkID[4];				// fmt�`�����N���ʎq : 'f', 'm', 't', ' ' (�Ō�̓X�y�[�X)
	int m_chunkSize;				// fmt�`�����N�̃T�C�Y : 16(�擪�� 8 �o�C�g������)
	unsigned short m_formatType;	// �t�H�[�}�b�g�̎�� : 1(PCM)
	unsigned short m_channelType;	// �`�����l���� : 1(���m����) or 2(�X�e���I)
	int m_frequency;				// ���g�� : ���yCD�� 44100Hz(44.1kHz)
	int m_dataSize_perSec;			// 1�b������̃f�[�^�T�C�Y : ���g�� * �u���b�N�T�C�Y
	unsigned short m_blockSize;		// �u���b�N�T�C�Y : �`�����l���� * �r�b�g�� / 8
	unsigned short m_bitType;		// �r�b�g�� : 8 �r�b�g or 16 �r�b�g
};

struct WavDataHeader
{
	char m_chunkID[4];	// data�`�����N���ʎq : 'd', 'a', 't', 'a'
	int m_dataSize;		// data�`�����N�̃T�C�Y : data�`�����N�̃T�C�Y - 8(�擪�� 8 �o�C�g������)�B�g�`�f�[�^�̃T�C�Y�B
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
		printf("map���@����܂���I\n");
		assert(NULL);
	}

	fread(&wavFileHeader, sizeof(WavFileHeader), 1, fp);
	printf("%s\n", wavFileHeader.m_fileID);
	printf("%d\n", wavFileHeader.m_fileSize);
	printf("%s\n", wavFileHeader.m_riffID);

	fread(&wavFmtHeader, sizeof(WavFmtHeader), 1, fp);
	printf("fmt�`�����N�̃T�C�Y:%d\n", wavFmtHeader.m_chunkSize);
	printf("�t�H�[�}�b�g�̎��:%hu\n", wavFmtHeader.m_formatType);
	printf("�`�����l����:%hu\n", wavFmtHeader.m_channelType);
	printf("���g��:%d\n", wavFmtHeader.m_frequency);
	printf("1�b������̃f�[�^�T�C�Y:%d\n", wavFmtHeader.m_dataSize_perSec);
	printf("�u���b�N�T�C�Y:%hu\n", wavFmtHeader.m_blockSize);
	printf("�r�b�g��:%hu\n", wavFmtHeader.m_bitType);

	fread(&wavDataHeader, sizeof(WavDataHeader), 1, fp);
	printf("%d\n", wavDataHeader.m_dataSize);

	// �g�`�f�[�^�ǂݍ���
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
	printf("�o�b�t�@:%s\n", errorString);
}


void al_set_device()
{
	// �f�o�C�X���I�[�v�����A�R���e�L�X�g���擾����
	ALCdevice *device = alcOpenDevice(NULL);

	// �f�o�C�X����A�R���e�L�X�g���쐬����
	ALCcontext *context = alcCreateContext(
		device, // ALCdevice *device
		NULL);  // const ALCint* attrlist

	// ���݂̃R���e�L�X�g��ݒ肷��
	alcMakeContextCurrent(context);// ALCcontext *context

	ALenum error = alGetError();
	const ALchar *errorString = alGetString(error);	// ALenum param
	printf("�R���e�L�X�g:%s\n", errorString);
}


// ���݂̍Đ��ʒu���u�b�v�P�ʂŎ擾����
const float getOffset(ALuint &_source)
{
	float offset;

	alGetSourcef(_source, AL_SEC_OFFSET, &offset);

	return offset;
}


void al_offset_reset()
{
	// 01.07.419
	const float shouldResetPos = 67.419f;// �b�ɒ�����

	if (getOffset(g_source) >= shouldResetPos)
	{
		alSourceStop(g_source);

		/* �Đ��ʒu�̐ݒ� */
		const float initPos = 22.622f;
		alSourceRewind(g_source);
		alSourcef(g_source, AL_SEC_OFFSET, initPos);

		/* �܂��Đ������� */
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
	glutCreateWindow("�������[�v�Đ�");

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