/* FILE NAME: t03clock.c
 * PROGRAMMER: VG4
 * DATE: 02.06.2022
 * PURPOSE: WinAPI eyes drawing windowed application sample.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My summer practice'2022 window class"

/* Forward declartion */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* Draw clock hand function */
VOID DrawHand( HDC hDC, INT X, INT Y, INT Length, INT Width, DOUBLE Angle );

/* Flip window full screen mode function */
VOID FlipFullScreen( HWND hWnd );

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
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_NO);
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
  hWnd = CreateWindow(WND_CLASS_NAME, "CGSG'2022 Primary Practice Group Clock",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    /* CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, */
    1920 * 2, 0, 1100, 700,
    NULL, NULL, hInstance, NULL);

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
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  BITMAP bm;
  HFONT hFnt, hFntOld;
  SIZE sz;
  SYSTEMTIME st;
  CHAR Buf[102];
  RECT rc;
  POINT pt;
  MINMAXINFO *minmax;
  static CHAR *WDays[] = {"ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"};
  static HBITMAP hBm, hBmClock, hBmXor, hBmAnd;
  static HDC hMemDC, hMemDCClock, hMemDCSprite;
  static INT W, H;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);

    /* Create compatible DC for background image */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCClock = CreateCompatibleDC(hDC);
    hMemDCSprite = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBmClock = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDCClock, hBmClock);
    hBmXor = LoadImage(NULL, "xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    SetStretchBltMode(hMemDC, COLORONCOLOR);

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
    /* Clear background */
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDC, RGB(0, 180, 221));
    Rectangle(hMemDC, 0, 0, W, H);

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(0, 0, 0));

    GetObject(hBmClock, sizeof(BITMAP), &bm);
    StretchBlt(hMemDC, 0, 0, W, H, hMemDCClock, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    /* Draw time */
    GetLocalTime(&st);
    DrawHand(hMemDC, W / 2, H / 2, (INT)(H / 4.7), 18, (st.wHour % 12 + st.wMinute / 60.0) * 360 / 12);
    DrawHand(hMemDC, W / 2, H / 2, (INT)(H / 3.6), 12, (st.wMinute + st.wSecond / 60.0) * 360 / 60);
    DrawHand(hMemDC, W / 2, H / 2, (INT)(H / 2.8), 8, (st.wSecond + st.wMilliseconds / 1000.0) * 360 / 60);

    /* Draw time as text */

    hFnt = CreateFont(H / 8, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      RUSSIAN_CHARSET,
      OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_SWISS, "");
    hFntOld = SelectObject(hMemDC, hFnt);

    GetLocalTime(&st);
    SetBkColor(hMemDC, RGB(220, 220, 180));
    SetBkMode(hMemDC, TRANSPARENT);

    i = wsprintf(Buf, "%02d.%02d.%d (%s) %02d:%02d:%02d",
          st.wDay, st.wMonth, st.wYear,
          WDays[st.wDayOfWeek],
          st.wHour, st.wMinute, st.wSecond);

    SetTextColor(hMemDC, RGB(30, 30, 30));
    TextOut(hMemDC, 5, 5, Buf, i);

    SetTextColor(hMemDC, RGB(130, 190, 180));
    TextOut(hMemDC, 0, 0, Buf, i);

    GetTextExtentPoint(hMemDC, Buf, i, &sz);

    rc.left = 0;
    rc.top = H * 3 / 4;
    rc.right = W;
    rc.bottom = H;
    OffsetRect(&rc, (INT)(30 * sin(clock() / 1000.0)), 0);
    DrawText(hMemDC, Buf, i, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    SelectObject(hMemDC, hFntOld);
    DeleteObject(hFnt);

    /* Draw sprite */
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    GetObject(hBmAnd, sizeof(BITMAP), &bm);
    for (i = 0; i < 47; i++)
    {
      INT x = W / 2 + (INT)((W / 2 - 30) * sin(clock() / 10000.0 - i / 10.0)), y = H / 2 + (INT)((H / 2 - 30) * sin(2 * (clock() / 10000.0 - i / 10.0)));

      SelectObject(hMemDCSprite, hBmAnd);
      BitBlt(hMemDC, x - bm.bmWidth / 2, y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemDCSprite, 0, 0, SRCAND);
      SelectObject(hMemDCSprite, hBmXor);
      BitBlt(hMemDC, x - bm.bmWidth / 2, y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemDCSprite, 0, 0, SRCINVERT);
    }

    SelectObject(hMemDCSprite, hBmAnd);
    BitBlt(hMemDC, pt.x - bm.bmWidth / 2, pt.y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemDCSprite, 0, 0, SRCAND);
    SelectObject(hMemDCSprite, hBmXor);
    BitBlt(hMemDC, pt.x - bm.bmWidth / 2, pt.y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemDCSprite, 0, 0, SRCINVERT);



    /* Copy background image to window  */
    hDC = GetDC(hWnd);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);

    /* InvalidateRect(hWnd, NULL, TRUE); */
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure?", "Quit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDYES)
      return 0;
    break;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      PostMessage(hWnd, WM_CLOSE, 0, 0);
    else if (wParam == VK_F11)
      FlipFullScreen(hWnd);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteObject(hBmClock);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCClock);
    KillTimer(hWnd, 47);
    PostMessage(hWnd, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Flip window full screen mode function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;

    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRect);

    /* Go to full screen mode */

    /* oBTAIN NEAREST MONITOR */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);

    /* Expand window */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Restore from full screen mode */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* Draw clock hand function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;
 *   - hand center position:
 *       INT X, Y;
 *   - hand length:
 *       INT Length;
 *   - hand width:
 *       INT Width;
 *   - hand angle (from up direction in degrees):
 *       DOUBLE Angle;
 * RETURNS: None.
 */
VOID DrawHand( HDC hDC, INT X, INT Y, INT Length, INT Width, DOUBLE Angle )
{
  INT i;
  DOUBLE PI = 3.14159265358979323846, si, co;
  HPEN hPen, hPenOld;
  POINT pnts[] = {{0, -Width}, {-Width, 0}, {0, Length}, {Width, 0}}, pnts1[sizeof(pnts) / sizeof(pnts[0])];


  /* Recalculate hand screen position */
  Angle *= PI / 180;
  si = sin(Angle);
  co = cos(Angle);
  for (i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pnts1[i].x = (INT)(pnts[i].x * co + pnts[i].y * si) + X;
    pnts1[i].y = -(INT)(pnts[i].y * co - pnts[i].x * si) + Y;
  }

  hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  hPenOld = SelectObject(hDC, hPen);

  Polygon(hDC, pnts1, sizeof(pnts) / sizeof(pnts[0]));

  MoveToEx(hDC, X, Y, NULL);
  LineTo(hDC, X + (INT)(Length * sin(Angle)), Y - (INT)(Length * cos(Angle)));

  SelectObject(hDC, hPenOld);
  DeleteObject(hPen);
} /* End of 'DrawHand' function */

/* END OF 't03clock.c' FILE */
