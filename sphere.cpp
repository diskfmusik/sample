#include <stdio.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <assert.h>
#include <vector>
#include "glut.h"
#include "Vec3.h"


int g_windowWidth = 960;// 640
int g_windowHeight = 720;// 480

#define F_WIDTH (35)
#define F_HEIGHT (35)

std::vector<Vec3> f_coord;// ���_���W
std::vector<Vec3> f_vertex;// ���_�z��(�@�������߈Ղ�����ׂɕ��ёւ���)
std::vector<unsigned short>f_index;// �ʃf�[�^
std::vector<Vec3> f_normal;// �@��
std::vector<Vec3> gr_normal;

Vec3 camera = Vec3(0, 1, 0);
float cameraYaw = 0;




// 0 ~ 1.0f ��Ԃ�
float f_rand() {
	int i = rand() % 101;
	return i / 100.0f;
}


void set_coord() {
	int x, z;


	for (z = 0; z < F_HEIGHT; z++) {
		for (x = 0; x < F_WIDTH; x++) {

			float r = (float)sin(M_PI / (F_HEIGHT - 1) * z) * F_HEIGHT;
			float y = (float)cos(M_PI / (F_HEIGHT - 1) * z) * F_HEIGHT;

			f_coord.push_back(
				Vec3(
					sin(M_PI * 2 / (F_WIDTH - 1) * x) * r,
					y,
					cos(M_PI * 2 / (F_WIDTH - 1) * x) * r)
				);
		}
	}
}


void set_index() {
	unsigned int x, z;

	/* ���_�z�� */
	for (z = 0; z < (F_HEIGHT - 1); z++) {
		for (x = 0; x < (F_WIDTH - 1); x++) {
			f_vertex.push_back(f_coord[0 + x + F_HEIGHT * z]);
			f_vertex.push_back(f_coord[F_WIDTH + x + F_HEIGHT * z]);
			f_vertex.push_back(f_coord[1 + x + F_HEIGHT * z]);

			f_vertex.push_back(f_coord[1 + x + F_HEIGHT * z]);
			f_vertex.push_back(f_coord[F_WIDTH + x + F_HEIGHT * z]);
			f_vertex.push_back(f_coord[(F_WIDTH + 1) + x + F_HEIGHT * z]);
		}
	}

	/* �ʃf�[�^ */
	for (x = 0; x < f_vertex.size(); x++) {
		f_index.push_back(x);
	}
}


// ���_�ψʖ@
void set_y(int _size, const int _leftTop, const int _leftLow, const int _rightTop, const int _rightLow) {

	// ���ԍ��W
	const int upCoord = (_leftTop + _rightTop) * 0.5;
	const int downCoord = (_leftLow + _rightLow) * 0.5;
	const int leftCoord = (_leftLow + _leftTop) * 0.5;
	const int rightCoord = (_rightLow + _rightTop) * 0.5;
	const int centerCoord = (_leftTop + _leftLow + _rightTop + _rightLow) * 0.25;

	// 4���݂�y���Wrand
	f_coord[_leftTop].y = f_rand() * _size;
	f_coord[_leftLow].y = f_rand() * _size;
	f_coord[_rightTop].y = f_rand() * _size;
	f_coord[_rightLow].y = f_rand() * _size;

	// 4���݂̒���
	f_coord[upCoord].y = (f_coord[_leftTop].y + f_coord[_rightTop].y) * 0.5;
	f_coord[downCoord].y = (f_coord[_leftLow].y + f_coord[_rightLow].y) * 0.5;
	f_coord[leftCoord].y = (f_coord[_leftLow].y + f_coord[_leftTop].y) * 0.5;
	f_coord[rightCoord].y = (f_coord[_rightLow].y + f_coord[_rightTop].y) * 0.5;

	f_coord[centerCoord].y = (f_coord[upCoord].y + f_coord[downCoord].y + f_coord[leftCoord].y + f_coord[rightCoord].y + f_rand()) * 0.2;


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


void get_flat_normal() {
	int i;
	Vec3 vOut, v1, v2;
	int size = f_vertex.size();

	/* �ʂ̐������̖@�� */
	std::vector<Vec3> index_normal;

	for (i = 0; i < size; i += 3) {

		v1 = Vec3(//1 - 0
			f_vertex[(i + 1)] - f_vertex[i]);

		v2 = Vec3(//2 - 1
			f_vertex[(i + 2)] - f_vertex[(i + 1)]);

		/* �O�� */
		vOut = Vec3(
			v1.y * v2.z - v2.y * v1.z,
			v1.z * v2.x - v2.z * v1.x,
			v1.x * v2.y - v2.x * v1.y);

		vOut = vOut / vOut.Length();// ���K��


		index_normal.push_back(vOut);
	}


	size = index_normal.size();

	for (i = 0; i < size; i++) {

		for (int j = 0; j < 3; j++) {
			f_normal.push_back(index_normal[i]);
		}
	}
}


void get_gouraud_normal() {
	int i, num;
	Vec3 vOut;
	int size = f_vertex.size();

	printf("size:%d\n", size);

	for (num = 0; num < size; num++) {

		vOut *= 0;// ��U�A������
		for (i = 0; i < size; i++) {
			if (f_vertex[num] == f_vertex[i]) {

				vOut += f_vertex[i];
			}
		}

		vOut = vOut / vOut.Length();// ���K��

		gr_normal.push_back(vOut);
	}
}




void joystick(unsigned int buttonMask, int x, int y, int z) {
}


//-------------------------------------------------------------------------------------------------
// timer
//-------------------------------------------------------------------------------------------------
void timer(int value) {
	//glutForceJoystickFunc();

	cameraYaw += 0.01f;


	glutPostRedisplay();

	glutTimerFunc(//60fps
		1000 / 90,		//unsigned int millis,���b��ɋN�����邩�H(1000�~���b��1�b)
		timer,	//void (GLUTCALLBACK *func)(int value), �ǂ̃R�[���o�b�N�֐����ĂԂ��H
		0		//int value,�֐��ɓn������
		);
}


//-------------------------------------------------------------------------------------------------
// display
//-------------------------------------------------------------------------------------------------
void display(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);//�ˉe�s�񃂁[�h
	glLoadIdentity();//��U�A�s��ݒ��P�ʍs��ŏ���������

	gluPerspective(
		60, // GLdouble fovy
		(float)g_windowWidth / g_windowHeight,  // GLdouble aspect w/h
		1, // GLdouble zNear
		100);// GLdouble zFar

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		sin(cameraYaw)*10.0f, 0.0f, cos(cameraYaw)*10.0f,	// eye
		0, 0, 0,	// center
		0, 1, 0); // up


	glEnable(GL_DEPTH_TEST);	// GLenum cap
	glEnable(GL_CULL_FACE);	// GLenum cap

	glEnable(GL_LIGHTING);	// GLenum cap
	glEnable(GL_LIGHT0);	// GLenum cap
	glEnable(GL_NORMALIZE);




	auto v_it = f_vertex.begin();
	auto i_it = f_index.begin();
	auto n_it = f_normal.begin();


	glEnableClientState(GL_VERTEX_ARRAY);	// GLenum array
	glVertexPointer(3, GL_FLOAT, 0, &(*v_it));

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, &(*n_it));


	/* �t�B�[���h */
	glPushMatrix();
	{
		glScalef(0.05f, 0.05f, 0.05f);

		//glPolygonMode(
		//	GL_FRONT_AND_BACK,  // GLenum face
		//	GL_LINE);           // GLenum mode


		glDrawElements(
			GL_TRIANGLES,       // GLenum mode
			f_index.size(),		// GLsizei count
			GL_UNSIGNED_SHORT,  // GLenum type
			&(*i_it));          // const GLvoid *indices
	}
	glPopMatrix();


	glDisableClientState(GL_VERTEX_ARRAY);	// GLenum array
	glDisableClientState(GL_NORMAL_ARRAY);


	glFlush();
}


void reshape(const int width, const int height) {
	g_windowWidth = width;
	g_windowHeight = height;
}


//-------------------------------------------------------------------------------------------------
// main
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	glutInit(
		&argc,	//int *argcp,
		argv);	//char **argv
	glutInitWindowSize(g_windowWidth, g_windowHeight);
	glutCreateWindow("�G�p!!");//const char *title


	set_coord();

	//srand(time(NULL));
	//set_y(F_WIDTH * F_HEIGHT,
	//	0,
	//	F_WIDTH * (F_HEIGHT - 1),
	//	F_WIDTH - 1,
	//	(F_WIDTH * F_HEIGHT - 1));

	set_index();

	get_flat_normal();
	//get_gouraud_normal();


	//�֐��̓o�^
	glutDisplayFunc(display);//void (GLUTCALLBACK *func)(void)
	glutReshapeFunc(reshape);// void (GLUTCALLBACK *func)(int width, int height)
	glutJoystickFunc(joystick, 0);//void (GLUTCALLBACK *func)(unsigned int buttonMask, int x, int y, int z), int pollInterval

	glutTimerFunc(
		0,		//unsigned int millis,���b��ɋN�����邩�H
		timer,	//void (GLUTCALLBACK *func)(int value), �ǂ̃R�[���o�b�N�֐����ĂԂ��H
		0		//int value,�֐��ɓn������
		);

	glutMainLoop();
}