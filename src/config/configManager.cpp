#include "config/configManager.h"
#include "config/configPaths.h"
#include "config/fileOps.h"
#include "overlay/overlayState.h"
#include <debugapi.h>
#include <fstream>
#include <json/json.hpp>
#include <sstream>
#include <windows.h>

using json = nlohmann::json;

std::vector<MacroConfig> getMacros() {
  std::vector<MacroConfig> macros;
  auto path = getConfigPath();
  if (path.empty()) {
    OutputDebugStringA("[MANAGER] path is empty.\n");
    return macros;
  }

  std::ifstream infile(path);

  // if the file does not exist
  if (!infile.is_open()) {
    OutputDebugStringA("[MANAGER] Macros.json not found; creating default.\n");
    HANDLE hFile = openConfigFile(path.c_str());
    if (hFile != INVALID_HANDLE_VALUE) {
      json defaultConfig =
          json::array({{{"keys", "Ctr+Shift+H"}, {"action", "Show Help"}}});
      // write to config
      writeTextToFile(hFile, defaultConfig.dump(4));
    }
    // return macros;
  }

  // if the file exists, read
  try {
    json j;
    infile >> j;
    for (const auto &entry : j) {
      if (entry.contains("keys") && entry.contains("action")) {
        macros.push_back({entry["keys"], entry["action"]});
      }
    }
    OutputDebugStringA("[MANAGER] Loaded macros.json successfully\n");
  } catch (const std::exception &e) {
    std::ostringstream err;
    err << "[MANAGER] JSON parse error: " << e.what() << "\n";
    OutputDebugStringA(err.str().c_str());
  }
  return macros;
}

void setMacros(const std::vector<MacroConfig> &macros) { g_allMacros = macros; }
