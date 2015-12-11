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
//是否进行lambda最佳插值，观察结果发现其效果不佳，选择使用双线性插值

GLuint WIDTH , HEIGHT; // 窗口的长和宽
const GLdouble PI = 3.1415926535897; 
char* bmpfile = "hw.bmp";//文件名

bitmap input, output; // 全景图和变换后的位图
GLdouble pos = 0.5 ; // 记录观察的角度，在(0,1)之间
const GLdouble pos_step = 0.01; // 按下A和D键时pos的变化量
const GLdouble theta_m = 35.0; // 水平方向的半观察角
GLdouble R; // 假设全景图被贴在半径为R的圆柱上

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
			//以上对原图像作Cylindrical panorama，计算对应的坐标
			#ifdef INTERP3
			if(ri > 5 && ri < input.w - 5 && j > 5 && j < input.h) 
				bitmap::interp3(output.getPixel(i,j),input,ri,j);
			//lambda最佳插值
			else 
			#endif
			bitmap::interp(output.getPixel(i,j),input,ri,j);
			//对于非整点采用双线性插值
		}
	}
}

void init()//初始化
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	input = bitmap(bmpfile);
	//读取bmp
	R = input.w / (2 * PI);
	//计算圆柱半径
	GLuint w = ((GLuint)(R * tan(theta_m / 180 * PI)) - 4) / 4 * 4;
	//计算生成图像的宽
	HEIGHT = input.h;
	WIDTH = 2 * w;
	//计算屏幕的长宽
	output = bitmap(2 * w,input.h - 1);
	transform();
}

void display()
//显示图像
{
	output.display(-1,-1);
}

void keyboard(unsigned char c,int x,int y)
//处理键盘事件
{
	if(c == 27)
		exit(0);
	if(c == 'a' || c == 'A') pos -= pos_step;
	if(c == 'd' || c == 'D') pos += pos_step;
	if(pos > 1.0) pos -= 1.0;
	if(pos < 0.0) pos += 1.0;
	//处理上溢下溢
	transform();
	//作Cylindrical panorama
	glutPostRedisplay();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	if(argc == 1) bmpfile = "hw.bmp";//默认选择hw.bmp
	else bmpfile = argv[1];//如果有第二个参数，则把它当做bmp文件名
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	init();
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cylindrical Panorama by evan69");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//以上注册回调函数
	glutMainLoop();
	return 0;
}
