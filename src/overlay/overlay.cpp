#include "overlay/overlay.h"

static HWND g_overlayWnd = nullptr;
static bool g_visible = false;

LRESULT CALLBACK OverlayWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                LPARAM lParam) {
  if (msg == WM_PAINT) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOutA(hdc, 50, 50, "Ctrl Held", 9);
    EndPaint(hWnd, &ps);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND CreateOverlayWindow(HINSTANCE hInst) {
  const char CLASS_NAME[] = "OverlayWindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = OverlayWndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  RegisterClass(&wc);

  HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
                             CLASS_NAME, nullptr, WS_POPUP, 100, 100, 1, 1,
                             nullptr, nullptr, hInst, nullptr);

  SetLayeredWindowAttributes(hWnd, 0, 180, LWA_ALPHA);
  return hWnd;
}

void setOverlayHandle(HWND hwnd) { g_overlayWnd = hwnd; }

void showOverlay(bool show) {
  if (!g_overlayWnd)
    return;

  if (show) {

    POINT pt;
    if (GetCursorPos(&pt)) {

      HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
      MONITORINFO monitorInfo = {};
      monitorInfo.cbSize = sizeof(monitorInfo);
      if (GetMonitorInfo(hMonitor, &monitorInfo)) {
        int monitorWidth =
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        int monitorHeight =
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

        int width = monitorWidth * 0.8;
        int height = monitorHeight * 0.8;

        int x = monitorInfo.rcMonitor.left + (monitorWidth - width) / 2;
        int y = monitorInfo.rcMonitor.top + (monitorHeight - height) / 2;

        SetWindowPos(g_overlayWnd, HWND_TOPMOST, x, y, width, height,
                     SWP_NOACTIVATE | SWP_SHOWWINDOW);
      }
    }
    ShowWindow(g_overlayWnd, SW_SHOW);
    UpdateWindow(g_overlayWnd);
    g_visible = true;
  } else {
    ShowWindow(g_overlayWnd, SW_HIDE);
    g_visible = false;
  }
}

bool isOverlayVisible() { return g_visible; }
