/* FILE NAME: globe.h
 * PROGRAMMER: VG4
 * DATE: 06.06.2022
 * PURPOSE: WinAPI Globe drawing declaration module.
 */

#ifndef __globe_h_
#define __globe_h_

#include <windows.h>

/* Angle transform constants */
#define PI 3.14159265358979323846

/* Base float point types */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Space vector representation type */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector components */
} VEC;

/* Globe set parameters function.
 * ARGUMENTS:
 *   - globe radius:
 *       DBL R;
 * RETURNS: None.
 */
VOID GlobeSet( DBL R );

/* Globe draw function.
 * ARGUMENTS:
 *   - drawing device context:
 *       HDC hDC;
 *   - window size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID GlobeDraw( HDC hDC, INT W, INT H );

#endif /* __globe_h_ */

/* END OF 'globe.h' FILE */
