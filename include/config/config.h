#pragma once

#include <string>
#include <vector>
#include <windows.h>

struct MacroConfig {
  std::string keys;
  std::string action;
};

std::vector<MacroConfig> getMacroConfig();
HANDLE createConfigFile(const std::string &path);
bool writeConfig(HANDLE hFile, const std::string &text);
void setMacros(const std::vector<MacroConfig> &macros);
