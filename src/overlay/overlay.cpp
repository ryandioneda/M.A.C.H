#include "overlay/overlay.h"
#include "config/config.h"
#include "hooks/hook.h"
#include <minwindef.h>
#include <wingdi.h>
#include <winuser.h>

static HWND g_overlayWnd = nullptr;
static bool g_visible = false;
static std::vector<std::string> g_overlayLines; // for display
std::vector<MacroConfig> g_allMacros;

// FIX: DELETE brush after use with DeleteObject function
void DestoryOverlayWindow(HINSTANCE hInst) {
  if (g_overlayWnd) {
    // handle for window to be destroyed
    DestroyWindow(g_overlayWnd);
    g_overlayWnd = nullptr;
  }
}
