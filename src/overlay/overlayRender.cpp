#include "overlay/overlay.h"
#include "overlay/overlayState.h"
#include "overlay/overlayWndProc.h"

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
