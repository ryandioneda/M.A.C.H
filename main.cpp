#include <windows.h>
#include <winuser.h>

// holds the handle to installed keyboard hook
HHOOK g_hHook = nullptr;
HWND g_hWndOverlay = nullptr;
bool g_isOverlayVisible = false;

const int HOTKEY_EXIT = VK_ESCAPE;
const DWORD CTRL_HOLD_THRESHOLD_MS = 1000; // 1 sec
                                           //
DWORD g_ctrlDownStart = 0;

bool isCtrlHeld() {
  bool ctrlIsDown = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) ||
                    (GetAsyncKeyState(VK_RCONTROL) & 0x8000);

  if (ctrlIsDown) {
    if (g_ctrlDownStart == 0) {
      g_ctrlDownStart = GetTickCount();
    }
    return (GetTickCount() - g_ctrlDownStart) >= CTRL_HOLD_THRESHOLD_MS;
  } else {
    g_ctrlDownStart = 0;
    return false;
  }
}

bool isEscPressed() { return (GetAsyncKeyState(HOTKEY_EXIT) & 0x8000); }

void showOverlayOnCurrentMonitor() {
  if (isCtrlHeld()) {

    POINT pt;
    if (GetCursorPos(&pt)) {

      HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
      MONITORINFO monitorInfo = {};
      monitorInfo.cbSize = sizeof(monitorInfo);
      if (GetMonitorInfo(hMonitor, &monitorInfo)) {
        int width = 300;
        int height = 150;
        int x =
            monitorInfo.rcWork.left +
            (monitorInfo.rcWork.right - monitorInfo.rcWork.left - width) / 2;
        int y =
            monitorInfo.rcWork.top +
            (monitorInfo.rcWork.bottom - monitorInfo.rcWork.top - height) / 2;

        SetWindowPos(g_hWndOverlay, HWND_TOPMOST, x, y, width, height,
                     SWP_NOACTIVATE | SWP_SHOWWINDOW);
      }
    }
    ShowWindow(g_hWndOverlay, SW_SHOW);
    UpdateWindow(g_hWndOverlay);
    g_isOverlayVisible = true;
  }
}

void hideOverlay() {
  ShowWindow(g_hWndOverlay, SW_HIDE);
  g_isOverlayVisible = false;
}

// hook callback
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

    // watch both left and right Ctrl
    // accessing kb.vkCode

    if (!g_isOverlayVisible && isCtrlHeld()) {
      OutputDebugStringA("[KEY] Showing overlay...\n");
      showOverlayOnCurrentMonitor();
    } else if (g_isOverlayVisible && isEscPressed()) {
      OutputDebugStringA("[KEY] Hiding overlay...\n");
      hideOverlay();
    }

    if (isCtrlHeld() && isQKey) {
      OutputDebugStringA("[My Program] Ctrl + Q pressed. Ending program...\n");
      PostQuitMessage(0);
      return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }
  }

  // pass hook to next chain
  return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK OverlayWndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                LPARAM lParam) {
  switch (msg) {
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOutA(hdc, 50, 50, "Ctrl Held", 9);
    EndPaint(hWnd, &ps);
    return 0;
  }
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
}

// window creation
HWND CreateOverlayWindow(HINSTANCE hInst) {
  const char CLASS_NAME[] = "OverlayWindowClass";

  WNDCLASS wc = {};
  wc.lpfnWndProc = OverlayWndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); // black

  RegisterClass(&wc);

  DWORD exStyle = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST;
  DWORD style = WS_POPUP;

  HWND hWnd = CreateWindowEx(exStyle, CLASS_NAME, nullptr, style, 100, 100, 300,
                             150, nullptr, nullptr, hInst, nullptr);

  SetLayeredWindowAttributes(hWnd, 0, 180, LWA_ALPHA); // semi-transparent

  return hWnd;
}

// program entry
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
  OutputDebugStringA("Program starting...\n");

  g_hWndOverlay = CreateOverlayWindow(hInst);
  ShowWindow(g_hWndOverlay, SW_HIDE);

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
