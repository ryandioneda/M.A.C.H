#pragma once

#include <string>
#include <vector>

struct MacroConfig {
  std::string keys;
  std::string action;
};

std::vector<MacroConfig> getMacroConfig();
void setMacros(const std::vector<MacroConfig> &macros);
