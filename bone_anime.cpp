#define _USE_MATH_DEFINES
#include <math.h>
#include "glut.h"

#include "masu/Mat4x4.h"
#include "masu/Vec3.h"


using namespace mso;

int g_windowWidth = 960;
int g_windowHeight = 720;


void timer(int value)
{
	glutPostRedisplay();

	glutTimerFunc(1000 / 60, timer, 0);
}




struct MeshTransform
{
	MeshTransform() :
		parent_(nullptr),
		child_(nullptr),
		brother_(nullptr)
	{ }

	void SetFamily(MeshTransform* parent, MeshTransform* child, MeshTransform* brother)
	{
		parent_ = parent;
		child_ = child;
		brother_ = brother;
	}

	Mat4x4 scale_;
	Mat4x4 rot_;
	Mat4x4 trans_;
	Mat4x4 offset_;

	// ワールド : 子や兄弟に渡す為の行列
	// scale の情報を持たせてはならない
	Mat4x4 world_;

	MeshTransform* parent_;
	MeshTransform* child_;
	MeshTransform* brother_;
};
MeshTransform* parent = nullptr;
MeshTransform* head = nullptr;
MeshTransform* rightUpArm = nullptr;
MeshTransform* leftUpArm = nullptr;

MeshTransform* rightHand = nullptr;
MeshTransform* leftHand = nullptr;


void mesh_init()
{
	parent = new MeshTransform();
	head = new MeshTransform();
	rightUpArm = new MeshTransform();
	leftUpArm = new MeshTransform();
	rightHand = new MeshTransform();
	leftHand = new MeshTransform();

	// parent
	//   |
	// head -> rightUpArm -> leftUpArm
	//             |            |
	//         rightHand     leftHand

	parent->SetFamily(nullptr, head, nullptr);

	head->SetFamily(parent, nullptr, rightUpArm);
	rightUpArm->SetFamily(parent, rightHand, leftUpArm);
	leftUpArm->SetFamily(parent, leftHand, nullptr);

	rightHand->SetFamily(rightUpArm, nullptr, nullptr);
	leftHand->SetFamily(leftUpArm, nullptr, nullptr);
}


void draw(MeshTransform* mesh)
{
	Mat4x4 world;
	if (mesh->parent_ != nullptr) world = mesh->parent_->world_;

	glPushMatrix();
	{
		glMultMatrixf((GLfloat*)&(world * mesh->world_ * mesh->scale_));

		glutSolidSphere(1.0, 100, 100);
	}
	glPopMatrix();
}


void aaa(MeshTransform* mesh)
{
	draw(mesh);

	if (mesh->child_ != nullptr) aaa(mesh->child_);
	if (mesh->brother_ != nullptr) aaa(mesh->brother_);
}


void display(void)
{
	glClearColor(0.5, 0, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, g_windowWidth, g_windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)g_windowWidth / g_windowHeight, 0.1f, 10.0f);

	static float f = M_PI / 4;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		sin(f) * 6.0f, 1.5f, cos(f) * 6.0f,
		0, 0, 0,
		0, 1, 0);

	//--------------------------------------------------------------------------------------
	//フィールド
	//--------------------------------------------------------------------------------------
	glPushMatrix();
	{
		glScalef(5, 1, 5);
		glBegin(GL_LINES);
		{
			for (int i = 0; i <= 20; i++) {
				glColor3f(1, 0, 0);
				glVertex3f(1.0f, 0, -1.0f + 0.1f*i);
				glVertex3f(-1.0f, 0, -1.0f + 0.1f*i);
			}
			for (int i = 0; i <= 20; i++) {
				glColor3f(0, 1, 0);
				glVertex3f(-1.0f + 0.1f*i, 0, 1.0f);
				glVertex3f(-1.0f + 0.1f*i, 0, -1.0f);
			}
		}
		glEnd();
	}
	glPopMatrix();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);


	glColor3f(1, 1, 1);

	// update

	{ // parent
		parent->offset_ = Mat4x4::Translate(0.0f, 0.5f, 0.0f);
		parent->scale_ = Mat4x4::Scale(1.0f, 2.0f, 0.5f);
		parent->world_ = parent->offset_;
	}

	{ // head
		head->offset_ = Mat4x4::Translate(0.0f, 1.0f, 0.0f);
		head->trans_ = Mat4x4::Translate(0.0f, 1.5f, 0.0f);
		head->world_ = head->offset_ * head->trans_;
	}

	static int frame = 0;
	frame++;
	float pitch = sinf(frame * 0.1f);
	{ // rightUpArm
		rightUpArm->scale_ = Mat4x4::Scale(0.5f, 1.0f, 0.5f);
		rightUpArm->offset_ = Mat4x4::Translate(-1.5f, 0.5f, 0.0f);
		rightUpArm->trans_ = Mat4x4::Translate(0.0f, -0.5f, 0.0f);
		rightUpArm->rot_ = Mat4x4::RotateX(pitch);
		rightUpArm->world_ = rightUpArm->offset_ * rightUpArm->rot_ * rightUpArm->trans_;
	}

	pitch = sinf(M_PI + frame * 0.1f);
	{ // leftUpArm
		leftUpArm->scale_ = Mat4x4::Scale(0.5f, 1.0f, 0.5f);
		leftUpArm->offset_ = Mat4x4::Translate(1.5f, 0.5f, 0.0f);
		leftUpArm->trans_ = Mat4x4::Translate(0.0f, -0.5f, 0.0f);
		leftUpArm->rot_ = Mat4x4::RotateX(pitch);
		leftUpArm->world_ = leftUpArm->offset_ * leftUpArm->rot_ * leftUpArm->trans_;
	}

	{ // rightHand
		rightHand->offset_ = Mat4x4::Translate(0.0f, -1.0f, 0.0f);
		rightHand->scale_ = Mat4x4::Scale(0.5f, 0.5f, 0.5f);
		rightHand->world_ = rightHand->offset_;
	}

	{ // leftHand
		leftHand->offset_ = Mat4x4::Translate(0.0f, -1.0f, 0.0f);
		leftHand->scale_ = Mat4x4::Scale(0.5f, 0.5f, 0.5f);
		leftHand->world_ = leftHand->offset_;
	}


	// draw

	aaa(parent);


	glFlush();
}


void reshape(const int width, const int height)
{
	g_windowWidth = width;
	g_windowHeight = height;
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(g_windowWidth, g_windowHeight);
	glutCreateWindow("title");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutTimerFunc(0, timer, 0);


	mesh_init();


	glutMainLoop();
}