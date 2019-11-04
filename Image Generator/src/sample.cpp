/********************************/
/* image processing framework   */
/* (C) Bedrich Benes 2018       */
/* bbenes ~ at ~ purdue.edu     */
/********************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;


const unsigned int MAX = 728;
unsigned char image[MAX][MAX][3];


//the main window size
GLint wWindow=MAX+20;
GLint hWindow=MAX+20;


//update intervals
time_t lasttime = 0;
time_t DELAY = 1;


void CalcImage();

/*********************************
Some OpenGL-related functions
**********************************/

//called when a window is reshaped
void Reshape(int w, int h)
{
  glViewport(0,0,w, h);       
  glEnable(GL_DEPTH_TEST);
//remember the settings for the camera
  wWindow=w;
  hWindow=h;
}


//the main rendering function
void RenderObjects()
{
	glColor3ub(225, 225, 225);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1, -1);
	glTexCoord2f(1, 0); glVertex2f(-1,  1);
	glTexCoord2f(1, 1); glVertex2f( 1,  1);
	glTexCoord2f(0, 1); glVertex2f( 1, -1);
	glEnd();
}

void Idle(void)
{
  glClearColor(0,0,1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  RenderObjects();
  glutSwapBuffers();
  if (lasttime + DELAY < time(0)) {
	  lasttime = time(0);
	  glutPostRedisplay();
  }
}

void Display(void)
{
	CalcImage();
	unsigned int texture;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.2, 1.2, -1.2, 1.2, -1, 1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MAX, MAX, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
}

void Kbd(unsigned char a, int x, int y)//keyboard callback
{
	switch(a)
	{
 	  case 27 : exit(0);break;
	}
	glutPostRedisplay();
}

void SetPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
	if ((x < 0) || (x > MAX) || (y < 0) || (y > MAX))
	{
		cout << "Pixel coordinates are out of range for the image size!" << x << " " << y << endl;
		return;
	}
	image[x][y][0] = red;
	image[x][y][1] = green;
	image[x][y][2] = blue;

}


//this is the function you need to edit

void CalcImage() {
	cerr << "tst";
	for (int i = 0; i < MAX; i++) {
//white line
		SetPixel(i,i,255,255,255);
//red line
		SetPixel(i, MAX - i - 1, 255, 0, 0);
//blue line
		SetPixel(i, MAX / 2, 0, 0, 255);
//yellow line
		SetPixel(MAX / 2, i, 255, 255, 0);
	}

	for (float t = 0; t < 2 * 3.14; t += 0.01)
		SetPixel(MAX / 2 + MAX / 2 * sin(t), MAX / 2 + MAX / 2 * cos(t), 255, 255, 255);
}


int main(int argc, char **argv)
{ 
	CalcImage();

	glutInitDisplayString("stencil>=2 rgb double depth samples");
	glutInit(&argc, argv);
	glutInitWindowSize(wWindow,hWindow);
	glutInitWindowPosition(500,100);
	glutCreateWindow("Mandel");
	GLenum err = glewInit();
	if (GLEW_OK != err){
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Kbd); //+ and -
	glutMouseFunc(NULL);
	glutSpecialUpFunc(NULL); 
	glutSpecialFunc(NULL);
	glutMainLoop();
	return 0;        
}
