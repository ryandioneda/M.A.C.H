#pragma once
#include "config/macroConfig.h"
#include <string>
#include <vector>
#include <windows.h>

extern HBRUSH g_bgBrush;
extern HWND g_overlayWnd;
extern bool g_visible;
extern bool g_inSearchMode;
extern std::vector<std::string> g_overlayLines;
extern std::string g_searchQuery;
extern std::vector<MacroConfig> g_allMacros;
