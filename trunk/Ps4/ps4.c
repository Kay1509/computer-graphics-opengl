/*
 * Computer Graphics:  Andrew Hanson, Indiana University
 * GLUT  Graphics Exercise Template 
 *

 cc -o template.linux template.c -lglut -lGLU -lGL -lm

 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

/* Local Graphics Definitions. Also in /l/b481/include, /l/b581/include */
#include "defs.h"

/*  double y0,y1;   math.h conflict */  

/***************************************************************/
/******************** MY GLOBAL VARIABLES **********************/
/***************************************************************/

int mouseButtons = 0;

int MouseModifiers = 0,bugX=100,bugY=100;

int  X=0, Y=0, dX=0, dY=0,mode =0,flag = 0,fixX=0,fixY=0,dragX,dragY,realX,realY,transX,transY,rand1,dot =0, dotCount=0,spaceCount = 0;

int main_window,i,j,color;
int main_height, main_width;
float mat[16],scaleX,scaleY;
GLint viewport[4];
GLdouble mvmatrix[16],projmatrix[16];
GLdouble wx,wy,wz;
GLint realY;
GLint XformX, XformY;

/*void drawCurrentState() {

glRectf(100,100,200,200);

}*/
/***************************************************************/
/*********************** MOUSE CALLBACKS ***********************/
/***************************************************************/

/* Mouse Button  UP/DOWN  or DOWN/UP transition only */
void
mouse(int button, int state, int x, int y)
{
  X = x;
  Y =  main_height - y - 1;	/* Invert to right-handed system */
  button = (1 << button);
  
  MouseModifiers = glutGetModifiers();
  /* bitmask with GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT */

  	if (state == GLUT_DOWN){
  	mouseButtons |= button;
	switch(mode) {
	case 1:
		fixX = X;
		fixY = Y;
		break;
	case 2:
		fixX = X;
		fixY = Y;
		break;	
	case 3:
		fprintf(stderr,"\n\nInside case 3 of mouse\n\n");
	default:
		fprintf(stderr,"\n\nUnhandled Mode\n\n");
	}
	}
 	else {
    	mouseButtons &= (~(button));
	/*switch(mode) {
	case 1:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		drawCurrentState();
		glPopMatrix();
		break;
		
	default:
		fprintf(stderr,"\n\nUnhandled Mode\n\n");
	}*/


	}

 
     
  fprintf(stderr,"Comment this out: mouse buttons = [%d], modifiers = [%d] \n\
          state = %s, mouse at (%d,%d)\n",
	  mouseButtons, MouseModifiers, 
          state == GLUT_UP? "Just Up":"Just Down",x,y); 
 
  glutPostRedisplay(); 
}

void
drawFixedPoint(GLint x, GLint y){
switch(mode) {
case 1 :case 2:
	glColor3f(0.0,0.0,1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
	break;
case 3:
	if(dotCount % 2 ==0) glColor3f(0.0,0.0,1.0);
	else glColor3f(1.0,0.0,0.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
	break;
default:
	glColor3f(0.0,0.0,1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
	
}
}

void 
drawLine(){

glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1.0,0.0,0.0);
drawFixedPoint(dragX,dragY);
fprintf(stderr,"\n\n****DOT**** = %d\n\n", dot);
	if(dot == 1) {
	fprintf(stderr,"\n\n****DOT**** = %d\n\n", dot);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	gluUnProject((GLdouble)transX, (GLdouble)transY, 0.0,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);

	realX =  wx;
	realY =  wy;

	drawFixedPoint(realX,realY);
	glBegin(GL_LINES);
	glVertex2f(main_width/2,main_height/2);
	glVertex2f(realX,realY);
	glEnd();
	}

drawFixedPoint(transX,transY);
glBegin(GL_LINES);
	glVertex2f(main_width/2,main_height/2);
	glVertex2f(transX,transY);
glEnd();
//glMatrixMode(GL_MODELVIEW);
//glPopMatrix();
}
/* Mouse motion WHILE BUTTON DOWN  */
/*  COMPLAINS  if you call glutGetModifiers()  here!! */
void
motion(int x, int y)
{
  fprintf(stderr,"Comment this out: mouse at (%d,%d)\n\
          mouse buttons = [%d], modifiers = [%d]\n",
	  x,y, mouseButtons, MouseModifiers);  
  X = x;
  Y =  main_height - y - 1;	/* Invert to right-handed system */


  switch(mouseButtons ){
  	case 0x1:  /* Left Mouse Button   */
  	switch(mode){
	case 1:
		dragX = X;
		dragY = Y;	
		break;
	case 2:
		dragX = X;
		dragY = Y;
		break;	
	case 3:
		//realX = X;
		//realY = Y;
		transX = X;
		transY = Y;
		dragX = X;
		dragY = Y;
		
	default:
		fprintf(stderr,"\n\nUnhandled mouseButton\n\n"); 
	}
    	break;
  	case 0x2:  /* Middle Mouse Button */
    	break;
  	case 0x4:  /* Right Mouse Button  */
    	break;
  }
  glutPostRedisplay();

}

/* Mouse motion WHILE ALL BUTTONS UP  */
/*  COMPLAINS  if you call glutGetModifiers()  here!! */
void
passive(int x, int y)
{
  X = x;
  Y = main_height - y - 1;

  glutPostRedisplay();
}

void 
replaceXY(){

transX = dragY;
transY = dragX;
}
void
pickRandom(){

rand1 = (rand() % 7) +1;
	switch(rand1) {
	case 1:
		glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(90,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);
		//glPopMatrix();
		fprintf(stderr,"\n\n(x,y) = (-mx,my) Rotated by 90 degrees\n\n");
		break;
	case 2:
		glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(180,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);
		//glPopMatrix();
		fprintf(stderr,"\n\n(x,y) = (-mx,-my)Rotated by 180 degrees\n\n");
		break;
	case 3:
		glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(270,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);
		//glPopMatrix();
		fprintf(stderr,"\n\n(x,y) = (mx,-my)Rotated by 270 degrees\n\n");
		break;
	case 4:
		glMatrixMode(GL_MODELVIEW);
		replaceXY();
		/*glMatrixMode(GL_MODELVIEW);
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(90,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);*/
		fprintf(stderr,"\n\n(x,y) = (my,mx)Rotated by 0 degrees\n\n");
		break;
	case 5:
		
		glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		replaceXY();
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(90,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);
		//glPopMatrix();
		fprintf(stderr,"\n\n(x,y) = (-my,mx)Rotated by 90 degrees\n\n");
		break;
	case 6:
		
		glMatrixMode(GL_MODELVIEW);
		replaceXY();
		//glPushMatrix();
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(180,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);
		//glPopMatrix();
		fprintf(stderr,"\n\n(x,y) = (-my,-mx)Rotated by 180 degrees\n\n");
		break;
	case 7:
		
		glMatrixMode(GL_MODELVIEW);
		replaceXY();
		//glPushMatrix();
		glTranslatef(main_width/2.0,main_height/2.0,0.0);
		glRotatef(270,0.0,0.0,1.0);
		glTranslatef(-main_width/2.0,-main_height/2.0,0.0);
		//glPopMatrix();
		fprintf(stderr,"\n\n(x,y) = (my,-mx)Rotated by 270 degrees\n\n");
		break;
	default:
		fprintf(stderr,"\n\nUnhandled Mode %d\n\n",rand1);
	
	}
}
/* Handle Keyboard */
void
keyboard(unsigned char key, int x, int y)
{
  /*  Option: To force all lower case */
  /* if (key >= 'A' && key <= 'Z') key = (key-'A') + 'a'; */

  switch (key) {
  case 'h' :     fprintf(stderr,"Help: goes here.\n");
    break;
  case 'q' : case 27 :  /* 27 -> ESC */
    fprintf(stderr,"Normal Exit.\n");
    exit(EXIT_SUCCESS);
    break;
   case 'a':
	if(mode == 3){
	if(dotCount++%2 == 0) dot = 1;
	else dot =0;
	}
	break;
    case 32:
	if(mode == 3){
	pickRandom();
	spaceCount++;
	}
	break;	
    case 49:  
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	fprintf(stderr, "\n\nMode 1 Entered\n\n");
	flag = 1;
	mode = 1;
	break;	
    case 50:  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	fprintf(stderr, "\n\nMode 2 Entered\n\n");
	flag = 1;
	mode = 2;
	break;	
    case 51 :
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	fprintf(stderr,"\n\nMode 3 Entered\n\n");
	mode = 3;
	drawFixedPoint(main_width/2,main_height/2);
	break;
   case 52:
    mode = 4;
    fprintf(stderr,"\n\nMode 4 Entered\n\n");	
    break;
  case 'r':
	if(mode == 1) {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}
	break;
  default: fprintf(stderr,"Unhandled key: %c [%d] \n",key,key);
  }

  glutPostRedisplay();
}


/***************************************************************/
/************************** INIT GL ****************************/
/***************************************************************/

void
initGL()
{
  glClearColor(0.7, 0.8, 0.9, 0.0);
}




/***************************************************************/
/************************ DRAW STUFF ***************************/
/***************************************************************/


void
drawObjs()
{
  glColor3f(0.9,0.8,0.4);
  glRecti(200,50,300,450); 

  glColor3f(1.0,0.0,1.0);
  glPointSize(5.0);
  glBegin(GL_POINTS);
  glVertex2i(X,Y);
  glEnd();

}



/***************************************************************/
/************************ GLUT STUFF ***************************/
/***************************************************************/


void
reshape(int w, int h)
{
  fprintf(stderr,"Comment this out: reshape(%d,%d) \n",w,h);
  w = Max(w,128);
  h = Max(h,128);
  main_height = h;
  main_width = w;
  glutReshapeWindow(w,h);
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0.0,(double)w-1,0.0,(double)h-1); 

  glMatrixMode(GL_MODELVIEW);
}

void
visibility(int state)
{
  /* May also want to check PARTIAL DAMAGE that
     does not result in a call to the visibility callback:
      if (glutLayerGet(GLUT_NORMAL_DAMAGED)) {
       redraw = TRUE; }
       */
  fprintf(stderr,"Comment this out: visibility: %s\n",
	  (state==GLUT_VISIBLE)?"visible":"invisible");
  if (state == GLUT_VISIBLE){
    glutPostRedisplay();
  }
}

void
drawTeapot(){

glClear(GL_COLOR_BUFFER_BIT);
fprintf(stderr,"\n\nInside drawTeapot\n\n");
glColor3f(1.0,0.0,0.0);
glutWireTeapot(0.5);

}





void
drawGrid(){
 color=0; 			
  for(j=120;j<=360;j=j+40)
	for(i=120;i<=360;i=i+40){
		if(color++%2 ==0) glColor3f(1.0,0.0,0.0);
		else  glColor3f(0.0,1.0,0.0);
		glRecti(i,j,i+40,j+40);  	
	}
	
}
void
InteractiveMethods1(){
glGetIntegerv(GL_VIEWPORT,viewport);
glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
switch(mode) {
case 1:
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.9,0.8,0.7);
	XformX = fixX;
	XformY = fixY;
		if (flag) {
	//glRectf(200,200,400,400);
		drawGrid();
		flag = 0;
		}
		else {
	
		if (dragX > fixX) scaleX = 1.0 + ((float)dragX/(float)fixX/(float)main_width); 
		if (dragX < fixX) scaleX = 1.0 - ((float)dragX/(float)fixX/(float)main_width);	
		if (dragY > fixY) scaleY = 1.0 + ((float)dragY/(float)fixY/(float)main_height);
		if (dragY < fixY) scaleY = 1.0 - ((float)dragY/(float)fixY/(float)main_height);	 
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
		glTranslatef(fixX,fixY,0.0);
		glScalef(scaleX,scaleY,1.0);
		glTranslatef(-fixX,-fixY,0.0);
	//glRectf(200,200,400,400);
		drawGrid();
	
	//glPopMatrix();
		}
	break;
case 2:
	gluUnProject((GLdouble)fixX, (GLdouble)fixY, 0.0,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
	XformX = wx;
	XformY = wy;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.9,0.8,0.7);
		if (flag) {
	//glRectf(200,200,400,400);
		drawGrid();
		flag = 0;
		}
		else {
	
		if (dragX > fixX) scaleX = 1.0 + ((float)dragX/(float)fixX/(float)main_width); 
		if (dragX < fixX) scaleX = 1.0 - ((float)dragX/(float)fixX/(float)main_width);	
		if (dragY > fixY) scaleY = 1.0 + ((float)dragY/(float)fixY/(float)main_height);
		if (dragY < fixY) scaleY = 1.0 - ((float)dragY/(float)fixY/(float)main_height);	 
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
		glTranslatef(fixX,fixY,0.0);
		glScalef(scaleX,scaleY,1.0);
		glTranslatef(-fixX,-fixY,0.0);
	//glRectf(200,200,400,400);
		drawGrid();
	
	//glPopMatrix();
		}
	break;
default:
	fprintf(stderr,"\n\nUnhandled mode %d\n\n", mode);
}
drawFixedPoint(XformX,XformY);
}
void 
drawBug(){

glColor3f(1.0,1.0,0.0);

	if(spaceCount > 0) {
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	gluUnProject((GLdouble)bugX, (GLdouble)bugY, 0.0,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
	bugX = wx;
	bugY = wy; 
	}

	glBegin(GL_QUADS);
	glVertex2i(bugX,bugY);
	glVertex2i(bugX+100,bugY);
	glVertex2i(bugX+100,bugY+100);
	glVertex2i(bugX,bugY+100);
	glEnd();
	

}
void
display(void)
{

 fprintf(stderr,"Comment this out: display\n");

  

 	switch(mode) {
	case 1:
		fprintf(stderr,"\n\nInside Case 1\n\n");
		InteractiveMethods1();
		break;
	case 2:
		fprintf(stderr,"\n\nInside Case 2\n\n");
		InteractiveMethods1();
		break;
	case 3:

		drawLine();
		drawFixedPoint(main_width/2,main_height/2);
		drawBug();

		break;
	case 4:
	fprintf(stderr,"\nInside Case 4\n");
	drawTeapot();
	fprintf(stderr,"\nAfter returning from drawTeapot\n");
	break;
 
	default:
	fprintf(stderr,"\n\nUnhandled mode %c [%d]\n\n", mode, mode); 


	}
  //drawObjs();


  glutSwapBuffers();
}



/***************************************************************/
/**************************** MAIN *****************************/
/***************************************************************/


int
main(int argc, char **argv)
{
  /* GLUT Initialization */
  glutInit(&argc,argv);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  /* Create a new window and give a title to it */
  main_window = glutCreateWindow("CHANGE THIS TITLE!!!!!!!");

  /* Initialize OpenGL stuff */
  initGL();

  /* Post Event Handlers */
  glutReshapeFunc(reshape);
  glutVisibilityFunc(visibility);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(NULL);
    
  glutKeyboardFunc(keyboard);
  fprintf(stderr,"Press ESC or 'q' to Exit.\n");

  glutIdleFunc(NULL);

  glutMainLoop();

  return(EXIT_SUCCESS);
}
