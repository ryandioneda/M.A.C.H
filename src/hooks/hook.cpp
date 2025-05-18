#include "hooks/hook.h"
#include "input/input.h"
#include "overlay/overlay.h"
#include "overlay/overlayState.h"
#include <debugapi.h>
#include <windows.h>
#include <winuser.h>

static HHOOK g_hHook = nullptr;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  /**
   * Used in keyboard hook to listen for key events
   *
   * Params:
   * - nCode: Tells what kind of hook event it is
   * - wParam: The message type (ex. WM_KEYDOWN, WM_KEYUP...)
   * - lParam: A pointer to a KBDLLHOOKSTRUCT (contains info about key event)
   */
  if (nCode != HC_ACTION)
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);

  auto kb = (KBDLLHOOKSTRUCT *)lParam;

  if (!isOverlayVisible() && isCtrlHeldLongEnough()) {
    OutputDebugStringA("[OVERLAY] Showing overlay...\n");
    showOverlay(true);
    g_inSearchMode = true;
    g_searchQuery.clear();
    applySearchFilter();
    return 1;
  }

  if (isCtrlHeld() && kb->vkCode == 'Q') {
    OutputDebugStringA("[OVERLAY] Quitting Macro Program...\n");
    PostQuitMessage(0);
    return 1;
  }

  if (isOverlayVisible()) {

    if (isEscapePressed(wParam, kb)) {
      OutputDebugStringA("[OVERLAY] Hiding overlay...\n");
      g_inSearchMode = false;
      g_searchQuery.clear();
      showOverlay(false);
      return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    // if (wParam == WM_KEYDOWN && kb->vkCode == VK_OEM_2 &&
    //     (GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
    //   OutputDebugStringA("[OVERLAY] Triggering search mode...\n");
    //   g_inSearchMode = true;
    //   g_searchQuery.clear();
    //   applySearchFilter();
    //   return 1;
    // }

    if (g_inSearchMode && wParam == WM_KEYDOWN) {
      if (kb->vkCode == VK_BACK && !g_searchQuery.empty()) {
        OutputDebugStringA("[OVERLAY] Popping char...\n");
        g_searchQuery.pop_back();
        applySearchFilter();
        return 1;
      }
      if ((kb->vkCode >= 'A' && kb->vkCode <= 'Z') ||
          (kb->vkCode >= '0' && kb->vkCode <= '9') || kb->vkCode == VK_SPACE) {
        bool shift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
        char c = shift ? char(kb->vkCode) : char(std::tolower(kb->vkCode));
        g_searchQuery.push_back(c);
        applySearchFilter();
        return 1;
      }
      // all other keys fall through to next hook
      return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }
    // not in search mode - pass along
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
  }
  // overlay is not shown, pass along
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
