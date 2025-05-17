#include "overlay/overlay.h"
#include "overlay/overlayState.h"
#include "overlay/overlayWndProc.h"
#include <wingdi.h>
#include <winuser.h>

WNDCLASS createOverlayWindowClass(HINSTANCE hInst) {

  if (!g_bgBrush) {
    g_bgBrush = CreateSolidBrush(RGB(0, 0, 0));
  }

  WNDCLASS wc = {};
  wc.lpfnWndProc = OverlayWndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = OVERLAY_CLASS_NAME;
  wc.hbrBackground = g_bgBrush;
  return wc;
}

void destroyOverlay(HINSTANCE hInst) {
  UnregisterClassA(OVERLAY_CLASS_NAME, hInst);

  if (g_bgBrush) {
    DeleteObject(g_bgBrush);
    g_bgBrush = nullptr;
  }
}
