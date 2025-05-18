#include "input/input.h"
#include <windows.h>
#include <winuser.h>

static DWORD g_ctrlDownStart = 0;
static const DWORD CTRL_HOLD_THRESHOLD = 1000;

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

bool isEscapePressed(WPARAM wParam, KBDLLHOOKSTRUCT *kb) {
  return ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&
          kb->vkCode == VK_ESCAPE);
}
