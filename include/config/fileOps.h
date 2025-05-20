#pragma once
#include <string>
#include <windows.h>

// reads existing config file; creates if does not exist
HANDLE openConfigFile(const std::string &path);

// writes simple startup config to file
bool writeTextToFile(HANDLE hFile, const std::string &text);
