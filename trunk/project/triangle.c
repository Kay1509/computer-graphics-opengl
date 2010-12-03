//////////////////////////////////////////////////////
// Example #1: Rotating Triangle
//////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
int GAngle=0;

void Display(void)
	{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glRotated(GAngle,0,1,0);

	glBegin(GL_TRIANGLES);
		glVertex3f(-1,0,0);
		glVertex3f(1,0,0);
		glVertex3f(0,1,0);
	glEnd();

	GAngle = GAngle + 1;

	glFlush();

	glutSwapBuffers();
	}

void Timer(int extra)
	{
	glutPostRedisplay();
	glutTimerFunc(30,Timer,0);
	}

int main(int argc, char **argv)
	{
	// Init to double buffering
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow("This is the window title");
	glutDisplayFunc(Display);
	glutTimerFunc(0,Timer,0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,1,3);
	glTranslated(0,0,-2);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
	return 0;
	}
