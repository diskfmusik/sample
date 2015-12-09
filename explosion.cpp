#include <stdio.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <assert.h>
#include <vector>
#include "glut.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Vec3.h"


int g_windowWidth = 960;//640
int g_windowHeight = 720;//480

GLuint g_texID;
glm::mat4 g_lookAt;// view行列


//-------------------------------------------------------------------------------------------------
// Particles
//-------------------------------------------------------------------------------------------------
class Particles {
public:
	Vec3 m_pos;
	float m_scale, m_alpha;
	bool m_isActice;

	Particles() { m_pos = Vec3(0, 0, 0); m_scale = 0; m_alpha = 0; m_isActice = true; }
	~Particles() {}

	virtual void Draw(const Vec3 &_pos)const = 0;
	virtual void Update() = 0;
};




//-------------------------------------------------------------------------------------------------
// FireEffect
//-------------------------------------------------------------------------------------------------
class FireEffect :public Particles {
public:

	FireEffect() {}
	FireEffect(const Vec3 &_pos, const float _s, const float _a) {
		m_pos = _pos;
		m_scale = _s;
		m_alpha = _a;
		m_isActice = true;
	}
	~FireEffect() {}

	virtual void Draw(const Vec3 &_pos)const override;
	virtual void Update()override;
};


void FireEffect::Draw(const Vec3 &_pos)const {
	glPushMatrix();
	{
		glm::mat4 bil = glm::inverse(g_lookAt);
		//二次元配列で [0][0] が 一番上
		bil[3][0] = 0;
		bil[3][1] = 0;
		bil[3][2] = 0;

		glMultMatrixf((GLfloat*)&bil);// const GLfloat *m

		//加算aブレンド
		glBlendFunc(
			GL_SRC_ALPHA,   // GLenum sfactor
			GL_ONE);        // GLenum dfactor
		glEnable(GL_BLEND);// GLenum cap

		//------------------------------------------------------------------
		//テクスチャ描画
		//------------------------------------------------------------------

		glColor4f(0.9, 0.4, 0.15, m_alpha);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, g_texID);

		glTranslatef(_pos.x + m_pos.x, _pos.y + m_pos.y, _pos.z + m_pos.z);
		glScalef(m_scale / 1.5f, m_scale / 1.25f, m_scale / 1.5f);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1);
			glTexCoord2f(1, 0);
			glVertex2f(1, -1);
			glTexCoord2f(1, 1);
			glVertex2f(1, 1);
			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();
}


void FireEffect::Update() {
	m_alpha -= 0.25f / 5;
	m_pos.y += 0.075f / 5;
	m_scale += 0.1f / 5;

	if (m_alpha < 0) m_isActice = false;
}




//-------------------------------------------------------------------------------------------------
// SmokeEffect
//-------------------------------------------------------------------------------------------------
class SmokeEffect :public Particles {
public:
	float m_color;

	SmokeEffect() {}
	SmokeEffect(const Vec3 &_pos, const float _s, const float _a) {
		m_pos = _pos;
		m_scale = _s;
		m_alpha = _a;
		m_isActice = true;
		m_color = 0.15f;
	}
	~SmokeEffect() {}

	virtual void Draw(const Vec3 &_pos)const override;
	virtual void Update()override;
};


void SmokeEffect::Draw(const Vec3 &_pos)const {
	glPushMatrix();
	{
		glm::mat4 bil = glm::inverse(g_lookAt);
		//二次元配列で [0][0] が 一番上
		bil[3][0] = 0;
		bil[3][1] = 0;
		bil[3][2] = 0;

		glMultMatrixf((GLfloat*)&bil);// const GLfloat *m

		// aブレンド
		glBlendFunc(
			GL_SRC_ALPHA,           // GLenum sfactor
			GL_ONE_MINUS_SRC_ALPHA);// GLenum dfactor
		glEnable(GL_BLEND);// GLenum cap

		//------------------------------------------------------------------
		// テクスチャ描画
		//------------------------------------------------------------------

		glColor4f(m_color, m_color, m_color, m_alpha);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, g_texID);

		glTranslatef(_pos.x + m_pos.x, _pos.y + m_pos.y, _pos.z /*+ m_pos.z*/);
		glScalef(m_scale / 1.5f, m_scale / 2.0f, m_scale / 1.5f);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1);
			glTexCoord2f(1, 0);
			glVertex2f(1, -1);
			glTexCoord2f(1, 1);
			glVertex2f(1, 1);
			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();
}


void SmokeEffect::Update() {
	float f = (rand() % 100) / 1000.0f;// 0 ~ +0.1f

	m_alpha -= 0.15f / 8;
	m_pos.y += (0.025f + f) / 5;
	m_scale += 0.05f / 5;
	m_color += 0.005f;

	if (m_alpha < 0) m_isActice = false;
}




//-------------------------------------------------------------------------------------------------
// Explode
//-------------------------------------------------------------------------------------------------
class Explode {
public:
	bool m_isActive, m_isFire, m_isExplode;
	int m_counter;
	Vec3 m_setPos;
	std::vector<FireEffect*> v_fire;
	std::vector<SmokeEffect*> v_smoke;

	Explode();
	Explode(const Vec3 &_pos);
	~Explode() {}

	void Draw()const;
	void Update();
};


Explode::Explode() {
	m_isActive = true;
	m_isExplode = true;
	m_isFire = true;
	m_counter = 0;
}


Explode::Explode(const Vec3 &_pos) {
	m_isActive = true;
	m_isExplode = true;
	m_isFire = true;
	m_counter = 0;
	m_setPos = _pos;
}


void Explode::Draw()const {

	auto f_it = v_fire.begin();
	while (f_it != v_fire.end()) {
		(*f_it)->Draw(m_setPos);
		f_it++;
	}

	auto s_it = v_smoke.begin();
	while (s_it != v_smoke.end()) {
		(*s_it)->Draw(m_setPos);
		s_it++;
	}
}


void Explode::Update() {
	m_counter++;

	if (m_isExplode) {

		if (m_isFire && v_fire.size() < 16) {
			float f = (-100 + rand() % 200) / 100.0f;// -1.0f ~ +1.0f
			FireEffect *fire = new FireEffect(Vec3(f / 40, 0, f / 40), 0, 1.0f);
			v_fire.push_back(fire);
			if (v_fire.size() >= 15)m_isFire = false;
		}

		if (m_counter % 2 == 0 && v_smoke.size() < 16 && m_isFire == false) {
			float f = (-100 + rand() % 200) / 100.0f;// -1.0f ~ +1.0f
			SmokeEffect *smoke = new SmokeEffect(Vec3(f / 40, 0, f / 40), 0, 1.0f);
			v_smoke.push_back(smoke);
			if (v_smoke.size() >= 15)m_isExplode = false;
		}

	}// if (m_isExplode) 
	// 大きくなるほど拡大率を下げる

	std::vector<FireEffect*>::iterator f_it = v_fire.begin();
	while (f_it != v_fire.end()) {
		(*f_it)->Update();
		if ((*f_it)->m_isActice == false) {
			delete (*f_it);
			f_it = v_fire.erase(f_it);
			continue;
		}

		f_it++;
	}

	std::vector<SmokeEffect*>::iterator s_it = v_smoke.begin();
	while (s_it != v_smoke.end()) {
		(*s_it)->Update();
		if ((*s_it)->m_isActice == false) {
			delete (*s_it);
			s_it = v_smoke.erase(s_it);
			continue;
		}
		s_it++;
	}

	if (v_fire.size() == 0 && v_smoke.size() == 0 && m_counter > 0) {
		m_isActive = false;
	}

}


std::vector<Explode*> v_explode;


struct RGB {
	unsigned char r, g, b;
};

struct RGBA {
	unsigned char r, g, b, a;
};


void load_image_bmp(const char * _FileName) {
	FILE *pFile;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	RGB *pixels;
	RGBA *pixels4f;

	fopen_s(
		&pFile,
		_FileName, // const char * _Filename
		"rb");      // const char * _Mode

	if (pFile == NULL) {
		printf("mapが　ありません！\n");
		assert(NULL);
	}

	fread(&bf, sizeof(bf), 1, pFile);//ファイルヘッダ
	fread(&bi, sizeof(bi), 1, pFile);//情報ヘッダ

	pixels = (RGB*)malloc(bi.biHeight*bi.biWidth * sizeof(RGB));

	fread(pixels, bi.biHeight*bi.biWidth * sizeof(RGB), 1, pFile);//ピクセルデータ読み込み
	fclose(pFile);// FILE * _File


	/*rとbのソート*/
	unsigned char escape_bit;
	for (int i = 0; i < bi.biHeight * bi.biWidth; i++) {
		escape_bit = pixels[i].b;
		pixels[i].b = pixels[i].r;
		pixels[i].r = escape_bit;
	}

	/*上下反転ソート*/
	RGB escape_rgb;
	for (int y = 0; y < bi.biHeight / 2; y++) {
		for (int x = 0; x < bi.biWidth; x++) {
			escape_rgb = pixels[y*bi.biWidth + x];
			pixels[y*bi.biWidth + x] = pixels[((bi.biHeight - 1) - y)*bi.biWidth + x];
			pixels[((bi.biHeight - 1) - y)*bi.biWidth + x] = escape_rgb;
		}
	}


	//----------------------------------------------------------------------------
	//アルファチャンネル付きピクセルデータ
	//----------------------------------------------------------------------------

	pixels4f = (RGBA*)malloc(bi.biHeight*bi.biWidth * sizeof(RGBA));

	for (int i = 0; i < bi.biHeight * bi.biWidth; i++) {
		pixels4f[i].r = 0xff;
		pixels4f[i].g = 0xff;
		pixels4f[i].b = 0xff;
		pixels4f[i].a = pixels[i].r;
	}


	/*ピクセルデータを VRAM に転送*/
	glTexImage2D(
		GL_TEXTURE_2D,      // GLenum target
		0,                  // GLint level
		GL_RGBA,             // GLint internalformat
		bi.biWidth,
		bi.biHeight,// GLsizei width, height
		0,                  // GLint border
		GL_RGBA,             // GLenum format
		GL_UNSIGNED_BYTE,   // GLenum type
		pixels4f);// const GLvoid *pixels


	free(pixels);
	free(pixels4f);
}




void joystick(unsigned int buttonMask, int x, int y, int z) {

	if (buttonMask) {

		float f = (-100 + rand() % 200) / 100.0f;// -1.0f ~ +1.0f
		//Explode *exp = new Explode(Vec3(sin(f)*1.0f, 0, cos(f)*1.0f));
		Explode *exp = new Explode(Vec3(0, 0, 0));
		v_explode.push_back(exp);
	}
}

// z < 0.5f
void keyboard(unsigned char key, int x, int y) {
	// 二次元空間への描画だから、z座標は変換が必要
	// カメラ位置からの距離を取る
	if (key) {
		float f = (-100 + rand() % 200) / 100.0f;// -1.0f ~ +1.0f
		Explode *exp = new Explode(Vec3(0, 0, 0));
		v_explode.push_back(exp);
	}
}

float f = 0;


//---------------------------------------------------------------------------------------
// timer
//---------------------------------------------------------------------------------------
void timer(int value) {
	//glutForceJoystickFunc();

	f += 0.01f;

	auto ex_itr = v_explode.begin();
	while (ex_itr != v_explode.end()) {
		(*ex_itr)->Update();

		if ((*ex_itr)->m_isActive == false) {
			delete (*ex_itr);
			ex_itr = v_explode.erase(ex_itr);
			continue;
		}
		ex_itr++;
	}


	glutPostRedisplay();


	glutTimerFunc(//60fps
		1000 / 90,		//unsigned int millis,何秒後に起動するか？(1000ミリ秒＝1秒)
		timer,	//void (GLUTCALLBACK *func)(int value), どのコールバック関数を呼ぶか？
		0		//int value,関数に渡す引数
		);
}




//---------------------------------------------------------------------------------------
// display
//---------------------------------------------------------------------------------------
void display(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, g_windowWidth, g_windowHeight);


	glMatrixMode(GL_PROJECTION);//射影行列モード
	glLoadIdentity();//一旦、行列設定を単位行列で初期化する

	glm::mat4 projection = glm::perspective(
		90.0f,		// valType const & fovy
		1.0f,		// valType const & aspect
		0.1f,		// valType const & zNear
		10.0f);		// valType const & zFar
	glMultMatrixf((GLfloat*)&projection);// const GLfloat *m


	glMatrixMode(GL_MODELVIEW);//モデル・ビュー行列モード
	glLoadIdentity();

	g_lookAt = glm::lookAt(
		glm::vec3(sin(f)*0.5f, 0.5f, cos(f)*0.5f),	// eye
		glm::vec3(0.0f, 0.0f, 0.0f),				// center
		glm::vec3(0.0f, 1.0f, 0.0f)					// up
		);
	glMultMatrixf((GLfloat*)&g_lookAt);// const GLfloat *m


	glEnable(GL_DEPTH_TEST);	// GLenum cap
	glEnable(GL_CULL_FACE);	// GLenum cap


	/*テクスチャを縮小描画する場合は必須*/
	glTexParameteri(
		GL_TEXTURE_2D,          // GLenum target
		GL_TEXTURE_MIN_FILTER,  // GLenum pname
		GL_NEAREST);            // GLint param

	/*テクスチャを拡大描画する場合は必須*/
	glTexParameteri(
		GL_TEXTURE_2D,        // GLenum target
		GL_TEXTURE_MAG_FILTER,// GLenum pname
		GL_NEAREST);          // GLint param




	//--------------------------------------------------------------------------------------
	//フィールド
	//--------------------------------------------------------------------------------------
	glBegin(GL_LINES);
	{
		for (int i = 0; i <= 20; i++) {
			glColor3f(1, 0, 0);
			glVertex3f(1.0f, 0, -1.0f + 0.1f*i);//GLfloat x, y, z
			glVertex3f(-1.0f, 0, -1.0f + 0.1f*i);//GLfloat x, y, z
		}
		for (int i = 0; i <= 20; i++) {
			glColor3f(0, 1, 0);
			glVertex3f(-1.0f + 0.1f*i, 0, 1.0f);//GLfloat x, y, z
			glVertex3f(-1.0f + 0.1f*i, 0, -1.0f);//GLfloat x, y, z
		}
	}
	glEnd();



	//---------------------------------------------------------------------------------------
	// particles
	//---------------------------------------------------------------------------------------
	glDepthMask(GL_FALSE);// GLboolean flag

	auto ex_itr = v_explode.begin();
	while (ex_itr != v_explode.end()) {
		(*ex_itr)->Draw();
		ex_itr++;
	}

	glDepthMask(GL_TRUE);// GLboolean flag



	glFlush();
}




void reshape(const int width, const int height) {
	g_windowWidth = width;
	g_windowHeight = height;
}




//---------------------------------------------------------------------------------------
// main
//---------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	glutInit(
		&argc,	//int *argcp,
		argv);	//char **argv
	glutInitWindowSize(
		g_windowWidth,  // int width
		g_windowHeight);// int height
	glutCreateWindow("title");//const char *title

	glGenTextures(
		1,          // GLsizei n
		&g_texID);  // GLuint *textures

	glBindTexture(GL_TEXTURE_2D, g_texID);
	load_image_bmp("smokevol1.bmp");


	// callback関数の登録
	glutDisplayFunc(display);//void (GLUTCALLBACK *func)(void)
	glutKeyboardFunc(keyboard); // void (GLUTCALLBACK *func)(unsigned char key, int x, int y)
	glutReshapeFunc(reshape);// void (GLUTCALLBACK *func)(int width, int height)
	glutJoystickFunc(joystick, 0);//void (GLUTCALLBACK *func)(unsigned int buttonMask, int x, int y, int z), int pollInterval

	glutTimerFunc(
		0,		//unsigned int millis,何秒後に起動するか？
		timer,	//void (GLUTCALLBACK *func)(int value), どのコールバック関数を呼ぶか？
		0		//int value,関数に渡す引数
		);

	srand(time(NULL));
	glutMainLoop();
}