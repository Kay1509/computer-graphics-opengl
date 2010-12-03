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

int MouseModifiers = 0;
int  X=0, Y=0,L,R,L1,R1,flag=-1,op=1,mode=0,Xr=100,Yr=100,Xg=200,Yg=200,Xb=300,Yb=300,Xc=100,Yc=100,Xm=200,Ym=200,Xy=300,Yy=300,Xi,Yi,i,l,m;
float z[500],k,w,b1, dX=0.0, dY=0.0,p[500];
int main_window;
int main_height, main_width;

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
	
	L=X;R=Y;L1=X;R1=Y;
	Xi=X;Yi=Y;
	
	
}
	else if ( state == GLUT_UP){
	if(mode == 2)	
	{
	L1=X;R1=Y;
	
	}
	mouseButtons &= (~(button));
}
 else
    mouseButtons &= (~(button));

     
  fprintf(stderr,"Comment this out: mouse buttons = [%d], modifiers = [%d] \n\
          state = %s, mouse at (%d,%d)\n",
	  mouseButtons, MouseModifiers, 
          state == GLUT_UP? "Just Up":"Just Down",x,y); 
 
  glutPostRedisplay(); 
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
	if (mode == 1){
	glColor3f(0.6,0.8,0.4);
	glBegin(GL_LINES);
		glVertex3f (L, R, 0.0);
      		glVertex3f (L1, R1, 0.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f (L, R, 0.0);
      		glVertex3f (X, Y, 0.0);
	glEnd();
	L1=X;R1=Y;
}
	if (mode == 2 ){
	
	glColor3f(1.0,0.0,0.0);
	
	glRecti(Xr,Yr,Xr+75,Yr+75);
	glRecti(X,Y,X+75,Y+75);
	
	Xr=X;Yr=Y; 

}
	if (mode == 3 ){
	glColor3f(0.0,1.0,1.0);
	
	
	glRecti(Xc,Yc,Xc+75,Yc+75);
	glRecti(X,Y,X+75,Y+75);
	Xc=X;Yc=Y; 
}
	if (mode ==4 )	{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glPointSize(2);
	dX=(float)(X-Xi);dY=(float)(Y-Yi);
	if(dX == 0.0) dX = 0.01;
	p[0]=2*dY-dX;
	p[1]=p[0]+2*dY;
	k=X-Xi;
	w=(dY/dX);
	b1=(float)(Yi-w*Xi);
	glBegin(GL_POINTS);
	glVertex2f((float)Xi,(float)Yi);
	glEnd();
	for(i=1;i<k;i++)
	z[i]=w*(Xi+i)+b1;	
	for(i=1;i<k;i++) {
	if(p[i] <0) { 
		p[i+1]=p[i]+2*dY; 
		glBegin(GL_POINTS);
		glColor3f(1.0,1.0,1.0);
			glVertex2f((float)(Xi+i),z[i]);
		glEnd();
	}
	else { 
		p[i+1]=p[i]+2*dY-2*dX;
		glBegin(GL_POINTS);
		glColor3f(1.0,1.0,1.0);
			glVertex2f((float)(Xi+i),z[i]+1);
		glEnd();
		
}	
	
	glBegin(GL_POINTS);
	glColor3f(1.0,0.0,0.0);
	glVertex2f((float)(Xi+i-100),p[i]);
	glEnd();
}
}
 break;

  case 0x2:  /* Middle Mouse Button */
	if ( mode == 2 ){
	glColor3f(0.0,1.0,0.0);
	glRecti(Xg,Yg,Xg+75,Yg+75);
	glRecti(X,Y,X+75,Y+75);
	 Xg=X;Yg=Y;
//	mode = 2;
}
	if (mode == 3 ){
	
//	glLogicOp(GL_XOR);
	glColor3f(1.0,0.0,1.0);
	glRecti(Xm,Ym,Xm+75,Ym+75);
	glRecti(X,Y,X+75,Y+75);
	//glLogicOp(GL_AND);
	Xm=X;Ym=Y; 
}
    break;
  case 0x4:  /* Right Mouse Button  */
	if ( mode == 2 ){
	glColor3f(0.0,0.0,1.0);
	glRecti(Xb,Yb,Xb+75,Yb+75);
	glRecti(X,Y,X+75,Y+75);
	Xb=X;Yb=Y;
//	mode =2;
	 
}
	if (mode == 3 ){

glColor3f(1.0,1.0,0.0);
glRecti(Xy,Yy,Xy+75,Yy+75);
glRecti(X,Y,X+75,Y+75);
	Xy=X;Yy=Y; 
} 
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

/* Handle Keyboard */
void
keyboard(unsigned char key, int x, int y)
{
  /*  Option: To force all lower case */
  /* if (key >= 'A' && key <= 'Z') key = (key-'A') + 'a'; */

  switch (key) {

  case 'q' : case 27 :  /* 27 -> ESC */
    fprintf(stderr,"Normal Exit.\n");
    exit(EXIT_SUCCESS);
    break;
  case 'r':
	mode = 1;
	flag = 1;
	glLogicOp(GL_XOR);
	break;
  case 32 :
	if(op > 8) op=1;
	if(op == 1) {glLogicOp(GL_AND);fprintf(stderr,"Operation  %d: GL_AND",op);}
	if(op == 2) {glLogicOp(GL_OR);fprintf(stderr,"Operation  %d: GL_OR",op);}
	if(op == 3) {glLogicOp(GL_NOR);fprintf(stderr,"Operation  %d: GL_NOR",op);}
	if(op == 4) {glLogicOp(GL_NAND);fprintf(stderr,"Operation  %d: GL_NAND",op);}
	if(op == 5) {glLogicOp(GL_COPY);fprintf(stderr,"Operation  %d: GL_COPY",op);}
	if(op == 6) {glLogicOp(GL_INVERT);fprintf(stderr,"Operation  %d: GL_INVERT",op);}
	if(op == 7) {glLogicOp(GL_AND_REVERSE);fprintf(stderr,"Operation  %d: GL_AND_REVERSE",op);}
	if(op == 8) {glLogicOp(GL_AND_INVERTED);fprintf(stderr,"Operation  %d: GL_AND_INVERTED",op);}
	op++;
	break;
  case '1':
	mode=1;
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);
	flag = 1;
	break;
  case '2':
	 mode =2;	
	 glEnable(GL_COLOR_LOGIC_OP);
	 glLogicOp(GL_XOR);
	 glClear(GL_COLOR_BUFFER_BIT);
	 glColor3f(1.0,0.0,0.0);
	 glRecti(100,100,175,175);
	 glColor3f(0.0,1.0,0.0);
	 glRecti(200,200,275,275);
	 glColor3f(0.0,0.0,1.0);
	 glRecti(300,300,375,375);
	 break;
  case '3':
	mode =3;	
	glEnable(GL_COLOR_LOGIC_OP);
	 glLogicOp(GL_XOR);
	 glClear(GL_COLOR_BUFFER_BIT);
	 glColor3f(0.0,1.0,1.0);
	 glRecti(100,100,175,175); 
	 glColor3f(1.0,0.0,1.0);
	 glRecti(200,200,275,275);
	 glColor3f(1.0,1.0,0.0);
	 glRecti(300,300,375,375);
	 break;
 case '4' :
	mode = 4;
	glClear(GL_COLOR_BUFFER_BIT);
	
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
  glClearColor(0.0, 0.0, 0.0, 0.0);
}




/***************************************************************/
/************************ DRAW STUFF ***************************/
/***************************************************************/


void
drawObjs()
{

if (flag == 1) {

  glColor3f(0.6,0.8,0.4);
  glRecti(100,100,300,300); 

  glColor3f(1.0,1.0,0.0);
glBegin(GL_POINTS);
  glVertex2i(X,Y);
glEnd();
}

flag = -1;
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
display(void)
{
  fprintf(stderr,"Comment this out: display\n");
	
 
  if(flag == 1 )
  glClear(GL_COLOR_BUFFER_BIT);
  
  if (mode == 1)	
  drawObjs();

  glFlush();
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
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  /*   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  */

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
