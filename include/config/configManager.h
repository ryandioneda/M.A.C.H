#pragma once
#include "macroConfig.h"
#include <vector>

// gets macros; writes default if missing
std::vector<MacroConfig> getMacros();
void setMacros(const std::vector<MacroConfig> &macros);
