/* defs.h
 *
 *  Computer Graphics
 *  B481/B581
 *  A.J. Hanson
 *  Indiana University
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Any C headers needing export to C++ linker go here. */


#ifdef _WIN32
#include <windows.h>
#else
#endif

  /*  #ifndef __gl_h_
      #include <GL/gl.h>
      #endif
   */


/* Windows forgot about pi. */
#ifndef M_PI
#define M_PI            3.1415926535897932384626433832795
#endif


/* Windows forgot bool. */
#ifndef _WIN32
#ifndef bool
#define bool	int
#endif
#endif




/***********************************************************************/
#define Sign(A) ((A) < 0.0 ? -1.0 : ((A) > 0.0 ? 1.0 : 0.0))
#define Min(A,B) ((A) <= (B) ? (A) : (B))
#define Max(A,B) ((A) >= (B) ? (A) : (B))
#define Inbetween(A,X,B) ((Min(A,B) <= (X)) && ((X) <= Max(A,B)))
/***********************************************************************/


#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


/* Vertex data structure definitions */
typedef struct {int x; int y;} ivert2d;
typedef struct {double x; double y;} vert2d;
typedef struct {double x; double y; double z;} vert3d;



#ifdef __cplusplus
}
#endif
