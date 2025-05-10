#include "config/config.h"
#include "json/json.hpp"
#include <ShlObj.h>
#include <debugapi.h>
#include <fstream>
#include <knownfolders.h>
#include <sstream>
#include <windows.h>

using json = nlohmann::json;

std::string retrieveAppDataPath() {
  /**
   * Helper function that retrieves %APPDATA%\ directory
   */
  PWSTR widePath = nullptr;

  HRESULT hr =
      SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &widePath);

  if (FAILED(hr)) {
    OutputDebugStringA("[CONFIG] Failed to get AppData path.\n");
    return "";
  }

  char appDataPath[MAX_PATH];
  WideCharToMultiByte(CP_UTF8, 0, widePath, -1, appDataPath, MAX_PATH, NULL,
                      NULL);
  CoTaskMemFree(widePath);

  return std::string(appDataPath);
}

std::string getAppDataMacroPath() {
  /**
   * Retrieves the path for config for macros.json
   *
   */
  std::string base = retrieveAppDataPath();
  if (base.empty())
    return "";

  std::string dir = base + "\\MacroKeyTool";

  // takes in path of directory to be created, and securityattributes, if NULL,
  // default
  CreateDirectoryA(dir.c_str(), NULL);

  return dir + "\\macros.json";
}

// TODO:  reads JSON config from read file (loads config)
std::vector<MacroConfig> loadMacroConfig() {
  std::vector<MacroConfig> macros;
  std::string path = getAppDataMacroPath();
  if (path.empty()) {
    OutputDebugStringA("[CONFIG] Path is empty.\n");
    return macros;
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    OutputDebugStringA("[CONFIG] Macros.json not found.\n");
    return macros;
  }

  try {
    json j;
    file >> j;

    for (const auto &entry : j) {
      if (entry.contains("keys") && entry.contains("action")) {
        macros.push_back({entry["keys"], entry["action"]});
      }
    }
    OutputDebugStringA("[CONFIG] Successfully loaded from macros.json.\n");
  } catch (const std::exception &e) {
    std::ostringstream err;
    err << "[CONFIG] JSON error: " << e.what() << "\n";
    OutputDebugStringA(err.str().c_str());
  }
  return macros;
}
