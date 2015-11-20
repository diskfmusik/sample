#include <time.h>
#include <vector>
#include "glut.h"
#include "Vec3.h"


int g_windowWidth = 960;
int g_windowHeight = 720;

#define F_WIDTH (15)
#define F_HEIGHT (15)

std::vector<Vec3> f_vertex;
std::vector<unsigned short>f_index;
std::vector<Vec3> f_normal;


// 0 ~ 1.0f ��Ԃ�
float f_rand() {
	int i = rand() % 101;
	return i / 100.0f;
}


void set_vertex() {
	int x, z;

	for (z = 0; z < F_HEIGHT; z++) {
		for (x = 0; x < F_WIDTH; x++) {
			f_vertex.push_back(Vec3(-1.0f + 2.0f / (F_WIDTH - 1) * x, 0, -1.0f + 2.0f / (F_HEIGHT - 1) * z));
		}
	}
}


void set_index() {
	int x, z;

	for (z = 0; z < (F_HEIGHT - 1); z++) {
		for (x = 0; x < (F_WIDTH - 1); x++) {
			f_index.push_back(0 + x + F_HEIGHT * z);
			f_index.push_back(F_WIDTH + x + F_HEIGHT * z);
			f_index.push_back(1 + x + F_HEIGHT * z);

			f_index.push_back(1 + x + F_HEIGHT * z);
			f_index.push_back(F_WIDTH + x + F_HEIGHT * z);
			f_index.push_back((F_WIDTH + 1) + x + F_HEIGHT * z);
		}
	}
}


// ���_�ψʖ@
void set_y(int _size, const int _leftTop, const int _leftLow, const int _rightTop, const int _rightLow) {

	// ���ԍ��W
	int upCoord = (_leftTop + _rightTop) * 0.5;
	int downCoord = (_leftLow + _rightLow) * 0.5;
	int leftCoord = (_leftLow + _leftTop) * 0.5;
	int rightCoord = (_rightLow + _rightTop) * 0.5;
	int centerCoord = (_leftTop + _leftLow + _rightTop + _rightLow) * 0.25;

	// 4���݂�y���Wrand
	f_vertex[_leftTop].y = f_rand() * _size;
	f_vertex[_leftLow].y = f_rand() * _size;
	f_vertex[_rightTop].y = f_rand() * _size;
	f_vertex[_rightLow].y = f_rand() * _size;

	// 4���݂̒���
	f_vertex[upCoord].y = (f_vertex[_leftTop].y + f_vertex[_rightTop].y) * 0.5;
	f_vertex[downCoord].y = (f_vertex[_leftLow].y + f_vertex[_rightLow].y) * 0.5;
	f_vertex[leftCoord].y = (f_vertex[_leftLow].y + f_vertex[_leftTop].y) * 0.5;
	f_vertex[rightCoord].y = (f_vertex[_rightLow].y + f_vertex[_rightTop].y) * 0.5;

	f_vertex[centerCoord].y = (f_vertex[upCoord].y + f_vertex[downCoord].y + f_vertex[leftCoord].y + f_vertex[rightCoord].y + f_rand()) * 0.2;


	if (_size < 1) {
		return;
	}
	else {
		_size *= 0.5;

		// ����t���A
		set_y(_size, _leftTop, leftCoord, upCoord, centerCoord);
		// �����t���A
		set_y(_size, leftCoord, _leftLow, centerCoord, downCoord);
		// �E��t���A
		set_y(_size, upCoord, centerCoord, _rightTop, rightCoord);
		// �E���t���A
		set_y(_size, centerCoord, downCoord, rightCoord, _rightLow);
	}
}


float f = 0;


void timer(int value) {
	f++;

	glutPostRedisplay();

	glutTimerFunc(//60fps
		1000 / 90,		//unsigned int millis,���b��ɋN�����邩�H(1000�~���b��1�b)
		timer,	//void (GLUTCALLBACK *func)(int value), �ǂ̃R�[���o�b�N�֐����ĂԂ��H
		0		//int value,�֐��ɓn������
		);
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);//�ˉe�s�񃂁[�h
	glLoadIdentity();//��U�A�s��ݒ��P�ʍs��ŏ���������

	gluPerspective(
		60, // GLdouble fovy
		1,  // GLdouble aspect
		.1, // GLdouble zNear
		100);// GLdouble zFar

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 3, 2,  // eye
		0, 0, 0,  // center
		0, 1, 0); // up


	auto v_it = f_vertex.begin();
	auto i_it = f_index.begin();


	glEnableClientState(GL_VERTEX_ARRAY);	// GLenum array
	glVertexPointer(3, GL_FLOAT, 0, &(*v_it));


	glRotatef(f, 0, 1, 0);

	glPolygonMode(
		GL_FRONT_AND_BACK,  // GLenum face
		GL_LINE);           // GLenum mode

	glDrawElements(
		GL_TRIANGLES,       // GLenum mode
		f_index.size(),// GLsizei count
		GL_UNSIGNED_SHORT,   // GLenum type
		&(*i_it));           // const GLvoid *indices


	glDisableClientState(GL_VERTEX_ARRAY);	// GLenum array


	glFlush();
}


void reshape(const int width, const int height) {
	g_windowWidth = width;
	g_windowHeight = height;
}


int main(int argc, char *argv[]) {
	glutInit(
		&argc,	//int *argcp,
		argv);	//char **argv
	glutInitWindowSize(
		g_windowWidth,  // int width
		g_windowHeight);// int height
	glutCreateWindow("���_�ψʖ@");//const char *title


	set_vertex();

	srand(time(NULL));
	set_y(F_WIDTH * F_HEIGHT,
		0,
		F_WIDTH * (F_HEIGHT - 1),
		F_WIDTH - 1,
		(F_WIDTH * F_HEIGHT - 1));
	
	set_index();


	glutDisplayFunc(display);//void (GLUTCALLBACK *func)(void)
	glutReshapeFunc(reshape);// void (GLUTCALLBACK *func)(int width, int height)

	glutTimerFunc(
		0,		//unsigned int millis,���b��ɋN�����邩�H
		timer,	//void (GLUTCALLBACK *func)(int value), �ǂ̃R�[���o�b�N�֐����ĂԂ��H
		0		//int value,�֐��ɓn������
		);

	glutMainLoop();
}