/* VG4, 01.06.2022 */
#include <stdio.h>
#include <windows.h>

VOID main( VOID )
{
  INT x, y;
  CHAR Buf[102];
  RECT rc;
  HANDLE h;
  HDC hDC;

  hDC = GetDC(NULL);

  for (y = 0; y < 10; y++)
    for (x = 0; x < 1920; x++)
      SetPixelV(hDC, x + 1920 * 2, y, RGB(x, y, 255 - (x + y) / 2));

  ReleaseDC(NULL, hDC);

  printf("Input X:");
  scanf("%d", &x);
  sprintf(Buf, "X = %d\nFirst attempt", x);
  if (MessageBox(NULL, Buf, "T00FST", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
    MessageBox(NULL, "Pressed 'Yes'", "T00FST", MB_OK | MB_ICONINFORMATION);
}