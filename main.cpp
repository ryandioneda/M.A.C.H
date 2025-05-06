#include <windows.h>
#include <iostream>

//holds the handle to installed keyboard hook
HHOOK g_hHook = nullptr;

//hook callback
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    /**
     * Params:
     * - nCode: Tells what kind of hook event it is
     * - wParam: The message type (ex. WM_KEYDOWN, WM_KEYUP...)
     * - lParam: A pointer to a KBDLLHOOKSTRUCT
     */

    //if real keyboard action
    if (nCode == HC_ACTION) {
        //recast virtual key (vk) code
        auto kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        //watch both left and right Ctrl
        if (kb->vkCode == VK_LCONTROL || kb->vkCode == VK_RCONTROL) {
            switch (wParam) {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                    std::cout << "[HOOK] Ctrl DOWN\n";
                    break;
                case WM_KEYUP:
                case WM_SYSKEYUP:
                    std::cout << "[HOOK] Ctrl   UP\n";
                    break;
            }
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}


//program entry
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    std::cout << "CheatKeysClone starting...\n";

    // instal global hook
    g_hHook = SetWindowsHookExW(
        WH_KEYBOARD_LL,  //id hook
        LowLevelKeyboardProc, //callback
        GetModuleHandle(nullptr), //hInstance
        0   //hookallthreads
    );
    if (!g_hHook) {
        std::cerr << "ERROR: Failed to install keyboard hook.\n";
        return -1;
    }
    std::cout << "Hook installed.\n";

    //message loop to drive hook
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //clean
    UnhookWindowsHookEx(g_hHook);
    std::cout << "Exiting message loop.\n";
    return 0;
}