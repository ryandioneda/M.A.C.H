#pragma once
#include <windows.h>

HWND CreateOverlayWindow(HINSTANCE hInst);
void setOverlayHandle(HWND hwnd);
void showOverlay(bool show);
bool isOverlayVisible();
