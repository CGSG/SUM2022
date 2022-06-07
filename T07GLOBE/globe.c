/* FILE NAME: globe.c
 * PROGRAMMER: VG4
 * DATE: 06.06.2022
 * PURPOSE: WinAPI Globe drawing application sample.
 */

#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "globe.h"

/*** Globe global data ***/

/* Globe sphere grid size */
#define GRID_W 47
#define GRID_H 28

/* Globe geometry data */
static VEC Geom[GRID_H][GRID_W];

/* Rotate point around Z axis function.
 * ARGUMENTS:
 *   - point coordinates:
 *       VEC P;
 *   - angle in degrees:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector.
 */
VEC RotateZ( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);

  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
} /* End of 'RotateZ' function */

/* Rotate point around X axis function.
 * ARGUMENTS:
 *   - point coordinates:
 *       VEC P;
 *   - angle in degrees:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector.
 */
VEC RotateX( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);

  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  NewP.X = P.X;
  return NewP;
} /* End of 'RotateX' function */

/* Globe set parameters function.
 * ARGUMENTS:
 *   - globe radius:
 *       DBL R;
 * RETURNS: None.
 */
VOID GlobeSet( DBL R )
{
  INT i, j;
  DBL theta, phi;

  /* Fill geometry array with sphere */
  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      Geom[i][j].X = 1.2 * R * sin(theta) * sin(phi);
      Geom[i][j].Y = 0.28 * R * cos(theta);
      Geom[i][j].Z = 0.8 * R * sin(theta) * cos(phi);
    }
} /* End of 'GlobeSet' function */

/* Globe draw function.
 * ARGUMENTS:
 *   - drawing device context:
 *       HDC hDC;
 *   - window size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID GlobeDraw( HDC hDC, INT W, INT H )
{
  INT i, j, s = 3;
  DBL r, t = clock() / (DBL)CLOCKS_PER_SEC;
  POINT ps[4];
  static POINT pnts[GRID_H][GRID_W];

  /* Evaluate screen radius */
  r = (W < H ? W : H) / 2;

  /* Project all points to screen */
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = Geom[i][j];

      p = RotateZ(p, t * 30);
      p = RotateX(p, t * 18);

      /* Viewport transform */
      pnts[i][j].x = (INT)((p.X * r) + W / 2);
      pnts[i][j].y = (INT)(-(p.Y * r) + H / 2);
    }

  /* By points */
  /*
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
      Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s,
                   pnts[i][j].x + s, pnts[i][j].y + s);
  */

  /* By lines */
  /*
  for (j = 0; j < GRID_W; j++)
  {
    MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
    for (i = 1; i < GRID_H; i++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  for (i = 1; i < GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 0; j < GRID_W; j++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  */

  /* By facets */
  srand(30);
  /* SetDCBrushColor(hDC, RGB(rand() & 255, rand() & 255, rand() & 255)); */
  SetDCBrushColor(hDC, RGB(220, 220, 220));
  SetDCPenColor(hDC, RGB(180, 180, 180));
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      ps[0] = pnts[i][j];
      ps[1] = pnts[i][j + 1];
      ps[2] = pnts[i + 1][j + 1];
      ps[3] = pnts[i + 1][j];

      if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) +
          (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) +
          (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
          (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) > 0)
        Polygon(hDC, ps, 4);
    }
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      ps[0] = pnts[i][j];
      ps[1] = pnts[i][j + 1];
      ps[2] = pnts[i + 1][j + 1];
      ps[3] = pnts[i + 1][j];

      if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) +
          (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) +
          (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
          (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) <= 0)
        Polygon(hDC, ps, 4);
    }
} /* End of 'GlobeDraw' function */

/* END OF 'globe.c' FILE */

