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

int FirstClear = GL_TRUE,Pointclear = GL_TRUE;

int myDrawingMode = 3;
GLubyte logo_bits[] = {
   0x00, 0x66, 0x66, 
   0xff, 0x66, 0x66, 
   0x00, 0x00, 0x00, 
   0xff, 0x3c, 0x3c, 
   0x00, 0x42, 0x40, 
   0xff, 0x42, 0x40, 
   0x00, 0x41, 0x40, 
   0xff, 0x21, 0x20, 
   0x00, 0x2f, 0x20, 
   0xff, 0x20, 0x20, 
   0x00, 0x10, 0x90, 
   0xff, 0x10, 0x90, 
   0x00, 0x0f, 0x10, 
   0xff, 0x00, 0x00, 
   0x00, 0x66, 0x66, 
   0xff, 0x66, 0x66, 
   
};
/* 1 == pixel manipulations, 2 == color blending, 3 == rubberband line  */
int mySelectedRect = 1;
/* 1 == red rectangle, 2 == green rectangle, 3 == blue rectangle  */

GLfloat myZoomFactor = 1.0;

void drawRubberband();
int mouseButtons = 0;

int MouseModifiers = 0;

int  X=-1, Y=-1, dX=0, dY=0;
int  Xr=160, Yr=240, Xg=320, Yg=240, Xb=480, Yb=240;
int  Xp = -1, Yp = -1,L=0,R=0;
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
/*Recording click values*/
    L = Xp;
    R = Yp; 	
    
  MouseModifiers = glutGetModifiers();
  /* bitmask with GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT */
 if (button == 1){
 
  glPointSize((x+y)/20.0);
glColor3f(x/500.0,y/500.0,(x+y)/500.0);	
  glBegin(GL_POINTS);
      glVertex3f (X, Y, 0.0);
   glEnd();
   glFlush();
}
 if (button == 2){
   	
  glPointSize((x+y)/20.0);	
glColor3f(x/500.0,y/500.0,(x+y)/500.0);
  glBegin(GL_BITMAP);
	glRasterPos2i(X, Y); 
     glBitmap(X/10, Y/10, 8.0, 0.0, 0.0, 0.0, logo_bits);
   glEnd();
   glFlush();
}
if (button == 4){
 
  glRasterPos2i(X, Y);
     glPixelZoom(X/10,Y/10);
  glDrawPixels(8,8, GL_RGB,GL_UNSIGNED_BYTE,logo_bits);
   
   glFlush();
}
  if (state == GLUT_DOWN) {
    mouseButtons |= button;
    if (myDrawingMode == 3) {
	
      /* in rubberband line mode, you may want to deal with cleaning up here */
      /* .... your code here .... */

    }
  }
else {
    mouseButtons &= (~(button));
  }
     
  fprintf(stderr,"mouse buttons = [%d], modifiers = [%d] \n\
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
  fprintf(stderr," mouse at (%d,%d)\n\
          mouse buttons = [%d], modifiers = [%d]\n",
	  x,y, mouseButtons, MouseModifiers);  
  
      

  
  X = x;
  Y =  main_height - y - 1;	/* Invert to right-handed system */
  Xp = X;
  Yp = Y;
/* .... your mouse-motion-related code here .... */
     drawRubberband();

  // fprintf(stderr,"X, Y = %d, %d \n",X,Y);
  		
  			
  switch(mouseButtons ){
  case 0x1:  /* Left Mouse Button   */
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
        case 'p' :
		 Pointclear = GL_TRUE;	
		 fprintf(stdout,"drawing points\n");
	case 'r' :
         /* .... more code here .... */
          fprintf(stdout,"color blending, move red\n");
          break;
      case 'g' :
         /* .... more code here .... */
          fprintf(stdout,"color blending, move green\n");
          break;
      case 'b' :
         /* .... more code here .... */
          fprintf(stdout,"color blending, move blue\n");
          break;
      case 'a' :
         /* .... more code here .... */
	  FirstClear = GL_TRUE;
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
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
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
  glBegin(GL_LINES);
      glVertex3f (L, R, 0.0);
      glVertex3f (X, Y, 0.0);
   glEnd();
  // glutPostRedisplay();
   glFlush();	
        /* .... more code here .... */
} /* drawRubberband() */




/***************************************************************/
/************************ GLUT STUFF ***************************/
/***************************************************************/


void
reshape(int w, int h)
{
  fprintf(stderr,"reshape \n");
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
  fprintf(stderr,"visibility: %s\n",
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
     if(Pointclear){ fprintf(stderr,"Clear\n");
     sleep(1);  
     glClear(GL_COLOR_BUFFER_BIT ); 
     /*  sleep(1); does not work here, must preceed Clear  */    
    Pointclear = GL_FALSE;
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
  main_window = glutCreateWindow("Pixel Operations and Rubberband Line");

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
