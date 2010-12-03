
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "readoff.h"
#include "readppm.h"


/*****************************************************************/
/******************** MY FUNCTION DEFINITIONS ********************/
/*****************************************************************/

static void myInitGL(void);
static void myInitData(int *argc, char **argv);
static void myInitMenus (void);
static void myInitCheckers(void);
static void myDrawTeapot(GLdouble size);

/* my callback functions needed for GLUT event handling */
static void myHandleMenu(int value);
static void myDisplay(void);
static void myReshape(int width, int height);
static void myMotion(int curx, int cury);
static void myButton(int button, int state, int x, int y);
static void myVisibility(int state);
static void myKeyboard(unsigned char key, int x, int y);
static void myMenus (int value);


/***************************************************************/
/******************** MY GLOBAL CONSTANTS **********************/
/***************************************************************/

#define MY_MAX(A,B) ((A) >= (B) ? (A) : (B))

/* my constants for GLUT window defaults */
#define MY_WIND_X_SIZE     600
#define MY_WIND_Y_SIZE     600
#define MY_WIND_X_POS      100
#define MY_WIND_Y_POS      100

/* my menu IDs for GLUT menu-handling functions */
#define MY_MENU_EXIT     -9999
#define MY_MENU_RESET     -200
#define MY_MENU_CAMERA      20
#define MY_MENU_LIGHT       30
#define MY_MENU_OBJECT      40
#define MY_MENU_PROJECT     100

#define	MY_CHECKERS_WIDTH   64
#define	MY_CHECKERS_HEIGHT  64

Object3D object;
Object3D *obj;
Point3 *facenormals;
int i,j,l,m,n,count=0,flag=1,N ,M;
Vector3 p,q,r,a,b;
double rlength,d =0 /*,LA = 0.0,LB = 0.0,LC = 0.0*/;
//double dispx = -2.0, dispz = -1.2;
double smallx =0.0,smallz = 0.0,randx=0.0,randz=0.0,prevx=0.0,prevz=0.0;
double cx,cy,cz;
char *s = "My_Model";
struct timeval tv;


/***************************************************************/
/******************** MY GLOBAL VARIABLES **********************/
/***************************************************************/

/* globals for generated texture - modified from Red Book's original checker.c - */
static GLubyte CheckersImg[MY_CHECKERS_HEIGHT][MY_CHECKERS_WIDTH][3];
static GLuint  TextureName;
static GLboolean TextureOn = GL_TRUE;

static TexMap  TMap;   /* global for texture from readppm */

static GLfloat LightPos[4];

/* user interaction */
static GLint InteractionMode;
static GLint XmouseDown, YmouseDown;
static GLint XmouseCurrent, YmouseCurrent;
static GLint XmousePrev, YmousePrev, DX, DY;
static GLint WindHeight, WindWidth;
static GLint KeybModifiers;
static GLint mouseButtons = 0;

/* transformations */
static GLdouble  TX, TY, TZ;

/* 3D Projection */
static GLdouble XCamera, YCamera, ZCamera;
static GLdouble FOVangle, FOVratio, ClipNear, ClipFar;


/**************************** MAIN *****************************/
void timer(int value) {
	glutPostRedisplay();
	
	glutTimerFunc(100,timer,0);
}

int main(int argc, char **argv) {
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(MY_WIND_X_POS, MY_WIND_Y_POS);
    glutInitWindowSize(MY_WIND_X_SIZE, MY_WIND_Y_SIZE);

    glutCreateWindow("B581 template code with 3D lighting");

    glutDisplayFunc       (myDisplay);
    glutReshapeFunc       (myReshape);
    glutMouseFunc         (myButton);
    glutMotionFunc        (myMotion);
    glutPassiveMotionFunc (NULL);
    glutIdleFunc          (NULL);
    glutKeyboardFunc      (myKeyboard);
    glutVisibilityFunc    (myVisibility);
    
    fprintf(stderr,"Press ESC or 'q' to Exit.\n");

    myInitGL();
    myInitData(&argc,argv);  /* pass command line arguments for texture */

    /* initialize GLUT menu handler as last one, to take over access
     to GLUT_RIGHT_BUTTON, regardles to what other handlers did to it: */
    myInitMenus();
    glutTimerFunc(0,timer,0);
    glutMainLoop();
    return(EXIT_SUCCESS);
} /* main() */



void getCoM(){
	obj = &object;
	
	for(i=0 ;i< object.Nvertices ;i++){
	fprintf(stderr,"\n\n Vertex %d :%f %f %f\n\n",i,obj->vertices[i].x,obj->vertices[i].y,obj->vertices[i].z);
	cx = cx + obj->vertices[i].x;
	cy = cy + obj->vertices[i].y;
	cz = cz + obj->vertices[i].z ;
  	}
	cx/=object.Nvertices;
	cy/=object.Nvertices;
	cz/=object.Nvertices;
	fprintf(stderr,"\n\n COM is %f %f %f\n\n",cx,cy,cz);
}
/* --------------------------------------- */
/*           initialize my data            */
/* --------------------------------------- */

void myInitData(int *argc, char **argv) {
    int  i;
    char s[1024];
    InteractionMode = MY_MENU_CAMERA;

    TX = 1.1; TY = 0.8; TZ = -2.0;

    XmouseDown = -1;
    YmouseDown = -1;
    XmouseCurrent = XmousePrev = -1;
    YmouseCurrent = YmousePrev = -1;
    DX = DY = 0;
    KeybModifiers = 0;
    /* initialize texturing */
    TextureOn = GL_TRUE;

    /* read texture from PPM file - printout command line arguments for debugging */
    fprintf(stderr,"argc = %d: ",*argc);
    for (i = 0; i< *argc; i++)
        fprintf(stderr, " arg[%d] = %s, ",i, argv[i]);
    fprintf(stderr,"\n");


	if( *argc == 1){
        printf("Reading in %s.\n", s );
      	ReadOFF("cub.off", &object);
	printf("Using default texture .\n");
	} 
   
	else if (*argc == 2) {
        /* load image from default URL */
	printf("Reading in %s.\n", argv[1]);
      	ReadOFF(argv[1], &object);
    	}
	myInitCheckers();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &TextureName);
        glBindTexture(GL_TEXTURE_2D, TextureName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MY_CHECKERS_WIDTH, MY_CHECKERS_HEIGHT, 
                     0, GL_RGB, GL_UNSIGNED_BYTE, CheckersImg);

	
}  /* myInitData() */


/* --------------------------------------- */

void myInitCheckers(void) {
    int i, j, c;
    
   for (i = 0; i < MY_CHECKERS_HEIGHT; i++) {
      for (j = 0; j <MY_CHECKERS_WIDTH; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
	
	fprintf(stderr,"\n\ni,j %d ,%d\n\n",i,j); 
		if ( ( (i >= 32 && j>=32&&i < 40 && j<40) || (i >= 8 && j>=8 &&i < 16 && j<16) || (i >= 48 && j>=48 &&i < 56 && j<56) || (i >= 32 && j>=16 && i < 40 && j<24)) || (i >= 0 && j>=48 &&i < 8 && j<56)){
	 	CheckersImg[i][j][0] = (GLubyte) 255;
         	CheckersImg[i][j][1] = (GLubyte) 0;
         	CheckersImg[i][j][2] = (GLubyte) 0;
         	CheckersImg[i][j][3] = (GLubyte) 255;
      		}
		else if ( ( (i >= 0 && j>=24&&i < 8 && j<32) || (i >= 8 && j>=48 &&i < 16 && j<56) || (i >= 16 && j>=24 &&i < 24 && j<32) || (i >= 48 && j>=56 && i < 56 && j<64)) || (i >= 56 && j>=0 &&i < 64 && j<8)){
	 	CheckersImg[i][j][0] = (GLubyte) 0;
         	CheckersImg[i][j][1] = (GLubyte) 255;
         	CheckersImg[i][j][2] = (GLubyte) 0;
         	CheckersImg[i][j][3] = (GLubyte) 255;
      		}
		else {
         	CheckersImg[i][j][0] = (GLubyte) c;
         	CheckersImg[i][j][1] = (GLubyte) c;
         	CheckersImg[i][j][2] = (GLubyte) 127;
         	CheckersImg[i][j][3] = (GLubyte) 255;
      		}
	}
   }
} /* myInitCheckers() */




/***************************************************************/
/*********************** MOUSE CALLBACKS ***********************/
/***************************************************************/

/* ------------------------------------------------------------------------ */
/* Mouse Button  UP/DOWN  or DOWN/UP transition only */
/* ------------------------------------------------------------------------ */
void myButton(int button, int state, int x, int y) {  

   XmouseDown = x;
   YmouseDown = WindHeight - y - 1;	/* Invert to right-handed system */

   /* This permits detection of left, middle, left+middle */
   if (state == GLUT_DOWN)
     mouseButtons |= button;
   else
     mouseButtons &= (~(button));

   /* bitmask with GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT */
   KeybModifiers = glutGetModifiers();


  
    if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            XmouseCurrent = XmousePrev = XmouseDown;
            YmouseCurrent = YmousePrev = YmouseDown;
        } else {
            /* at button up, invalidate: */
            XmouseCurrent = XmousePrev = XmouseDown = -1;
            YmouseCurrent = YmousePrev = YmouseDown = -1;
            KeybModifiers = 0;
        }
        DX = DY = 0;
    }
    glutPostRedisplay();
} /* myButton() */


/* ------------------------------------------------------------------------ */
/* Mouse motion WHILE BUTTON DOWN  */
/*  COMPLAINS  if you call glutGetModifiers()  here!! */
/* ------------------------------------------------------------------------ */
void myMotion(int x, int y) {    
    XmouseCurrent = x;
    YmouseCurrent = WindHeight - y - 1;	/* Invert to right-handed system */
    
    DX = XmouseCurrent - XmousePrev;
    DY = YmouseCurrent - YmousePrev;
    // printf("%d %d \n", DX, DY);

    XmousePrev = XmouseCurrent;
    YmousePrev = YmouseCurrent;

    switch(mouseButtons ){
    case 0x1:  /* Left Mouse Button   */
      break;
    case 0x2:
	  /* Middle Mouse Button */
      break;
    case 0x3:  /* Left+Middle Mouse Button */
      break;
    case 0x4:  /* Right Mouse Button (disabled with Menu)  */
      break;
    }

    glutPostRedisplay();
} /* myMotion(int x, int y) */


/* ------------------------------------------------------------------------ */
/* Handle Keyboard */
/* ------------------------------------------------------------------------ */
void myKeyboard(unsigned char key, int x, int y) {


    /*  Option: To force all lower case */
    if (key >= 'A' && key <= 'Z') key = (key-'A') + 'a';

    switch (key) {
    case 'h': fprintf(stderr,"Help: \n\
  ESC or q: quit \n\
  t: toggle texture\n\
");
      break;
        case 't':
	
            if (TextureOn==GL_TRUE) TextureOn=GL_FALSE;
            else TextureOn=GL_TRUE;
            break;
	case 's' :
	
	flag = 1;
	N = 1+(int)((3.5 - prevx)/ (0.5));
	M = 1+(int)((5.3 - prevz)/(0.75));
	gettimeofday(&tv,NULL);
	srand(tv.tv_usec*tv.tv_sec);
	//fprintf(stderr,"\n\nRandX %d\n\n",1 + (int)(8.0*(rand()/(RAND_MAX+1.0))));
	fprintf(stderr,"\n\nN %d M %d\n\n",N,M);
	fprintf(stderr,"\n\nRandX %f\n\n",randx = ((int)(N*(rand()/(RAND_MAX+1.0))))*(0.5));
	//fprintf(stderr,"\n\nRandX %f\n\n",randx = ((int)(rand()%N))*(0.5));
	/*while(1){
	if ( randx < (2.85 - prevx) ) break;
	randx = ( (int)(N*(rand()/(RAND_MAX+1.0))))*(0.5);
	}
	while(1){
	if ( randz > (2.85 - prevz) ) break;
	randz = -((int)(M*(rand()/(RAND_MAX+1.0))))*(0.75);
	}*/
	fprintf(stderr,"\n\nRandZ %f\n\n",randz = ((int)(M*(rand()/(RAND_MAX+1.0))))*(0.75));
	//fprintf(stderr,"\n\nRandX %f\n\n",randz = ((int)(rand()%M))*(0.75));
	//fprintf(stderr,"\n\nRandx %f Randz %f\n\n",randx,randz);

	prevx += randx;
	prevz += randz;
	break;
	case 'r':
	smallx=0.0;
	smallz=0.0;
	randx=0.0;
	randz=0.0;
	prevx=0.0;
	prevz=0.0;
	break;
	
	case 'q' :
	
        case  27 :  /* 27 -> ESC */
            fprintf(stderr,"Normal Exit.\n");
            exit(EXIT_SUCCESS);
            break;

        default: fprintf(stderr,"Unhandled key: %c [%d] \n",key,key);
    }

    glutPostRedisplay();
} /* myKeyboard() */




/* -------------------------------------------------------------- */
/* myMenus() - callback for GLUT menus */
/* -------------------------------------------------------------- */
void myMenus (int value) {
    switch(value) {
        case MY_MENU_RESET:
            /* reset things here */
            break;
        case MY_MENU_CAMERA:
            InteractionMode = MY_MENU_CAMERA;
            break;
        case MY_MENU_LIGHT:
            InteractionMode = MY_MENU_LIGHT;
            break;
        case MY_MENU_OBJECT:
            InteractionMode = MY_MENU_OBJECT;
            break;            
        case MY_MENU_EXIT:
            exit(0);
            break;
        default:
            //            fprintf(stderr,"unknown menu entry");
            ;
    } /* switch () */
    glutPostRedisplay(); /* make sure we Display according to changes */
} /* myMenus() */


/* -------------------------------------------------------------- */
/* initialize GLUT menus */
/* -------------------------------------------------------------- */
void myInitMenus (void) {    
    glutCreateMenu(myMenus);
    
    glutAddMenuEntry("Reset Everything",   MY_MENU_RESET);
    glutAddMenuEntry(" ", -1);    
    glutAddMenuEntry("Move Camera",        MY_MENU_CAMERA);
    glutAddMenuEntry("Move Light",         MY_MENU_LIGHT);
    glutAddMenuEntry("Move Object",        MY_MENU_OBJECT);
    glutAddMenuEntry(" ", -2);
    glutAddMenuEntry("Exit",               MY_MENU_EXIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
} /* myInitMenus() */



/***************************************************************/
/************************** INIT GL ****************************/
/***************************************************************/

void myInitGL (void) {
    GLfloat light_ambient[] = {0.1,0.1,0.1, 1.0};
    GLfloat light_diffuse[] = {1.0,1.0,1.0, 1.0};
    GLfloat light_specular[] = {1.0,1.0,1.0, 1.0};

   
    /* projection pipeline initialization */
    FOVangle = 60.0;
    FOVratio = 1.0;
    ClipNear = 0.1;
    ClipFar = 1000.0;
    XCamera = 1.2; YCamera = 2.0; 
    ZCamera = 4.0;

    WindWidth = glutGet(GLUT_WINDOW_WIDTH);
    WindHeight  = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, WindWidth, WindHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    FOVratio  = (float)WindWidth / (float)WindHeight; /* keep viewed x/y ratio == 1 */
    /* instead of glFrustum(-1.0*ratio, 1.0*ratio, -1.0,1.0, FLength, 25.0);  */
    gluPerspective(FOVangle, FOVratio, ClipNear, ClipFar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Colors, Lighting, Material  initialization */
    glClearColor( 0.4f, 0.6f, 0.5f, 1.0f);
    glPointSize(1);     glEnable(GL_POINT_SMOOTH);
    glLineWidth(1);     glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    /* we do **not** want model materials to follow the current GL color: */
    glDisable(GL_COLOR_MATERIAL);
    
    glShadeModel(GL_SMOOTH);        /* or GL_FLAT */
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    LightPos[0] = -1.0;    LightPos[1] = 2.0;
    LightPos[2] =  0.5;    LightPos[3] = 1.0;
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	
   
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

} /* myInitGL() */



/***************************************************************/
/********************** DRAW FUNCTIONS *************************/
/***************************************************************/


void myDrawTeapot(GLdouble size) {
    GLfloat materialDiffuse[] = {1.0, 1.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialDiffuse);

    if (TextureOn==GL_TRUE) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, TextureName);
        glFrontFace(GL_CW);
        glutSolidTeapot(size);
        glFrontFace(GL_CCW);        
        glDisable(GL_TEXTURE_2D);
    } else {
        glFrontFace(GL_CW);
        glutSolidTeapot(size);
        glFrontFace(GL_CCW);        
    }
} /* myDrawTeapot() */


	
void myDrawOFF(){
	//PrintObj(&object);
  	obj = &object;
	
	for(i=0; i<obj->Nfaces; i++){
	if (i == 0) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, TextureName);
	}
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
	//fprintf(stderr,"\n\n(Nx,Ny,Nz)  = (%f,%f,%f) %d\n\n",(r.x)/rlength,(r.y)/rlength,(r.z)/rlength,i);

	glNormal3f((r.x)/rlength,(r.y)/rlength,(r.z)/rlength);
	if (i == 0 && j == 0) glTexCoord2f(0.0,0.0);
	if( i == 0 && j == 1) glTexCoord2f(1.0,0.0);
	if( i == 0 && j == 2) glTexCoord2f(1.0,1.0);
	if( i == 0 && j == 3) glTexCoord2f(0.0,1.0);
	glVertex3f(obj->vertices[obj->faces[i][j]].x,obj->vertices[obj->faces[i][j]].y,obj->vertices[obj->faces[i][j]].z);
	}
   	glEnd();

	if(i == 0) glDisable(GL_TEXTURE_2D);
	
	} 
	
}

void myDrawCube(){
    GLfloat materialDiffuse[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialDiffuse);
 if (TextureOn==GL_TRUE) 
	myDrawOFF();
}

/* ------------------------------------------------------------------------ */
void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();    
 /*  if ( LA > 4.0 || LB > 3.0 || LA < -5.0 || LB < -5.0) LA = LB = 0.5;  
    LA = drand48()*3.0;
    LB = drand48()*4.0;
    LC = drand48()*3.0;
	if( rand()%2 == 0) {LA = -LA ; LB = -LB ; LC = -LC;}
*/      if ( d > -80 && d < 80) d+=10;
	//if (d   == -80) d += 10;
	//if (d  == 80) d -= 160;
 /* viewing transformation */
    if (InteractionMode == MY_MENU_CAMERA) {
        /* move camera */
        if ((KeybModifiers & GLUT_ACTIVE_SHIFT) == 0) {
            XCamera = XCamera + 0.01*(GLdouble)DX;
            YCamera = YCamera + 0.01*(GLdouble)DY;
	   
        } 
	 if ((KeybModifiers & GLUT_ACTIVE_SHIFT) != 0) {
		XCamera = XCamera + 0.01*(GLdouble)DX;
            	YCamera = YCamera + 0.01*(GLdouble)DY;
		ZCamera =  ZCamera - 0.01*(GLdouble)DX - 0.01*(GLdouble)DY;
	}
    }
    // one could also try different camera positioning techinques with:
      // gluLookAt(XCamera + smallx, YCamera , ZCamera - smallz*2,0.0,0.0,0.0,0.0,1.0,0.0);
	
  //  glTranslatef(-XCamera - LA,-YCamera - LB, -ZCamera - LC );
	  glTranslatef(-XCamera ,-YCamera , -ZCamera );
    
    /* lights positioning */
    if (InteractionMode == MY_MENU_LIGHT) {
        /* move camera */
        if ((KeybModifiers & GLUT_ACTIVE_SHIFT) == 0) {
            LightPos[0] = LightPos[0] + 0.01*(GLdouble)DX ;
            LightPos[1] = LightPos[1] + 0.01*(GLdouble)DY ;
        } 
				
    }
      	  //  LightPos[0] = LightPos[0] + smallx/50 ;
            //LightPos[1] = LightPos[1] - smallx/20 ;
	   // LightPos[0] = LightPos[1] + smallz ;
	   
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
   
    /* draw a little cube to mark the light's position: */
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(LightPos[0],LightPos[1],LightPos[2]);
    glColor3f(0.5,0.5,0.5);
    glutSolidCube(0.1);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    
    /* modeling transformations follow,  */
    /*placing 3D models in the scene */ 
    if (InteractionMode == MY_MENU_OBJECT) {
        /* move camera */
        if ((KeybModifiers & GLUT_ACTIVE_SHIFT) == 0) {
		if(mouseButtons == GLUT_LEFT_BUTTON) {
            	TX = TX + 0.01*(GLdouble)DX;
	    	TY = TY + 0.01*(GLdouble)DY;
		}
		if(mouseButtons == GLUT_MIDDLE_BUTTON) {
	
		}
        }
	 if ((KeybModifiers & GLUT_ACTIVE_SHIFT) != 0) {
		if(mouseButtons == GLUT_LEFT_BUTTON){
            	TX = TX + 0.01*(GLdouble)DX;
	    	TZ = TZ + 0.01*(GLdouble)DY;
		}
		if(mouseButtons == GLUT_MIDDLE_BUTTON){
	
		}
        }
    }
    /* Push/Pop Doesn't matter here due to LoadIdentity, but 
       could matter in more complicated scene. */
    glPushMatrix();
    //if(flag == 1) { glRotatef(90,0.0,1.0,0.0);}
    glTranslatef(TX, TY, TZ);
    //glRotated( 20 + d, 0.0,0.0,1.0);
   //myDrawTeapot(0.4);
    myDrawCube();
    glPopMatrix();

    glPushMatrix();
	GLfloat materialDiffuse1[] = {0.5, 0.0, 0.0, 1.0};
    	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialDiffuse1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, TextureName);
	glTranslatef(TX, TY, TZ);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0,-1.0,0.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-5.0,5.0,10.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(5.0,5.0,10.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(5.0,5.0,-10.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-5.0,5.0,-10.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	
	glNormal3f(0.0,0.0,1.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(5.0,-5.0,-10.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(-5.0,-5.0,-10.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(-5.0,5.0,-10.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(5.0,5.0,-10.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(-1.0,0.0,0.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(5.0,-5.0,-10.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(5.0,5.0,-10.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(5.0,5.0,10.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(5.0,-5.0,10.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(5.0,-5.0,10.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(-5.0,-5.0,10.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(-5.0,-5.0,-10.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(5.0,-5.0,-10.0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(1.0,0.0,0.0);
	glTexCoord2f(0.0,0.0);
	glVertex3f(-5.0,5.0,10.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(-5.0,5.0,-10.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(-5.0,-5.0,-10.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(-5.0,-5.0,10.0);
	glEnd();
	 glDisable(GL_TEXTURE_2D);
	/*glBegin(GL_TRIANGLE_FAN);
	glVertex3f(4.0,4.0,4.0);
	glVertex3f(-4.0,4.0,4.0);
	glVertex3f(-4.0,-4.0,4.0);
	glVertex3f(4.0,-4.0,4.0);
	glEnd();*/
    glPopMatrix();
  //  for ( i = 10; i< 15; i++){
    //if ( dispx >0.9 || dispx < -3.0) dispx = -3.0;
	//dispx += 0.1;
    //  if ( dispz >5.0 || dispz < -1.0) dispz = 5.0;
	//dispz+= 0.1;

	if (flag == 1) {
	if(smallx < prevx ) smallx+=0.05;
	else if(smallz < prevz) smallz+=0.05;
	}

	//else  flag = 0;
    glPushMatrix();
    //sleep(1);
  //  glLoadIdentity();
    //glTranslatef((float)(i/2)-0.7 ,0.9,0.7-(float)(i/2));
    //glTranslatef(-0.5,0.9,0.8); 
   // fprintf(stderr,"\n\n d %f\n\n",d);
    //fprintf(stderr,"\n\n TX %f TY %f TZ %f\n\n ",TX + d/50,TY,TZ -d/50);
	 // fprintf(stderr,"\n\n TX %f TY %f TZ %f\n\n ",TX ,TY,TZ );
  // glTranslatef(TX + d/50, TY, TZ - d/50);
	if (flag == 1)
    	glTranslatef(-0.65 + smallx ,0.8 ,0.65 - smallz);

	//glTranslatef(-0.65  ,0.8 ,0.65 );
	//glTranslatef(2.85  ,0.8 ,-4.65);
	 //glTranslatef(-0.65  ,0.8 ,-4.65);
	//glTranslatef(2.85  ,0.8 ,0.65);
	glRotatef(90,-1.0,0.0,0.0);
    //glRotatef(20 + d,1.0,0.0,0.0);
    glutSolidCone(0.2,0.4,3,3);
    glPopMatrix();    
//}
     
    glutSwapBuffers();
} /* myDisplay() */


/***************************************************************/
/*********************** GLUT FUNCTIONS ************************/
/***************************************************************/


/* ------------------------------------------------------------------------ */
void myReshape(int w, int h) {
    w = MY_MAX(w,200);
    h = MY_MAX(h,200);
    WindHeight = h;
    WindWidth = w;
    glutReshapeWindow(w,h);
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    FOVratio  = (float)w / (float)h; /* keep world coord x/y ratio == 1 */
    gluPerspective(FOVangle, FOVratio, ClipNear, ClipFar);
    glMatrixMode(GL_MODELVIEW);
} /* myReshape() */

/* ------------------------------------------------------------------------ */
void myVisibility(int state) {
/* ------------------------------------------------------------------------ */
    if (state == GLUT_VISIBLE){
        glutPostRedisplay();
    }
} /* myVisibility() */

/* ------------------------------------------------------------------------ */
