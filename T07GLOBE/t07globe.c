/* FILE NAME: t07globe.c
 * PROGRAMMER: VG4
 * DATE: 06.06.2022
 * PURPOSE: WinAPI Globe drawing application sample.
 */

#include <stdio.h>
#include <time.h>

#include <windows.h>

#include "globe.h"

/* Window class name */
#define WND_CLASS_NAME "My Window Class Name"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
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
  hWnd = CreateWindow(WND_CLASS_NAME, "CGSG'SummerPractice'2022'Primary Globe Drawing",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    /* CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, */
    1920 * 2, 0, 1100, 700,
    NULL, NULL, hInstance, NULL);

  /* Message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
} /* End of 'WinMain' function */

/* Toggle window fullscreen mode function.
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
    MONITORINFO moninfo;
    RECT rc;

    IsFullScreen = TRUE;

    /* Goto fullscreen mode */

    /* Store current window size and position */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    IsFullScreen = FALSE;

    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */

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
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static HFONT hFnt, hFntOld;
  static HDC hMemDCFrame;
  static HBITMAP hBmFrame;
  static INT W, H;
  static CHAR Buf[102];

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBmFrame = NULL;
    GlobeSet(1.0);
    return 0;

  case WM_SIZE:
    /* Obtain new window width and height */
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    if (hFnt != NULL)
      DeleteObject(hFnt);
    hFnt = CreateFont((W < H ? W : H) / 28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_SWISS | VARIABLE_PITCH, "Consolas");
    SelectObject(hMemDCFrame, hFnt);

    /* Recreate background frame image */
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDCFrame, hBmFrame);

    /* Redraw frame */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    /* Clear background */
    SelectObject(hMemDCFrame, GetStockObject(GRAY_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    Rectangle(hMemDCFrame, 0, 0, W + 1, H + 1);

    /* Draw globe */
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SetDCPenColor(hMemDCFrame, RGB(0, 0, 0));
    SetDCBrushColor(hMemDCFrame, RGB(255, 255, 255));
    GlobeDraw(hMemDCFrame, W, H);

    /* Output text sample */
    SetBkMode(hMemDCFrame, TRANSPARENT);
    SetTextColor(hMemDCFrame, RGB(255, 255, 255));
    TextOut(hMemDCFrame, 8, 8, Buf, sprintf(Buf, "Time: %.3f", clock() / (DBL)CLOCKS_PER_SEC));

    /* Send repaint message */
    /* InvalidateRect(hWnd, NULL, FALSE); */
    hDC = GetDC(hWnd);
    /* Copy frame to window */
    BitBlt(hDC, 0, 0, W, H, hMemDCFrame, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    /* Copy frame to window */
    BitBlt(hDC, 0, 0, W, H, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (wParam == VK_F11)
      FlipFullScreen(hWnd);
    return 0;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
    {
      FlipFullScreen(hWnd);
      return 0;
    }
    break;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to exit?", "Exit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES)
      break;
    return 0;
  case WM_DESTROY:
    /* Delete resourses */
    if (hFnt != NULL)
      DeleteObject(hFnt);
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    DeleteDC(hMemDCFrame);
    KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 't07globe.c' FILE */
