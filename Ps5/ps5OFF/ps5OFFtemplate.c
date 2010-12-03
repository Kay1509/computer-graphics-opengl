/*
 * Computer Graphics:  Andrew Hanson
 * ps5 : template 
 *
 * Use Makefile or by hand:
  cc -c readoff.c
  cc -o ps5.linux ps5.c readoff.o -lglut -lGLU -lGL -lm
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "readoff.h"		/* Data structure definitions */

/***********************************************************************/
#define Sign(A) ((A) < 0.0 ? -1.0 : ((A) > 0.0 ? 1.0 : 0.0))
#define Min(A,B) ((A) <= (B) ? (A) : (B))
#define Max(A,B) ((A) >= (B) ? (A) : (B))
#define Inbetween(A,X,B) ((Min(A,B) <= (X)) && ((X) <= Max(A,B)))
/***********************************************************************/

/* ---------- */
/* 3D objects */
/* ---------- */
Object3D object;
Object3D *obj;
Point3 *facenormals;
float focal, viewDist;
int i,j;
Vector3 p,q,r;
double rlength;
/***************************************************************/
/********************  GLOBAL VARIABLES ************************/
/***************************************************************/
int FirstVertex = GL_TRUE;
int ClearFlag= GL_TRUE;
int MouseButton = 0;
int JustUp = GL_FALSE;
int JustDown = GL_FALSE;
int MouseModifiers = 0,i=0;

int  X=0, Y=0;

GLfloat XC, YC, ZC;


/* LIGHTING */
GLfloat light_ambient[] = {0.5,0.5,0.5, 1.0};
GLfloat light_diffuse[] = {1.0,1.0,1.0, 1.0};
GLfloat light_specular[] = {1.0,1.0,1.0, 1.0};
GLfloat light_position[] = {10.0,10.0,30.0, 0.0}; /* At infinity */
GLfloat material_diffuse[] = {0.7,0.7,0.0, 1.0};
GLfloat material1_diffuse[] = {0.0,0.7,0.7, 1.0};
/***************************************************************/
/********************** DRAWING STUFF **************************/
/***************************************************************/


void drawObjects()
{
  glNormal3f(0.0,0.0,1.0);
  glRectf(-1.0,-1.0,1.0,1.0);
  
  glDisable(GL_LIGHTING);
  glLineWidth(3.0);
  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,1.0);
  glEnd();
  glEnable(GL_LIGHTING);
 }


void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glNormal3f(0.0,0.0,1.0);
  PrintObj(&object);
  obj = &object;
  
 	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,
        //    material_diffuse);
   //glDisable(GL_LIGHTING);
	for(i=0; i<obj->Nfaces; i++){
	glBegin(GL_TRIANGLE_FAN);
	for(j=0; j<obj->nv_face[i]; j++) {
   	
	p.x = obj->vertices[obj->faces[i][1]].x - obj->vertices[obj->faces[i][0]].x;
	p.y = obj->vertices[obj->faces[i][1]].y - obj->vertices[obj->faces[i][0]].y;
	p.z = obj->vertices[obj->faces[i][1]].z - obj->vertices[obj->faces[i][0]].z;
	
	q.x = obj->vertices[obj->faces[i][2]].x - obj->vertices[obj->faces[i][1]].x;
	q.y = obj->vertices[obj->faces[i][2]].y - obj->vertices[obj->faces[i][1]].y;
	q.z = obj->vertices[obj->faces[i][2]].z - obj->vertices[obj->faces[i][1]].z;
	
	r = V3cross(p,q);
	rlength = V3Length(r);	
	fprintf(stderr,"\n\n(Nx,Ny,Nz)  = (%f,%f,%f) %d\n\n",(r.x)/rlength,(r.y)/rlength,(r.z)/rlength,i);
	glNormal3f((r.x)/rlength,(r.y)/rlength,(r.z)/rlength);
	//fprintf(stderr,"%d ", obj->faces[i][j]);
	// fprintf(stderr,"\n\n(Nx,Ny,Nz) = (%f,%f,%f)\n\n",(r.x)/rlength,(r.y)/rlength,(r.z)/rlength);
	//glColor3f(1,1,0);
	glVertex3f(obj->vertices[obj->faces[i][j]].x,obj->vertices[obj->faces[i][j]].y,obj->vertices[obj->faces[i][j]].z);
	}
   	glEnd();
	fprintf(stderr,"\n\n(Nx,Ny,Nz)  = (%f,%f,%f) %d\n\n",(r.x),(r.y),(r.z),i);
	glDisable(GL_LIGHTING);
	glColor3f(0,0,1);
 	//glMaterialf(GL_FRONT,GL_SHININESS,
               //0.5);
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex3f(obj->vertices[obj->faces[i][1]].x,obj->vertices[obj->faces[i][1]].y,obj->vertices[obj->faces[i][1]].z);
	//glVertex3f((r.x)/rlength,(r.y)/rlength,(r.z)/rlength);
	//glVertex3f((r.x)/rlength,(r.y)/rlength,(r.z)/rlength);
	//glVertex3f((r.x)+5.0,(r.y)+5.0,(r.z)+5.0);
	glVertex3f(obj->vertices[obj->faces[i][1]].x +(r.x),obj->vertices[obj->faces[i][1]].y+(r.y),obj->vertices[obj->faces[i][1]].z+(r.z));
	glEnd();
	glEnable(GL_LIGHTING);	
	} 

  // glEnable(GL_LIGHTING);
   

  //drawObjects();
 
  glutSwapBuffers();
}

void
reshape(int w, int h)
{
  fprintf(stderr,"Reshape World\n");
  w = Max(w,128);
  h = Max(h,128);
  glutReshapeWindow(w,h);
  glViewport(0,0,w,h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* gluOrtho2D(0.0,(double)w,(double)h,0.0); */
  /* glOrtho(-1.0,1.0,-1.0,1.0,1.0,-1.0); */
  //glFrustum(-1.0,1.0,-1.0,1.0,
	//    focal,25.0);
  gluPerspective(90,1.0,0.1,100);
  glTranslatef(-XC,-YC,-ZC);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

/***************************************************************/
/*********************** MOUSE CALLBACKS ************************/
/***************************************************************/

void
mouse(int button, int state, int x, int y)
{ X = x;
  Y = y;
 
  MouseButton ^= (1 << button);
  MouseModifiers = glutGetModifiers();
  /* bitmask with GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT */

  /*   fprintf(stderr," mouse at (%d,%d), buttons %x, Modifiers %x\n", 
         x,y, MouseButton, MouseModifiers); */
     

  glutPostRedisplay(); 
}

void
motion(int x, int y)
{
  glRotatef(1.0*(x-X),0.0,1.0,0.0);
  X = x;
  Y = y;

  /*  COMPLAINS  if you call glutGetModifiers()  here!! */

  glutPostRedisplay();
}


void
passive(int x, int y)
{
  X = x;
  Y = y;

  /*  COMPLAINS  if you call glutGetModifiers()  here!! */

  glutPostRedisplay();
}

/***************************************************************/
/************************** Initialization**********************/
/***************************************************************/

void myinit(int *argc, char **argv)
{
  int i;

  /*  int wid,hi;
      wid= glutGet(GLUT_WINDOW_WIDTH);
      hi = glutGet(GLUT_WINDOW_HEIGHT);
   */

  /* focal length initialization */
  focal = 5.0;
  //viewDist = 10.0;
   viewDist = 4.0;

  /* Lighting initialization */
  glShadeModel(GL_FLAT);        /* GL_SMOOTH */
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 1.0, 1.0, 0.0);
 
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,
               material_diffuse);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glPopMatrix();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);


  /* Read in 3D object from OFF file */
  fprintf(stderr,"\n ps5: argc = %d: ",*argc);
  for (i = 0; i< *argc; i++)
    fprintf(stderr, " arg[%d] = %s, ",i, argv[i]);
  fprintf(stderr,"\n");

  /* Read in a file */

 if (*argc == 2)
    {
      printf("Reading in %s.\n", argv[1]);
      ReadOFF(argv[1], &object);
        }
  else if (*argc == 1)
    {
      printf("Using default object.\n");
      ReadOFF("cube.off", &object);
    }

  // XC = YC = 0.0;
	XC =0.0;
	YC = 0.0;
   ZC = viewDist;

   /* Hint: here is how to start face normal procedure. */
  facenormals = (Point3 *) malloc(object.Nfaces*sizeof(Point3));

}


void
keyboard(unsigned char key, int x, int y)
{
  /*  To lower case */
  if (key >= 'A' && key <= 'Z')
    key = (key-'A') + 'a';

  switch (key) {
    
  case 'q' : case 27 :	/* 27 -> ESC */
    fprintf(stderr,"Normal Exit.\n");
    exit(EXIT_SUCCESS);
    break;

  }
  glutPostRedisplay();
}



/***************************************************************/
/**************************** MAIN *****************************/
/***************************************************************/


int
main(int argc, char **argv)
{
  /* GLUT Initialization */
  glutInit(&argc,argv);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

  /* Create a new window and give a title to it */
  glutCreateWindow("ps5 - CHANGE ME OR ELSE!");

  /* Initialize OpenGL stuff */
  myinit(&argc,argv);

  /* Post Event Handlers */
  glutReshapeFunc(reshape);

  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(NULL);
    
  glutKeyboardFunc(keyboard);
  fprintf(stderr,"Press ESC or 'q' to Exit. 'r' to Clear and Reset\n");


  glutIdleFunc(NULL);

  glutMainLoop();

  return(EXIT_SUCCESS);
}
