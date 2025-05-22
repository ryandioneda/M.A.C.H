#pragma once
#include "config/macroConfig.h"
#include <string>
#include <vector>
#include <windows.h>

// reads existing config file; creates if does not exist
HANDLE openConfigFile(const std::string &path);

// writes simple startup config to file
bool writeTextToFile(HANDLE hFile, const std::string &text);

// reads and pushes macros to global macros to read and displayed
void readConfigMacros(std::string &path, std::vector<MacroConfig> &macros);
