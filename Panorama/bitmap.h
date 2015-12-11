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
	GLuint w, h; //ͼ����
	GLuint line, size; //һ�е��ֽ��������ֽ���
	GLubyte *a; //λͼ��BGR����

	bitmap(){}
	bitmap(GLuint width, GLuint height); // ��ָ����ߵĹ��캯����������line, size��Ϊa��������ռ�
	bitmap(const char* fileName); // ����һ��bmp������bitmap��
	GLubyte* getPixel(int x,int y); //���ĳ�������ص�ַ
	GLubyte* bitmap :: operator ()(const GLuint &x, const GLuint &y);
	void display(GLdouble y, GLdouble x); // ����glDrawPixels����Ļ����ʾͼ��
	static void interp(GLubyte* out,bitmap& in,GLdouble x,GLdouble y);//˫���Բ�ֵ
	static void interp3(GLubyte* out,bitmap& in,GLdouble x,GLdouble y);//lambda��Ѳ�ֵ
	static GLdouble S(GLdouble x);//��Ѳ�ֵ���ú���S()
};

#endif