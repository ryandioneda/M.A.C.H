#pragma once
#include "config/config.h"
#include <string>
#include <vector>
#include <windows.h>

inline constexpr const char *OVERLAY_CLASS_NAME = "OverlayWindowClass";

// initialize and register window class (overlayClass)
WNDCLASS createOverlayWindowClass(HINSTANCE hInst);
void destroyOverlay(HINSTANCE hInst); // unregisters and deletes brush

// create/destroy overlay window instance (overlayWindow)
HWND createOverlayWindow(HINSTANCE hInst);
void destroyOverlayWindow();
void setOverlayHandle(HWND hwnd);

// filtering, search state, show/hide (overlayControl)
void showOverlay(bool show);
void applySearchFilter();
bool isOverlayVisible();

// WndProc and painting logic (overlayRender)
void setOverlayLines(const std::vector<std::string> &lines);
