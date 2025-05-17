#include "overlay/overlay.h"
#include "overlay/overlayState.h"
#include <winuser.h>

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

bool isOverlayVisible() { return g_visible; }
