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

int  X=0, Y=0, dX=0, dY=0,mode=0;

int main_window;
int main_height, main_width;

float mat[16];

int xvalue[100],yvalue[100],i=0,j,L,R;

float d,max,angle=1,limit; 
int k,xcent,ycent,xscale=0,yscale=0,xcum,ycum,flag1=1,flag2=1,flag3=1,lim; 

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

  if (state == GLUT_DOWN) {
    mouseButtons |= button;
	if (mode == 1) xscale=yscale=0;	
	if (mode == 2){
	// xscale = X-xvalue[1];
	 //yscale = Y-yvalue[1]; 	
	xscale = X-xcent;
	yscale = Y-ycent; 	
		
	}

   }
  else if (state == GLUT_UP) {
	if(mode == 1) {	
	xscale=yscale=0;	
	L= X;
	R = Y;
	++i;
	xvalue[i]= L;
	yvalue[i]= R;
    }	
	if ( mode == 2) {
	
	xscale = X-xcent;
	yscale = Y-ycent; 
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
	if (mode == 2) {
		if(flag1 ==1){
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
		flag1=0;
		}
	
	glLoadIdentity();
	xscale = X-xcent;
	yscale = Y-ycent;
	
	glTranslatef((float)xscale,(float)yscale,0.0);
	
	}
    break;
  case 0x2:  /* Middle Mouse Button */
	if (mode == 2) {
		if(flag2 == 1){
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
		flag2 ==0;
		limit = 0.4;
		lim =0;
	}
	   
	  if(lim++==10){limit = 0.4;lim=0; }
		limit = limit + 0.2;

	
	glScalef((float)limit,(float)limit,0.0);
	

	}
    break;
  case 0x4:  /* Right Mouse Button  */
	if (mode == 2){
	if(flag3 == 1){
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
		flag3 =0;
		
	}
	
	angle = atanf((float)(Y-ycent)/(float)(X-xcent));
	glRotatef((angle),0.0,0.0,1.0);
	


	}
    break;
  }
  glutPostRedisplay();

}

/* Mouse motion WHILE ALL BUTTONS UP  */
/*  COMPLAINS  if you call glutGetModifiers()  here!! */
void
passive(int x, int y)
{
  if (mode == 1) {
  X = x;
  Y = main_height - y - 1;
  xcum =0;
  ycum =0;	

	fprintf(stderr,"mouse at (%d,%d)\n",X,Y); 
	for(j=1;j<=i;j++){
	  
		xcum+=xvalue[j];
		ycum+=yvalue[j];
	  	
	}
	
	if(i!=0){	
	xcent=xcum/i;
	ycent=ycum/i;
	}
	fprintf(stderr,"xcent and ycent (%d,%d)\n",xcent,ycent);
	max = sqrt((X-xcent)*(X- xcent)+(Y- ycent)*(Y- ycent));
	for (j=1;j<=i;j++) {
	   d=sqrt((X- xvalue[j])*(X- xvalue[j])+(Y- yvalue[j])*(Y- yvalue[j]));
	   if(d < max){
		max = d;
		k = j;
	   }	
	}
	fprintf(stderr,"closest to (%d,%d)\n",xvalue[k],yvalue[k]); 
 }       
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
  case '1':case 'r' :
	mode = 1; 
	break;
  case '2':
	mode = 2;
        break;	
  case 'R':
	glClear(GL_COLOR_BUFFER_BIT);
	break;
  case 'h':
	fprintf(stderr,"Help: goes here.\n");
	showHelp();
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
  glClearColor(0.0, 0.0, 0.0, 0.0);
}




/***************************************************************/
/************************ DRAW STUFF ***************************/
/***************************************************************/


void
drawObjs()
{
  glColor3f(0.9,0.8,0.4);
  glRecti(200,50,300,450); 
  glClear(GL_COLOR_BUFFER_BIT);
        
	if (mode == 1 || mode ==2) {	
	
	if(i>=3) {
		glPointSize(3.0);
		glBegin(GL_POINTS);
		glVertex2i(xcent,ycent);
		glEnd();
	for (j=1;j<=i;j++) {
	fprintf(stderr,"mouse at (%d,%d)\n",xvalue[j],yvalue[j]); 
		glPointSize(5.0);
		glBegin(GL_POINTS);
		glVertex2i(xvalue[j],yvalue[j]);
		glEnd();
		
	}
	 
		glBegin(GL_LINE_LOOP);
		for (j=1;j<=i;j++)
		glVertex2i(xvalue[j],yvalue[j]);
		glEnd();
		
}
   if (mode == 1) {	
  showPixel(); 
  showEdge();
  }
  glColor3f(1.0,0.0,1.0);
  
      
	}
}

void
showPixel()
{
if(i>=3) {
glColor3f(1.0,0.6,0.0);
glPointSize(10.0);
	glEnable(GL_COLOR_LOGIC_OP);
	glEnable(GL_XOR);
	glBegin(GL_POINTS);
	glVertex2i(xvalue[k],yvalue[k]);
	glEnd();
	glDisable(GL_COLOR_LOGIC_OP);


}
}
void
showEdge()
{
if(i>=3) {
glColor3f(1.0,0.6,0.0);

	glEnable(GL_COLOR_LOGIC_OP);
	glEnable(GL_XOR);
	glBegin(GL_LINE_LOOP);
	if(k ==1) {
	glVertex2i(xvalue[i],yvalue[i]);
	glVertex2i(xvalue[1],yvalue[1]);
	}
	else{
	glVertex2i(xvalue[k-1],yvalue[k-1]);
	glVertex2i(xvalue[k],yvalue[k]);
	}
	glEnd();
	glDisable(GL_COLOR_LOGIC_OP);


}
}

void 
showHelp() {
	glClear(GL_COLOR_BUFFER_BIT);
	fprintf(stderr,"This is the help command\n");
	fprintf(stderr,"I have used showPixels and showEdge functions for the first task\n");
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
  
	
  glClear(GL_COLOR_BUFFER_BIT);
  
  
  drawObjs();
  
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
  glutPassiveMotionFunc(passive);
    
  glutKeyboardFunc(keyboard);
  fprintf(stderr,"Press ESC or 'q' to Exit.\n");

  glutIdleFunc(NULL);

  glutMainLoop();

  return(EXIT_SUCCESS);
}
