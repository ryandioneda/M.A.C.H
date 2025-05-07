#include <windows.h>
#include <winuser.h>

// holds the handle to installed keyboard hook
HHOOK g_hHook = nullptr;

// hook callback
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  /**
   * Params:
   * - nCode: Tells what kind of hook event it is
   * - wParam: The message type (ex. WM_KEYDOWN, WM_KEYUP...)
   * - lParam: A pointer to a KBDLLHOOKSTRUCT (contains info about key event)
   */

  // if real keyboard action
  if (nCode == HC_ACTION) {
    // recast virtual key (vk) code
    auto kb = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    bool isCtrlKey = kb->vkCode == VK_LCONTROL || kb->vkCode == VK_RCONTROL;
    bool ctrlHeld = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) ||
                    (GetAsyncKeyState(VK_RCONTROL) & 0x8000);
    bool isQPress =
        kb->vkCode == 'Q' && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);

    if (ctrlHeld && isQPress) {
      OutputDebugStringA("[My Program] Ctrl + Q pressed. Exiting...\n");
      PostQuitMessage(0);
      return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    // watch both left and right Ctrl
    // accessing kb.vkCode
    if (isCtrlKey) {
      switch (wParam) {
      case WM_KEYDOWN:
      case WM_SYSKEYDOWN:
        OutputDebugStringA("[HOOK] Ctrl DOWN\n");
        break;
      case WM_KEYUP:
      case WM_SYSKEYUP:
        OutputDebugStringA("[HOOK] Ctrl UP\n");
        break;
      }
    }
  }
  // pass hook to next chain
  return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// program entry
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
  OutputDebugStringA("Program starting...\n");

  // instal global hook
  g_hHook = SetWindowsHookExW(WH_KEYBOARD_LL,           // id hook
                              LowLevelKeyboardProc,     // callback
                              GetModuleHandle(nullptr), // hInstance
                              0                         // hookallthreads
  );
  if (!g_hHook) {
    OutputDebugStringA("ERROR: Failed to install keyboard hook.\n");
    return -1;
  }
  OutputDebugStringA("Hook installed.\n");

  // message loop to drive hook
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // clean
  UnhookWindowsHookEx(g_hHook);
  OutputDebugStringA("Exiting message loop.\n");
  return 0;
}
