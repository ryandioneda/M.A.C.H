#include "overlay/overlay.h"
#include "overlay/overlayState.h"
#include <wingdi.h>
#include <winuser.h>

// create and destroy overlay window instance logic
HWND createOverlayWindow(HINSTANCE hInst) {

  WNDCLASS wc = createOverlayWindowClass(hInst);
  RegisterClassA(&wc);

  HWND hWnd =
      CreateWindowEx(WS_EX_TOOLWINDOW, OVERLAY_CLASS_NAME, nullptr, WS_POPUP,
                     100, 100, 1, 1, nullptr, nullptr, hInst, nullptr);

  return hWnd;
}

void destroyOverlayWindow() {
  if (g_overlayWnd) {
    DestroyWindow(g_overlayWnd);
    g_overlayWnd = nullptr;
  }
}

void setOverlayHandle(HWND hwnd) { g_overlayWnd = hwnd; }
