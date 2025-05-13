#include "hooks/hook.h"
#include "input/input.h"
#include "overlay/overlay.h"
#include <debugapi.h>
#include <windows.h>

static HHOOK g_hHook = nullptr;

// search mode state vars for hook
static bool g_inSearchMode = false;
static std::string g_searchQuery;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  /**
   * Used in keyboard hook to listen for key events
   *
   * Params:
   * - nCode: Tells what kind of hook event it is
   * - wParam: The message type (ex. WM_KEYDOWN, WM_KEYUP...)
   * - lParam: A pointer to a KBDLLHOOKSTRUCT (contains info about key event)
   */

  // if real keyboard action
  if (nCode == HC_ACTION) {
    // recast virtual key (vk) code
    auto kb = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    bool isQKey =
        kb->vkCode == 'Q' && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);

    if (!isOverlayVisible() && isCtrlHeldLongEnough()) {
      OutputDebugStringA("[KEY] Showing overlay...\n");
      showOverlay(true);
    } else if (isOverlayVisible() && isEscapePressed()) {
      OutputDebugStringA("[KEY] Hiding overlay...\n");
      showOverlay(false);
    }

    if (isCtrlHeld() && isQKey) {
      OutputDebugStringA("[My Program] Ctrl + Q pressed. Ending program...\n");
      PostQuitMessage(0);
      return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    if (isOverlayVisible() && isSearchTrigger(wParam, kb)) {
      g_inSearchMode = true;
      g_searchQuery.clear();
      OutputDebugStringA("[SEARCH] Entering search mode\n");
    }
  }

  // pass hook to next chain
  return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

bool installKeyboardHook() {
  g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc,
                             GetModuleHandle(nullptr), 0);
  return g_hHook != nullptr;
}

void uninstallKeyboardHook() {
  if (g_hHook)
    UnhookWindowsHookEx(g_hHook);
}
