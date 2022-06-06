/* FILE NAME: t02eyes.c
 * PROGRAMMER: VG4
 * DATE: 02.06.2022
 * PURPOSE: WinAPI eyes drawing windowed application sample.
 */

#include <stdlib.h>
#include <math.h>

#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My summer practice'2022 window class"

/* Forward declartion */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* Draw eye function */
VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hWnd, hWndButton;
  MSG msg;
  WNDCLASS wc;

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = WinFunc;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd = CreateWindow(WND_CLASS_NAME, "CGSG'2022 Primary Practice Group Eyes",
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
    /* CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, */
    1920 * 2, 0, 1100, 700,
    NULL, NULL, hInstance, NULL);

  hWndButton = CreateWindow("BUTTON", "Press me", WS_CHILD | WS_BORDER | WS_VISIBLE,
      30, 30, 100, 50, hWnd, (HMENU)123, hInstance, NULL);

  SendMessage(hWndButton, BM_SETSTATE, TRUE, 0);

  /* Цикл обработки сообщений, пока не будет получено сообщение 'WM_QUIT' */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return msg.wParam;
} /* End of 'WinMain' function */

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depended return value.
 */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  INT i;
  POINT pt;
  BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDC1;
  static INT W, H;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);

    /* Create compatible DC for background image */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDC1 = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBmLogo = LoadImage(NULL, "a.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDC1, hBmLogo);

    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == 123)
      PostMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  /*
  case WM_ERASEBKGND:
    return 0;
  */
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    /* Delete old background image */
    if (hBm != NULL)
      DeleteObject(hBm);
    /* Create new background image */
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);

    /* Set new bitmap to memory DC */
    SelectObject(hMemDC, hBm);

    /* Repaint window frame */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    /* Copy background image to window  */
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_TIMER:                        
    /* Draw eyes */

    /* Clear background */
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDC, RGB(0, 180, 221));
    Rectangle(hMemDC, 0, 0, W, H);
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(0, 0, 0));

    GetObject(hBmLogo, sizeof(BITMAP), &bm);
    StretchBlt(hMemDC, 0, 0, W, H, hMemDC1, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    /* Draw eyes */
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    srand(30);
    for (i = 0; i < 102; i++)
      DrawEye(hMemDC, rand() % 1920, rand() % 1080, 30 + rand() % 47, 18 + rand() % 8, pt.x, pt.y);

    DrawEye(hMemDC, 100, 100, 90, 30, pt.x, pt.y);

    /* Copy background image to window  */
    hDC = GetDC(hWnd);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);

    /* InvalidateRect(hWnd, NULL, TRUE); */
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure?", "Quit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
      return 0;
    break;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 47);
    PostMessage(hWnd, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Draw eye function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;
 *   - eye center position:
 *       INT X, Y;
 *   - eye radiuses (big/small):
 *       INT R, R1;
 *   - mouse position:
 *       INT Mx, My;
 * RETURNS: None.
 */
VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  DOUBLE len = sqrt((Mx - X) * (Mx - X) + (My - Y) * (My - Y)), coef;

  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);

  SetDCBrushColor(hDC, RGB(0, 0, 0));
  if (len > R - R1)
  {
    coef = (R - R1) / len;
    X += (INT)((Mx - X) * coef);
    Y += (INT)((My - Y) * coef);
  }
  else
  {
    X = Mx;
    Y = My;
  }
  Ellipse(hDC, X - R1, Y - R1, X + R1, Y + R1);
} /* End of 'DrawEye' function */
