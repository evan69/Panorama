#ifndef BITMAP_H
#define BITMAP_H
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <gl/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

struct bitmap{
	GLuint w, h; //图像宽高
	GLuint line, size; //一行的字节数和总字节数
	GLubyte *a; //位图的BGR数组

	bitmap(){}
	bitmap(GLuint width, GLuint height); // 仅指定宽高的构造函数，将计算line, size并为a数组申请空间
	bitmap(const char* fileName); // 读入一个bmp并构造bitmap类
	GLubyte* getPixel(int x,int y); //获得某处的像素地址
	GLubyte* bitmap :: operator ()(const GLuint &x, const GLuint &y);
	void display(GLdouble y, GLdouble x); // 利用glDrawPixels在屏幕上显示图像
	static void interp(GLubyte* out,bitmap& in,GLdouble x,GLdouble y);//双线性插值
	static void interp3(GLubyte* out,bitmap& in,GLdouble x,GLdouble y);//lambda最佳插值
	static GLdouble S(GLdouble x);//最佳插值所用函数S()
};

#endif