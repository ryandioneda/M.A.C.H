#pragma once
#include <string>
#include <vector>
#include <windows.h>

void setOverlayLines(const std::vector<std::string> &lines);
HWND CreateOverlayWindow(HINSTANCE hInst);
void setOverlayHandle(HWND hwnd);
void showOverlay(bool show);
bool isOverlayVisible();
