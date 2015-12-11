#include <Windows.h>
#include "bitmap.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <gl/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

//#define INTERP3
//�Ƿ����lambda��Ѳ�ֵ���۲���������Ч�����ѣ�ѡ��ʹ��˫���Բ�ֵ

GLuint WIDTH , HEIGHT; // ���ڵĳ��Ϳ�
const GLdouble PI = 3.1415926535897; 
char* bmpfile = "hw.bmp";//�ļ���

bitmap input, output; // ȫ��ͼ�ͱ任���λͼ
GLdouble pos = 0.5 ; // ��¼�۲�ĽǶȣ���(0,1)֮��
const GLdouble pos_step = 0.01; // ����A��D��ʱpos�ı仯��
const GLdouble theta_m = 35.0; // ˮƽ����İ�۲��
GLdouble R; // ����ȫ��ͼ�����ڰ뾶ΪR��Բ����

void transform()
{
	int cnt = 0;
	for(GLuint i = 0;i < output.w;i++)
	{
		for(GLuint j = 0;j < output.h;j++)
		{
			GLdouble rx = (GLdouble)i - ((GLdouble)output.w) / 2;
			GLdouble midpos = input.w * pos;
			GLdouble theta = atan(rx / R);
			GLdouble delta = R * theta;
			GLdouble ri = midpos + delta;
			//���϶�ԭͼ����Cylindrical panorama�������Ӧ������
			#ifdef INTERP3
			if(ri > 5 && ri < input.w - 5 && j > 5 && j < input.h) 
				bitmap::interp3(output.getPixel(i,j),input,ri,j);
			//lambda��Ѳ�ֵ
			else 
			#endif
			bitmap::interp(output.getPixel(i,j),input,ri,j);
			//���ڷ��������˫���Բ�ֵ
		}
	}
}

void init()//��ʼ��
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	input = bitmap(bmpfile);
	//��ȡbmp
	R = input.w / (2 * PI);
	//����Բ���뾶
	GLuint w = ((GLuint)(R * tan(theta_m / 180 * PI)) - 4) / 4 * 4;
	//��������ͼ��Ŀ�
	HEIGHT = input.h;
	WIDTH = 2 * w;
	//������Ļ�ĳ���
	output = bitmap(2 * w,input.h - 1);
	transform();
}

void display()
//��ʾͼ��
{
	output.display(-1,-1);
}

void keyboard(unsigned char c,int x,int y)
//��������¼�
{
	if(c == 27)
		exit(0);
	if(c == 'a' || c == 'A') pos -= pos_step;
	if(c == 'd' || c == 'D') pos += pos_step;
	if(pos > 1.0) pos -= 1.0;
	if(pos < 0.0) pos += 1.0;
	//������������
	transform();
	//��Cylindrical panorama
	glutPostRedisplay();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	if(argc == 1) bmpfile = "hw.bmp";//Ĭ��ѡ��hw.bmp
	else bmpfile = argv[1];//����еڶ������������������bmp�ļ���
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	init();
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cylindrical Panorama by evan69");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//����ע��ص�����
	glutMainLoop();
	return 0;
}
