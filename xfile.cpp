#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<windows.h>
#include<assert.h>
#include"glut.h"
#include <vector>

float yaw = 0;

std::vector<float> vertex;
std::vector<unsigned short> index;
std::vector<float> normal;
std::vector<float> texture;

int vertexNum = 0;
int indexNum = 0;
int normalNum = 0;
int texNum = 0;


void load_x(const char *_fileName)
{
	FILE *fp;
	int c;
	char buf[256];

	float verPoint;
	unsigned short indexPoint;
	float normalPoint;
	float texPoint;


	fopen_s(&fp, _fileName, "r");

	if (fp == NULL)
	{
		printf("mapが　ありません！\n");
		assert(NULL);
	}


	while (1)
	{
		fscanf_s(fp, "%s", buf, 256);

		if (strcmp(buf, "template") == 0)
		{
			while (c = getc(fp) != '}') {}
		}

		if (strcmp(buf, "Mesh") == 0)break;
	}

	//-------------------------------------------------------------------------
	// vertex
	//-------------------------------------------------------------------------
	fscanf_s(fp, "%*1s");

	/* 頂点数 */
	fscanf_s(fp, "%d", &vertexNum);
	printf("vertexNum:%d\n", vertexNum);

	fscanf_s(fp, "%*1s");// 一文字読み飛ばし

	/* 頂点座標 */
	for (int i = 0; i < vertexNum; i++)
	{
		fscanf_s(fp, "%f", &verPoint);
		vertex.push_back(verPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%f", &verPoint);
		vertex.push_back(verPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%f", &verPoint);
		vertex.push_back(verPoint);
		fscanf_s(fp, "%*1s");
		fscanf_s(fp, "%*1s");
	}
	printf("vertex_last:%f\n\n", vertex[vertexNum * 3 - 1]);


	//-------------------------------------------------------------------------
	// index
	//-------------------------------------------------------------------------

	/* インデックス数 */
	fscanf_s(fp, "%d", &indexNum);
	printf("indexNum:%d\n", indexNum);

	fscanf_s(fp, "%*1s");// 一文字読み飛ばし

	/* インデックス */
	for (int i = 0; i < indexNum; i++)
	{
		int num = 0;

		fscanf_s(fp, "%d", &num);// 3 or 4
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%hu", &indexPoint);
		index.push_back(indexPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%hu", &indexPoint);
		index.push_back(indexPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%hu", &indexPoint);
		index.push_back(indexPoint);
		fscanf_s(fp, "%*1s");
		fscanf_s(fp, "%*1s");
	}
	printf("index_last:%hu\n\n", index[indexNum * 3 - 1]);

	while (1)
	{
		fscanf_s(fp, "%s", buf, 256);

		if (strcmp(buf, "MeshNormals") == 0)break;
	}

	//-------------------------------------------------------------------------
	// normal
	//-------------------------------------------------------------------------
	fscanf_s(fp, "%*1s");

	/* 法線数 */
	fscanf_s(fp, "%d", &normalNum);
	printf("normalNum:%d\n", normalNum);

	fscanf_s(fp, "%*1s");// 一文字読み飛ばし

	/* 法線 */
	for (int i = 0; i < normalNum; i++)
	{
		fscanf_s(fp, "%f", &normalPoint);
		normal.push_back(normalPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%f", &normalPoint);
		normal.push_back(normalPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%f", &normalPoint);
		normal.push_back(normalPoint);
		fscanf_s(fp, "%*1s");
		fscanf_s(fp, "%*1s");
	}
	printf("normal_last:%f\n\n", normal[normalNum * 3 - 1]);

	while (c = getc(fp) != '}') {}

	while (1)
	{
		fscanf_s(fp, "%s", buf, 256);

		if (strcmp(buf, "MeshTextureCoords") == 0)break;
	}

	//-------------------------------------------------------------------------
	// texture
	//-------------------------------------------------------------------------
	fscanf_s(fp, "%*1s");

	/* テクスチャ数 */
	fscanf_s(fp, "%d", &texNum);
	printf("texNum:%d\n", texNum);

	fscanf_s(fp, "%*1s");// 一文字読み飛ばし

	/* テクスチャ */
	for (int i = 0; i < texNum; i++) {
		fscanf_s(fp, "%f", &texPoint);
		texture.push_back(texPoint);
		fscanf_s(fp, "%*1s");

		fscanf_s(fp, "%f", &texPoint);
		texture.push_back(texPoint);
		fscanf_s(fp, "%*1s");
		fscanf_s(fp, "%*1s");
	}
	printf("tex_last:%f\n\n", texture[texNum * 2 - 1]);


	fclose(fp);
}


struct RGB {
	unsigned char r, g, b;
};


void load_image_bmp(const char * _FileName)
{
	FILE *pFile;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	RGB *pixels;


	fopen_s(&pFile, _FileName, "rb");

	if (pFile == NULL)
	{
		printf("mapが　ありません！\n");
		assert(NULL);
	}

	fread(&bf, sizeof(bf), 1, pFile);// ファイルヘッダ
	fread(&bi, sizeof(bi), 1, pFile);// 情報ヘッダ

	pixels = (RGB*)malloc(bi.biHeight*bi.biWidth * sizeof(RGB));

	fread(pixels, bi.biHeight*bi.biWidth * sizeof(RGB), 1, pFile);// ピクセルデータ読み込み
	fclose(pFile);


	/* rとbのソート */
	unsigned char escape_bit;
	for (int i = 0; i < bi.biHeight * bi.biWidth; i++)
	{
		escape_bit = pixels[i].b;
		pixels[i].b = pixels[i].r;
		pixels[i].r = escape_bit;
	}

	/* 上下反転ソート */
	RGB escape_rgb;
	for (int y = 0; y < bi.biHeight / 2; y++)
	{
		for (int x = 0; x < bi.biWidth; x++)
		{
			escape_rgb = pixels[y*bi.biWidth + x];
			pixels[y*bi.biWidth + x] = pixels[((bi.biHeight - 1) - y)*bi.biWidth + x];
			pixels[((bi.biHeight - 1) - y)*bi.biWidth + x] = escape_rgb;
		}
	}


	/* ピクセルデータを VRAM に転送 */
	glTexImage2D(
		GL_TEXTURE_2D,      // GLenum target
		0,                  // GLint level
		GL_RGB,             // GLint internalformat
		bi.biWidth,
		bi.biHeight,// GLsizei width, height
		0,                  // GLint border
		GL_RGB,             // GLenum format
		GL_UNSIGNED_BYTE,   // GLenum type
		pixels);// const GLvoid *pixels
}


void timer(int value)
{
	yaw++;

	glutPostRedisplay();

	glutTimerFunc(1000 / 60, timer, 0);
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
		0, 2, 5,  // eye
		0, 0, 0,  // center
		0, 1, 0); // up


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glTexParameteri(
		GL_TEXTURE_2D,          // GLenum target
		GL_TEXTURE_MIN_FILTER,  // GLenum pname
		GL_NEAREST);            // GLint param

	glTexParameteri(
		GL_TEXTURE_2D,        // GLenum target
		GL_TEXTURE_MAG_FILTER,// GLenum pname
		GL_NEAREST);          // GLint param


	auto v_it = vertex.begin();
	auto i_it = index.begin();
	auto n_it = normal.begin();
	auto t_it = texture.begin();


	glRotatef(yaw, 0, 1, 0);
	//glScalef(0.1f, 0.1f, 0.1f);


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &(*v_it));

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, &(*n_it));

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &(*t_it));

	glEnable(GL_TEXTURE_2D);


	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_SHORT, &(*i_it));


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);


	glFlush();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("title");

	load_x("koura_g.x");
	load_image_bmp("tex00.bmp");

	glutDisplayFunc(display);

	glutTimerFunc(0, timer, 0);

	glutMainLoop();
}