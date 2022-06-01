/* VG4, 01.06.2022 */
#include <stdio.h>
#include <windows.h>

void main( void )
{
  int x;
  char Buf[102];

  printf("Input X:");
  scanf("%d", &x);
  sprintf(Buf, "X = %d\nFirst attempt", x);
  if (MessageBox(NULL, Buf, "T00FST", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
    MessageBox(NULL, "Pressed 'Yes'", "T00FST", MB_OK | MB_ICONINFORMATION);
}