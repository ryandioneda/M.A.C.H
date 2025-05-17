#include "overlay/overlay.h"
#include "config/config.h"
#include "hooks/hook.h"
#include <minwindef.h>
#include <wingdi.h>
#include <winuser.h>

static HWND g_overlayWnd = nullptr;
static bool g_visible = false;
static HBRUSH g_bgBrush = nullptr;
static constexpr const char *OVERLAY_CLASS_NAME = "OverlayWindowClass";
static std::vector<std::string> g_overlayLines; // for display
std::vector<MacroConfig> g_allMacros;

void applySearchFilter() {
  std::vector<std::string> matches;
  for (const auto &m : g_allMacros) {
    if (g_searchQuery.empty() ||
        m.keys.find(g_searchQuery) != std::string::npos ||
        m.action.find(g_searchQuery) != std::string::npos) {
      matches.push_back(m.keys + " -> " + m.action);
    }
  }
  setOverlayLines(matches);
  InvalidateRect(g_overlayWnd, NULL, TRUE);
}

void setOverlayLines(const std::vector<std::string> &lines) {
  g_overlayLines = lines;
}

LRESULT CALLBACK OverlayWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                LPARAM lParam) {
  if (msg == WM_PAINT) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    if (g_inSearchMode) {
      std::string searchHeader = "Search: " + g_searchQuery;
      TextOutA(hdc, 50, 20, searchHeader.c_str(), searchHeader.length());
    }

    int y = 50;
    for (const auto &line : g_overlayLines) {
      TextOutA(hdc, 50, y, line.c_str(), line.length());
      y += 24;
    }
    EndPaint(hWnd, &ps);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

WNDCLASS createOverlayWindowClass(HINSTANCE hInst) {
  WNDCLASS wc = {};
  wc.lpfnWndProc = OverlayWndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = OVERLAY_CLASS_NAME;
  wc.hbrBackground = g_bgBrush;
  return wc;
}

HWND CreateOverlayWindow(HINSTANCE hInst) {
  if (!g_bgBrush) {
    g_bgBrush = CreateSolidBrush(RGB(0, 0, 0));
  }

  WNDCLASS wc = createOverlayWindowClass(hInst);
  RegisterClassA(&wc);

  HWND hWnd =
      CreateWindowEx(WS_EX_TOOLWINDOW, OVERLAY_CLASS_NAME, nullptr, WS_POPUP,
                     100, 100, 1, 1, nullptr, nullptr, hInst, nullptr);

  return hWnd;
}

void setOverlayHandle(HWND hwnd) { g_overlayWnd = hwnd; }

void showOverlay(bool show) {
  if (!g_overlayWnd)
    return;

  if (show) {
    SetForegroundWindow(g_overlayWnd);
    SetFocus(g_overlayWnd);

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
                     SWP_SHOWWINDOW);
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

// FIX: DELETE brush after use with DeleteObject function
void DestoryOverlayWindow(HINSTANCE hInst) {
  if (g_overlayWnd) {
    // handle for window to be destroyed
    DestroyWindow(g_overlayWnd);
    g_overlayWnd = nullptr;
  }
}

bool isOverlayVisible() { return g_visible; }
