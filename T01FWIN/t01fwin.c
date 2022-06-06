/* t01fwin.c
 * ������ �������� ������ ��������� ��� WinAPI.
 * �������� � ��������� ����������� ����.
 */

#include <stdlib.h>

#include <windows.h>

#define WND_CLASS_NAME "My summer practice'2022 window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 * ���������:
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 *   - ���� ������ ���� (��. SW_***):
 *       INT ShowCmd;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hWnd, hWndButton;
  MSG msg;
  WNDCLASS wc;

  /* ��������� ��������� ������ ���� */
  wc.style = CS_HREDRAW | CS_VREDRAW; /* ����� ���� */
  wc.cbClsExtra = 0;         /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0;         /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* ������� ����-��������� � ������� */
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);   /* �������� ����������� (���������) */
  wc.lpszMenuName = NULL;                  /* ��� ������� ���� */
  wc.hInstance = hInstance;    /* ���������� ����������, ��������������� ����� */
  wc.lpszClassName = WND_CLASS_NAME;       /* ��� ������ */
  wc.lpfnWndProc = MyWindowFunc;   /* ��������� �� ������� ��������� ��������� */

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR",
      MB_OK | MB_ICONERROR);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME, "CGSG'SummerPractice'2022'Primary First Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
      1920 * 2, 0, 500, 200, NULL, NULL, hInstance, NULL);

  hWndButton = CreateWindow("BUTTON", "Press me", WS_CHILD | WS_BORDER | WS_VISIBLE,
      30, 30, 100, 50, hWnd, (HMENU)123, hInstance, NULL);

  SendMessage(hWndButton, BM_SETSTATE, TRUE, 0);

  /* ���� ��������� ���������, ���� �� ����� �������� ��������� 'WM_QUIT' */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return msg.wParam;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  HBRUSH hbr, hbrold;
  HPEN hpen, hpenold;
  INT i;
  POINT pt;
  static INT W, H;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);
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
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SetDCBrushColor(hDC, RGB(0, 180, 221));
    Rectangle(hDC, 0, 0, W, H);


    hbr = CreateSolidBrush(RGB(255, 0, 0));
    hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));

    hbrold = SelectObject(hDC, hbr);
    hpenold = SelectObject(hDC, hpen);

    Ellipse(hDC, 10, 10, 300, 200);

    SelectObject(hDC, GetStockObject(BLACK_BRUSH));
    SelectObject(hDC, GetStockObject(NULL_PEN));

    Rectangle(hDC, 50, 90, 400, 300);

    SelectObject(hDC, GetStockObject(NULL_BRUSH));
    SelectObject(hDC, GetStockObject(BLACK_PEN));
    Ellipse(hDC, 0, 0, W, H);


    SelectObject(hDC, hbrold);
    SelectObject(hDC, hpenold);

    DeleteObject(hpen);
    DeleteObject(hbr);

    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SelectObject(hDC, GetStockObject(NULL_PEN));

    srand(30);
    for (i = 0; i < 300; i++)
    {
      INT x = rand() % W, y = rand() % H, r = rand() % 90 + 8;

      SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
      Ellipse(hDC, x - r, y - r, x + r, y + r);
    }

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    SetDCBrushColor(hDC, RGB(220, 130, 50));
    Ellipse(hDC, pt.x - 30, pt.y - 30, pt.x + 30, pt.y + 30);
    ReleaseDC(hWnd, hDC);
    /* InvalidateRect(hWnd, NULL, TRUE); */
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure?", "Quit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
      return 0;
    break;
  case WM_DESTROY:
    KillTimer(hWnd, 47);
    PostMessage(hWnd, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */