#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "glut.h"

#include "Mat4x4.h"
#include "Vec3.h"
#include "KeyBoard.h"
#include "Vec2.h"

#include "glm/gtx/intersect.hpp"

#include <stdio.h>
#include <vector>

using namespace mso;

// -------------------------------------------------------------
// gv
int g_windowWidth = 960;
int g_windowHeight = 720;

float mouse_x = 0, mouse_y = 0;


// -------------------------------------------------------------
//

// 線分構造体
struct Segment {
	Segment(const Vec2& s, const Vec2& v) { s_ = s; v_ = v; }

	Vec2 s_; // 始点
	Vec2 v_; // 方向ベクトル（線分の長さも担うので正規化しないように！）
};

// 2Dベクトルの外積
float D3DXVec2Cross(Vec2* v1, Vec2* v2) {
	return v1->x_ * v2->y_ - v1->y_ * v2->x_;
}

// 線分の衝突
bool ColSegments(
	Segment &seg1,          // 線分1
	Segment &seg2,          // 線分2
	float* outT1 = 0,       // 線分1の内分比（出力）
	float* outT2 = 0,       // 線分2の内分比（出力
	Vec2* outPos = 0        // 交点（出力）
) {

	Vec2 v = seg2.s_ - seg1.s_;
	float Crs_v1_v2 = D3DXVec2Cross(&seg1.v_, &seg2.v_);
	if (Crs_v1_v2 == 0.0f) {
		// 平行状態
		return false;
	}

	float Crs_v_v1 = D3DXVec2Cross(&v, &seg1.v_);
	float Crs_v_v2 = D3DXVec2Cross(&v, &seg2.v_);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	if (outT1)
		*outT1 = Crs_v_v2 / Crs_v1_v2;
	if (outT2)
		*outT2 = Crs_v_v1 / Crs_v1_v2;

	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// 交差していない
		return false;
	}

	if (outPos)
		*outPos = seg1.s_ + seg1.v_ * t1;

	return true;
}


// -------------------------------------------------------------
//

struct Color
{
	float r_, g_, b_;
};


class Point
{
public:
	//Point(float x, float y) { x_ = x; y_ = y; }
	Point(float x, float y, const Color& color = {}) { x_ = x; y_ = y; color_ = color; }

	float x_, y_;
	Color color_;

	void draw()
	{
		glColor3f(color_.r_, color_.g_, color_.b_);
		glBegin(GL_QUADS);
		{
			const float w = 5.0f;
			const float h = 5.0f;

			glVertex2f(x_ - w, y_ + h);
			glVertex2f(x_ + w, y_ + h);
			glVertex2f(x_ + w, y_ - h);
			glVertex2f(x_ - w, y_ - h);
		}
		glEnd();
	}

private:
	Point() {}
};

std::vector< Point > g_defaultPointList;


class MouseTracks
{
public:

	static void draw()
	{
		glColor3f(1, 1, 1);
		glLineWidth(2.0f);
		glBegin(GL_LINE_STRIP);
		{
			for (auto& p : points_)
			{
				glVertex2f(p.x_, p.y_);
			}
		}
		glEnd();

	}

	static void clear()
	{
		points_.clear();
	}

	static std::vector< Point > points_;
};
std::vector< Point > MouseTracks::points_;


void init()
{
	float p[] = {
		100, 100,
		300, 670,
		450, 240,
		650, 540,
		800, 230,
		200, 470,
		350, 40,
		550, 340,
		700, 130,
	};
	const int size = sizeof(p) / sizeof(p[0]);

	for (int i = 0; i < size; i += 2)
	{
		Point point(p[i], p[i + 1], { 0, 0, 1 });
		g_defaultPointList.push_back(point);
	}


#if 0
	// col test

	Vec2 start(100, 100);
	Vec2 end(100, 600);

	Vec2 mouse(50, 300);
	Vec2 mouse_end(150, 300);

	Segment s1(start, end - start);
	Segment s2(mouse, mouse_end - mouse);

	if (ColSegments(s1, s2))
	{
		printf("true\n");
	}
#endif

}


void cut_mouse_tracks()
{
	if (!MouseTracks::points_.size()) return;

	int prev_size = MouseTracks::points_.size();

	int smallIndex = prev_size;
	int bigIndex = 0;

	for (int j = 0; j < MouseTracks::points_.size() - 1; j++)
	{
		int crossCount = 0;

		int crossNum = 0;

		Vec2 s(MouseTracks::points_[j].x_, MouseTracks::points_[j].y_);
		Vec2 e(MouseTracks::points_[j + 1].x_, MouseTracks::points_[j + 1].y_);

		for (int i = 0; i < MouseTracks::points_.size() - 1; i++)
		{
			if (j == i || (j - 1) == i || (j + 1) == i) continue;

			Vec2 m1(MouseTracks::points_[i].x_, MouseTracks::points_[i].y_);
			Vec2 m2(MouseTracks::points_[i + 1].x_, MouseTracks::points_[i + 1].y_);

			Segment s1(s, e - s);
			Segment s2(m1, m2 - m1);

			if (ColSegments(s1, s2))
			{
				printf("j(%d) i(%d)\n", j, i);

				if (j > i)
				{
					if (smallIndex > i) smallIndex = i;
					if (bigIndex < j)   bigIndex = j;
				}
				else
				{
					if (smallIndex > j) smallIndex = j;
					if (bigIndex < i)   bigIndex = i;
				}

				if ((i - 1) != crossNum)
					++crossCount;
				crossNum = i;
			}

		}
	}

	printf("small(%d) big(%d)\n----------\n", smallIndex, bigIndex);

	// 後半カット
	if (bigIndex != 0)
	{
		MouseTracks::points_.erase(
			MouseTracks::points_.begin() + (bigIndex + 1),
			MouseTracks::points_.end());
	}

	// 前半カット
	if (smallIndex != prev_size)
	{
		MouseTracks::points_.erase(
			MouseTracks::points_.begin(),
			MouseTracks::points_.begin() + (smallIndex /*+ 1*/));
	}

}


// マウスボタンが押されたり、離された時のコールバック
void mouse(int button, int state, int x, int y) {

	if (state == GLUT_UP)
	{
	}

}

// ボタンが押された状態でマウスが動いた時のコールバック
void motion(int x, int y) {

	// 左上(0, 0)
	//printf("mouse : x(%d) y(%d)\n", x, y);

	// display : 左下(0, 0)
	mouse_x = x;
	mouse_y = g_windowHeight - y;

	// 
	Point point(x, g_windowHeight - y);
	MouseTracks::points_.push_back(point);

}

// ボタンが押されていない状態でマウスが動いた時のコールバック
void passiveMotion(int x, int y) {
}


void keyboard(unsigned char key, int x, int y) { KeyBoard::GetState(key); }
void keyboard_up(unsigned char key, int x, int y) { KeyBoard::GetState(key, false); }

void special(int key, int x, int y) { }
void special_up(int key, int x, int y) { }


void timer(int value)
{
	glutPostRedisplay();

	glutTimerFunc(1000 / 60, timer, 0);
}


void display(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, g_windowWidth, g_windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, g_windowWidth, 0, g_windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 0, 1,  // eye
		0, 0, 0,  // center
		0, 1, 0); // up

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_NORMALIZE);


	if (KeyBoard::IsPressMoment('r'))
	{
		MouseTracks::clear();

		for (auto& p : g_defaultPointList)
		{
			p.color_ = { 0, 0, 1 };
		}

	}

#if 0
	// mouse
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	{
		const float w = 5.0f;
		const float h = 5.0f;

		glVertex2f(mouse_x - w, mouse_y + h);
		glVertex2f(mouse_x + w, mouse_y + h);
		glVertex2f(mouse_x + w, mouse_y - h);
		glVertex2f(mouse_x - w, mouse_y - h);
	}
	glEnd();
#endif

	// default point
	for (auto& p : g_defaultPointList)
	{
		p.draw();
	}

	// お絵かき
	MouseTracks::draw();


	// judge
	if (KeyBoard::IsPressMoment('q') && MouseTracks::points_.size())
	{

		cut_mouse_tracks();

		for (auto& p : g_defaultPointList)
		{
			int crossCount = 0;

			int crossNum = 0;

			Vec2 s(p.x_, p.y_);
			Vec2 e(p.x_ + 1000, p.y_); // 右に伸ばす

			for (int i = 0; i < MouseTracks::points_.size() - 1; i++)
			{
				Vec2 m1(MouseTracks::points_[i].x_, MouseTracks::points_[i].y_);
				Vec2 m2(MouseTracks::points_[i + 1].x_, MouseTracks::points_[i + 1].y_);

				Segment s1(s, e - s);
				Segment s2(m1, m2 - m1);

				if (ColSegments(s1, s2))
				{
					if ((i - 1) != crossNum)
						++crossCount;
					crossNum = i;
				}

			}

			if (crossCount % 2)
				p.color_ = { 0, 1, 0 };

		}

	}


	// 切り取り
	if (KeyBoard::IsPressMoment('x'))
	{
		cut_mouse_tracks();

	}


	// mouse point debug
	if (KeyBoard::IsPressMoment('p'))
	{
		for (int i = 0; i < MouseTracks::points_.size(); i++)
		{
			printf("point(%d) (%.1f)(%.1f)\n", i, MouseTracks::points_[i].x_, MouseTracks::points_[i].y_);
		}

	}


	KeyBoard::KeepPrevState();

	glFlush();
}


void reshape(int width, int height)
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

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(special);
	glutSpecialUpFunc(special_up);
	glutIgnoreKeyRepeat(GL_TRUE);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);

	glutTimerFunc(0, timer, 0);

	init();

	glutMainLoop();
}