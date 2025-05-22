#include "config/configManager.h"
#include "config/configPaths.h"
#include "config/fileOps.h"
#include "overlay/overlayState.h"
#include <debugapi.h>
#include <fstream>
#include <json/json.hpp>
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
      bool ok = writeTextToFile(hFile, defaultConfig.dump(4));
      if (!ok) {
        OutputDebugStringA("[MANAGER] Failed to write default config.\n");
        return macros;
      }
    }
    // return macros;
  }

  // if the file exists, read and insert (opens new stream)
  readConfigMacros(path, macros);
  return macros;
}

void setMacros(const std::vector<MacroConfig> &macros) { g_allMacros = macros; }
