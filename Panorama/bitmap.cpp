#include "bitmap.h"
#include <assert.h>

bitmap::bitmap(GLuint width, GLuint height){
	//生成空的图像
	w = width, h = height;
	line = width;
	while(line % 4 != 0)
	{
		line++;
	}
	line = line * 3;
	size = line * h;
	a = new GLubyte[size];
	//分配内存
}

bitmap::bitmap(const char* fileName){
	//读取bmp文件并构造bitmap
	FILE* pFile;
	fopen_s(&pFile, fileName, "rb");
	if( pFile == 0 )
	{
		printf("Warning! Cannot find the bmp file and the procedure will not work.");
		system("pause");
		exit(0);
	}

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&w, sizeof(w), 1, pFile);
	fread(&h, sizeof(h), 1, pFile);

	for(line = w * 3; line % 4 != 0; ++line)
		;
	size = line * h;

	if(a = (GLubyte*) malloc(size), a == NULL)
		exit(0);
	fseek(pFile, 54, SEEK_SET);
	fread(a, size, 1, pFile);

	fclose(pFile);
}

GLubyte* bitmap::getPixel(int x,int y)
{
	while(x * 3 > line) x -= this->w;
	while(x * 3 < 0) x += this->w; 
	return a + y * line + x * 3;
}

GLubyte* bitmap :: operator ()(const GLuint &x, const GLuint &y){
	return a + y * line + x * 3;
}

void bitmap::display(GLdouble x,GLdouble y)
{
	//屏幕上绘制
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2d(x, y);
	glDrawPixels(w,h,GL_BGR_EXT,GL_UNSIGNED_BYTE,a);
	glutSwapBuffers();
}

void bitmap::interp(GLubyte* out,bitmap& in,GLdouble x,GLdouble y)
//线性插值算法
{
	while(x < 0.0) x += in.w;
	while(x > in.w) x -= in.w;
	while(y < 0.0) y += in.h;
	while(y > in.h) y -= in.h;
	int Left = (int)x;
	int Bottom = (int)y;
	GLdouble dx = x - Left;
	GLdouble dy = y - Bottom;
	GLubyte* LU = in.getPixel(Left,Bottom+1);
	GLubyte* LB = in.getPixel(Left,Bottom);
	GLubyte* RU = in.getPixel(Left+1,Bottom+1);
	GLubyte* RB = in.getPixel(Left+1,Bottom);
	GLdouble L,R;
	L = LB[0] * (1 - dy) + LU[0] * dy;
	R = RB[0] * (1 - dy) + RU[0] * dy;
	out[0] = L * (1 - dx) + R * dx;
	L = LB[1] * (1 - dy) + LU[1] * dy;
	R = RB[1] * (1 - dy) + RU[1] * dy;
	out[1] = L * (1 - dx) + R * dx;
	L = LB[2] * (1 - dy) + LU[2] * dy;
	R = RB[2] * (1 - dy) + RU[2] * dy;
	out[2] = L * (1 - dx) + R * dx;
}

GLdouble bitmap::S(GLdouble x)
{
	if(x < 0.0) x = -x;
	GLdouble k = x - 2.0 * x * x + x * x * x;
	GLdouble lambda = 0.5;
	if(0.0 <= x && x < 1.0) 
		return (1.0 - x) + lambda * k;
	if(1.0 <= x && x < 2.0) 
	{
		x = x - 1.0;
		k = x - 2.0 * x * x + x * x * x;
		return - lambda * k;
	}
	return 0.0;
}

void bitmap::interp3(GLubyte* out,bitmap& in,GLdouble x,GLdouble y)
{
	
	while(x < 0.0) x += in.w;
	while(x > in.w) x -= in.w;
	while(y < 0.0) y += in.h;
	while(y > in.h) y -= in.h;
	int Left = (int)x;
	int Bottom = (int)y;
	if(Left < 1 || Left + 3 > in.w) return;
	if(Bottom < 1 || Bottom + 3 > in.h) return;
	GLdouble dx = x - (GLdouble)Left;
	GLdouble dy = y - (GLdouble)Bottom;

	GLdouble a[1][4] = {{S(1.0 + dy),S(dy),S(1.0 - dy),S(2.0 - dy)}};

	GLdouble b[3][4][4] = {
		{
		{in.getPixel(Left-1,Bottom-1)[0],in.getPixel(Left-1,Bottom)[0],
			in.getPixel(Left-1,Bottom+1)[0],in.getPixel(Left-1,Bottom+2)[0]},
		{in.getPixel(Left,Bottom-1)[0],in.getPixel(Left,Bottom)[0],
			in.getPixel(Left,Bottom+1)[0],in.getPixel(Left,Bottom+2)[0]},
		{in.getPixel(Left+1,Bottom-1)[0],in.getPixel(Left+1,Bottom)[0],
			in.getPixel(Left+1,Bottom+1)[0],in.getPixel(Left+1,Bottom+2)[0]},
		{in.getPixel(Left+2,Bottom-1)[0],in.getPixel(Left+2,Bottom)[0],
			in.getPixel(Left+2,Bottom+1)[0],in.getPixel(Left+2,Bottom+2)[0]}
		}
		,
		{
		{in.getPixel(Left-1,Bottom-1)[1],in.getPixel(Left-1,Bottom)[1],
			in.getPixel(Left-1,Bottom+1)[1],in.getPixel(Left-1,Bottom+2)[1]},
		{in.getPixel(Left,Bottom-1)[1],in.getPixel(Left,Bottom)[1],
			in.getPixel(Left,Bottom+1)[1],in.getPixel(Left,Bottom+2)[1]},
		{in.getPixel(Left+1,Bottom-1)[1],in.getPixel(Left+1,Bottom)[1],
			in.getPixel(Left+1,Bottom+1)[1],in.getPixel(Left+1,Bottom+2)[1]},
		{in.getPixel(Left+2,Bottom-1)[1],in.getPixel(Left+2,Bottom)[1],
			in.getPixel(Left+2,Bottom+1)[1],in.getPixel(Left+2,Bottom+2)[1]}
		}
		,
		{
		{in.getPixel(Left-1,Bottom-1)[2],in.getPixel(Left-1,Bottom)[2],
			in.getPixel(Left-1,Bottom+1)[2],in.getPixel(Left-1,Bottom+2)[2]},
		{in.getPixel(Left,Bottom-1)[2],in.getPixel(Left,Bottom)[2],
			in.getPixel(Left,Bottom+1)[2],in.getPixel(Left,Bottom+2)[2]},
		{in.getPixel(Left+1,Bottom-1)[2],in.getPixel(Left+1,Bottom)[2],
			in.getPixel(Left+1,Bottom+1)[2],in.getPixel(Left+1,Bottom+2)[2]},
		{in.getPixel(Left+2,Bottom-1)[2],in.getPixel(Left+2,Bottom)[2],
			in.getPixel(Left+2,Bottom+1)[2],in.getPixel(Left+2,Bottom+2)[2]}
		}
		};

	GLdouble c[4][1] = {{S(1 + dx)},{S(dx)},{S(1 - dx)},{S(2 - dx)}};
	GLdouble tmp = (GLdouble)0.0;
	GLdouble arr[] = {0.0,0.0,0.0,0.0};
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 4;j++)
		{
			for(int k = 0;k < 4;k++)
			{
				arr[j] += a[0][k] * b[i][k][j];
			}
		}
		for(int l = 0;l < 4;l++)
		{
			tmp += arr[l] * c[l][0];
		}
		out[i] = (GLubyte)tmp;
		tmp = 0.0;
		for(int m = 0;m < 4;m++)
		{
			arr[m] = 0.0;
		}
	}
}