#include "config/config.h"
#include "hooks/hook.h"
#include "overlay/overlay.h"
#include <debugapi.h>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
  auto macros = getMacroConfig();
  std::vector<std::string> displayLines;

  for (const auto &macro : macros) {
    std::string obj = "[MACRO] " + macro.keys + " -> " + macro.action + "\n";
    displayLines.push_back(macro.keys + " -> " + macro.action);
    OutputDebugStringA(obj.c_str());
  }
  setOverlayLines(displayLines);
  HWND hOverlay = createOverlayWindow(hInst);
  setOverlayHandle(hOverlay); // g_overlayWnd = hOverlay
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

  // TODO: destroy window handle (hOverlay)
  destroyOverlayWindow();

  // TODO: unregister overlay window class
  destroyOverlay(hInst);

  return 0;
}
