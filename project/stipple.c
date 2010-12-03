// Example #8: Create Stippling Pattern
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
int GAngle=0,i;

void Display(void)
	{
	// Clear Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glRotated(GAngle,0,1,0);

	glBegin(GL_POLYGON);
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

int main(int argc, char **argv) {
    	glutInit(&argc,argv);
	// Init to double buffering
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow("This is the window title");
	glutDisplayFunc(Display);
	glutTimerFunc(0,Timer,0);

	// Create random Stippling Pattern
	GLubyte data[32*4];

	for(i=0;i<32*4;++i)
		{  data[i]=rand();  }

	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(data);
	glColor3f(1,0,0);

	glutMainLoop();
	return 0;
	}

