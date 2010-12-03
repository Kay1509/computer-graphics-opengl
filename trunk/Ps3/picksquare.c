/*  Modified for GLUT, Solaris, Linux.  
 *   A. Hanson

LINUX:
 cc -o picksquare.linux picksquare.c -lglut -lGLU -lGL

SOLARIS:
 cc -o picksquare.sun picksquare.c  -I/l/glut/include  -L/l/glut/lib  -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm




 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
/*
 *  picksquare.c
 *  Use of multiple names and picking are demonstrated.  
 *  A 3x3 grid of squares is drawn.  When the left mouse 
 *  button is pressed, all squares under the cursor position 
 *  have their color changed.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


int board[3][3];	/*  amount of color for each square	*/

/*	Clear color value for every square on the board	    */
void init(void)
{
    int i, j;
    for (i = 0; i < 3; i++) 
	for (j = 0; j < 3; j ++)
	    board[i][j] = 0;
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

/*  The nine squares are drawn.  In selection mode, each 
 *  square is given two names:  one for the row and the 
 *  other for the column on the grid.  The color of each 
 *  square is determined by its position on the grid, and 
 *  the value in the board[][] array.
 */
void drawSquares(GLenum mode)
{
    GLuint i, j;
    for (i = 0; i < 3; i++) {
	if (mode == GL_SELECT)
	    glLoadName (i);
	for (j = 0; j < 3; j ++) {
	    if (mode == GL_SELECT)
		glPushName (j);
	    glColor3f ((GLfloat) i/3.0, (GLfloat) j/3.0, 
		    (GLfloat) board[i][j]/3.0);
	    glRecti (i, j, i+1, j+1);
	    if (mode == GL_SELECT)
		glPopName ();
	}
    }
}

/*  processHits() prints out the contents of the 
 *  selection array.
 */
void processHits (GLint hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint ii, jj, names, *ptr;

    printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;
    for (i = 0; i < hits; i++) {	/*  for each hit  */
	names = *ptr;
	/*  NOTE: the Redbook is wrong - this should give 0.5
	    NOT  0.999...   Use 0xffff... not 0x7fff..  */
	printf (" number of names for this hit = %d\n", names);	ptr++;
	printf ("  z1 is %g;",(float) *ptr/0xffffffff); ptr++;
	printf (" z2 is %g\n", (float) *ptr/0xffffffff); ptr++;
	printf ("   names are ");
	for (j = 0; j < names; j++) {	/*  for each name */
	    printf ("%d ", *ptr);
	    if (j == 0)	/*  set row and column  */
		ii = *ptr;
	    else if (j == 1)
		jj = *ptr;
	    ptr++;
	}
	printf ("\n");
	board[ii][jj] = (board[ii][jj] + 1) % 3;
    }
}

/*  [mouse]pickSquares() sets up selection mode, name stack, 
 *  and projection matrix for picking.  Then the 
 *  objects are drawn.
 */
#define BUFSIZE 512

void mousePickSquares(int button, int state, int x, int y)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];

    glGetIntegerv (GL_VIEWPORT, viewport);

    glSelectBuffer (BUFSIZE, selectBuf);
    (void) glRenderMode (GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
/*  create 5x5 pixel picking region near cursor location	*/
    gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
	10.0, 10.0, viewport);
    gluOrtho2D (0.0, 3.0, 0.0, 3.0);
    drawSquares (GL_SELECT);

    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
    processHits (hits, selectBuf);
    glutPostRedisplay();
} 

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSquares (GL_RENDER);
    glFlush();
}

/* Handle Keyboard */
void
keyboard(unsigned char key, int x, int y)
{
  /*  Option: To force all lower case */
  /* if (key >= 'A' && key <= 'Z') key = (key-'A') + 'a'; */

  switch (key) {
  case 'r': 
    printf(" 'r' key \n");
    break;
  case 'q' : case 27 :  /* 27 -> ESC */
    fprintf(stderr,"Normal Exit.\n");
    exit(EXIT_SUCCESS);
    break;
  default: fprintf(stderr,"Unhandled key: %c [%d] \n",key,key);
  }

  glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, 3.0, 0.0, 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */

int
main(int argc, char **argv)
{ int namestackdepth;

  /* GLUT Initialization */
  glutInit(&argc,argv);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100, 100);
 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
  /*  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); */

  /* Create a new window and give a title to it */
  glutCreateWindow("OGL picksquare example");

  /* Initialize OpenGL stuff */
  init();

  /* Post Event Handlers */
  glutMouseFunc(mousePickSquares);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);

  glutKeyboardFunc(keyboard);

  /*   glutMotionFunc(motion); */
  /* glutPassiveMotionFunc(passive); */

  glGetIntegerv(GL_MAX_NAME_STACK_DEPTH,&namestackdepth);

  fprintf(stderr,"\n Name stack depth = %d\n",namestackdepth );

  fprintf(stderr,"Press ESC or 'q' to Exit.\n");

  glutMainLoop();

  return(EXIT_SUCCESS);
}
