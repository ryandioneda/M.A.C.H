#pragma once
#include "config/config.h"
#include <string>
#include <vector>
#include <windows.h>

extern std::string g_searchQuery;
extern std::vector<MacroConfig> g_allMacros;
void setOverlayLines(const std::vector<std::string> &lines);
HWND CreateOverlayWindow(HINSTANCE hInst);
void setOverlayHandle(HWND hwnd);
void showOverlay(bool show);
bool isOverlayVisible();
void applySearchFilter();
