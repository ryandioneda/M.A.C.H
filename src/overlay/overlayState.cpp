#include "overlay/overlayState.h"
#include <string>
#include <vector>
#include <windows.h>

HBRUSH g_bgBrush = nullptr;
HWND g_overlayWnd = nullptr;
bool g_visible = false;
bool g_inSearchMode = false;
std::vector<std::string> g_overlayLines;
std::string g_searchQuery;
std::vector<MacroConfig> g_allMacros;
