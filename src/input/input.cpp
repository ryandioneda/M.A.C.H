#include "input/input.h"
#include <windows.h>
#include <winuser.h>

static DWORD g_ctrlDownStart = 0;
const DWORD CTRL_HOLD_THRESHOLD = 1000;

bool isCtrlHeld() {
  return (GetAsyncKeyState(VK_LCONTROL) & 0x8000) ||
         (GetAsyncKeyState(VK_RCONTROL) & 0x8000);
}

bool isCtrlHeldLongEnough() {
  if (isCtrlHeld()) {
    if (g_ctrlDownStart == 0)
      g_ctrlDownStart = GetTickCount();
    return (GetTickCount() - g_ctrlDownStart) >= CTRL_HOLD_THRESHOLD;
  } else {
    g_ctrlDownStart = 0;
    return false;
  }
}

bool isEscapePressed() { return (GetAsyncKeyState(VK_ESCAPE) & 0x8000); }

bool isSearchTrigger(WPARAM wParam, KBDLLHOOKSTRUCT *kb) {
  return (kb->vkCode == VK_OEM_2) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) &&
         (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
}
