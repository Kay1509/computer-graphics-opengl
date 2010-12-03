/*
 *  Computer Graphics:  Andrew Hanson
 * GLUT RGB  Graphics template 
 *

 on Linux (X11) you could type:
 cc -o ps1.linux  ps1.c -lglut -lGLU -lGL  -lm
 
 on Mac OS X (Aqua) you could type:
 cc -o ps1.macosx ps1.c -framework OpenGL -framework GLUT



 NOTE: This should work on default-X-configured Linux GRAPHICS SERVERS
        because it defines an RGB visual. 

 *
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/* Local Graphics Definitions  */
#include "defs.h"


/***************************************************************/
/******************** MY GLOBAL VARIABLES **********************/
/***************************************************************/

int FirstClear = GL_TRUE;

int myDrawingMode = 3,j,k,m=0,n=0,XL,YL;
/* 1 == pixel manipulations, 2 == color blending, 3 == rubberband line  */
int mySelectedRect = 1;
/* 1 == red rectangle, 2 == green rectangle, 3 == blue rectangle  */

GLfloat myZoomFactor = 1.0;


int mouseButtons = 0;

int MouseModifiers = 0;

int  X=-1, Y=-1, dX=0, dY=0;
int  Xr=160, Yr=240, Xg=320, Yg=240, Xb=480, Yb=240;
int  Xp = -1, Yp = -1;
GLfloat XC = 0.0, YC = 0.0, ZC = -10.0;
int main_window;
int main_height, main_width;

/***************************************************************/
/*********************** MOUSE CALLBACKS ***********************/
/***************************************************************/

/* Mouse Button  UP/DOWN  or DOWN/UP transition only */
void
mouse(int button, int state, int x, int y)
{
  button = (1 << button);

    X = x;
    Y = main_height - y - 1;	/* Invert to right-handed system */
    Xp = X;
    Yp = Y;
		
    	
    
  MouseModifiers = glutGetModifiers();
  /* bitmask with GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT */

  if (state == GLUT_DOWN) {
    mouseButtons |= button;
	if (button == 1) {   
	 m=0;n=0;
	 XL = Xp;
    	 YL = Yp;
}
    if (myDrawingMode == 3) {
      /* in rubberband line mode, you may want to deal with cleaning up here */
      /* .... your code here .... */

    }
  } else {
    mouseButtons &= (~(button));
  }
     
  fprintf(stderr,"Mouse buttons = [%d], modifiers = [%d] \n\
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
  fprintf(stderr,"Mouse at (%d,%d)\n\
          mouse buttons = [%d], modifiers = [%d]\n",
	  x,y, mouseButtons, MouseModifiers);  
  
      /* .... your mouse-motion-related code here .... */
  
  X = x;
  Y =  main_height - y - 1;	/* Invert to right-handed system */
  Xp = X;
  Yp = Y;
	

  // fprintf(stderr,"X, Y = %d, %d \n",X,Y);
  
  
  switch(mouseButtons ){
  case 0x1:  /* Left Mouse Button   */
	if(XL!=X || YL!=Y) glClear(GL_COLOR_BUFFER_BIT);
	m-=2;
 	if(sqrt((320-XL)*(320-XL)+(240-YL)*(240-YL)) -sqrt((X-320)*(X-320)+(Y-240)*(Y-240)) > 0 ) { 
	glColor3f(1.0,1.0,1.0);
	for (j=1,k=1;j<=14,k<=14;j++,k++) {
	if(m<-20 ) { m=0;continue;}
	glBegin(GL_LINE_LOOP);
	glVertex2f(320-20*j-m,240+15*k+m);
        glVertex2f(320+20*j+m, 240+15*k+m);
 	glVertex2f(320+20*j+m,240-15*k-m);
	glVertex2f(320-20*j-m, 240-15*k-m);
	glEnd();
	}

}
if(sqrt((320-XL)*(320-XL)+(240-YL)*(240-YL)) -sqrt((X-320)*(X-320)+(Y-240)*(Y-240)) < 0) { 
	n+=2;
	glColor3f(1.0,0.0,0.0);
	for (j=1,k=1;j<=14,k<=14;j++,k++) {
	if(n>20 ) { n=-2;continue;}
	glBegin(GL_LINE_LOOP);
	glVertex2f(320-20*j-n,240+15*k+n);
        glVertex2f(320+20*j+n, 240+15*k+n);
 	glVertex2f(320+20*j+n,240-15*k-n);
	glVertex2f(320-20*j-n, 240-15*k-n);
	glEnd();
	}

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
  Xp = x;
  Yp = main_height - y - 1;
      /* .... ask for redisplay here only if necessary .... */
}

/* Handle Keyboard */
void
keyboard(unsigned char key, int x, int y)
{
  /*  Option: To force all lower case */
  /* if (key >= 'A' && key <= 'Z') key = (key-'A') + 'a'; */

  switch (key) {
      case '1' : case '2' : case '3' : case '4' : case '5' :
      case '6' : case '7' : case '8' : case '9' :
         /* .... more code here .... */
          fprintf(stdout,"pixel manipulations, zoom factor = %3.3f\n", myZoomFactor);
          break;
      case 'r' :
         /* .... more code here .... */
          fprintf(stdout,"color blending, move red\n");
          break;
      case 'g' :
         /* .... more code here .... */
          fprintf(stdout,"color blending, move green\n");
          break;
      case 'z' :
		 	
		FirstClear = GL_TRUE;	
		 fprintf(stdout,"drawing rectangles\n");
      case 'b' :
         /* .... more code here .... */
          fprintf(stdout,"color blending, move blue\n");
          break;
      case 'a' :
         /* .... more code here .... */
          fprintf(stdout,"rubberband line\n");
          break;
      case 'q' : case 27 :  /* 27 -> ESC */
          fprintf(stderr,"Normal Exit.\n");
          exit(EXIT_SUCCESS);
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
  glClearColor(0.0,0.0,0.0,1.0);
  FirstClear = GL_TRUE;
/*     glEnable(GL_DEPTH_TEST);  */
}




/***************************************************************/
/************************ DRAW STUFF ***************************/
/***************************************************************/


/* ------------------------------- */
/* ----- pixel manipulations ----- */
/* ------------------------------- */

void drawPixels() {
         /* .... more code here .... */
} /* drawPixels() */

    
/* -------------------------- */
/* ----- color blending ----- */
/* -------------------------- */

void drawShape(int pX, int pY) {
         /* .... more code here .... */
}


void drawColors() {
    
         /* .... more code here .... */
    /* red shape */
    drawShape(Xr, Yr);

    /* green shape */

    /* blue shape */

         /* .... more code here .... */
} /* drawColors() */



/* --------------------------- */
/* ----- rubberband line ----- */
/* --------------------------- */
void drawRubberband() {
        /* ----- -----  draw rubberband line  ----- ----- */

        /* .... more code here .... */
} /* drawRubberband() */




/***************************************************************/
/************************ GLUT STUFF ***************************/
/***************************************************************/


void
reshape(int w, int h)
{
  fprintf(stderr,"Reshape \n");
  w = Max(w,320);
  h = Max(h,240);
  main_height = h;
  main_width = w;
  glutReshapeWindow(w,h);
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
   gluOrtho2D(0.0,(double)Max(w,320), 0.0, (double)Max(h,240));  


  glMatrixMode(GL_MODELVIEW);
/*   glutPostRedisplay(); */
}

void
visibility(int state)
{
  /* May also want to check PARTIAL DAMAGE that
     does not result in a call to the visibility callback:
      if (glutLayerGet(GLUT_NORMAL_DAMAGED)) {
       redraw = TRUE; }
       */
  fprintf(stderr,"Visibility: %s\n",
	  (state==GLUT_VISIBLE)?"visible":"invisible");
  if (state == GLUT_VISIBLE){
    glutPostRedisplay();
  }
}

void
display(void)
{
    fprintf(stderr,"Display %d\n",myDrawingMode);

    if(FirstClear){ fprintf(stderr,"Clear\n");
     sleep(1);  
     glClear(GL_COLOR_BUFFER_BIT ); 
     /*  sleep(1); does not work here, must preceed Clear  */    
    FirstClear = GL_FALSE;
    }
    
    switch(myDrawingMode){
        case 1:  /* pixel manipulations */
            /* .... more code here .... */
            break;
        case 2:  /* color blending */
            /* .... more code here .... */
            break;
        case 3:  /* rubberband line */
            drawRubberband();
        
            break;
        default:
            fprintf(stderr,"Unhandled drawing mode: %d \n",myDrawingMode);
    }


   /*  glFinish();  */
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
  glutInitWindowSize(640, 480);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA  );

  /* Create a new window and give a title to it */
  main_window = glutCreateWindow("Mouse controlled screen zoom");

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
