#include "hooks/hook.h"
#include "input/input.h"
#include "overlay/overlay.h"
#include <debugapi.h>
#include <windows.h>
#include <winuser.h>

static HHOOK g_hHook = nullptr;

// search mode state vars for hook
bool g_inSearchMode = false;
std::string g_searchQuery = "";

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
    } else if (kb->vkCode == VK_BACK && !g_searchQuery.empty()) {
      g_searchQuery.pop_back();
    } else if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&
               kb->vkCode >= 'A' && kb->vkCode <= 'Z') {
      bool isShift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
      char c = isShift ? static_cast<char>(kb->vkCode)
                       : std::tolower(static_cast<char>(kb->vkCode));
      g_searchQuery.push_back(c);
    }
    applySearchFilter(); // update overlay lines
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
