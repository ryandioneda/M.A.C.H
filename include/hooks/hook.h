#pragma once
#include <string>
extern bool g_inSearchMode;
extern std::string g_searchQuery;
bool installKeyboardHook();
void uninstallKeyboardHook();
