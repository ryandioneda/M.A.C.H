#include "hooks/hook.h"
#include "overlay/overlay.h"
#include <debugapi.h>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
  HWND hOverlay = CreateOverlayWindow(hInst);
  setOverlayHandle(hOverlay);
  showOverlay(false);

  if (!installKeyboardHook()) {
    return -1;
  }

  OutputDebugStringA(
      "[PROGRAM] Installed Keyboard hook. Programming running...\n");
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  uninstallKeyboardHook();
  return 0;
}
