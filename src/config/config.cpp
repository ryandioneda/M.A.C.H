#include "config/config.h"
#include "overlay/overlayState.h"
#include "json/json.hpp"
#include <ShlObj.h>
#include <debugapi.h>
#include <fileapi.h>
#include <fstream>
#include <handleapi.h>
#include <knownfolders.h>
#include <sstream>
#include <windows.h>
#include <winnt.h>

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

  std::string dir = base + "\\mach";

  // takes in path of directory to be created, and securityattributes, if NULL,
  // default
  CreateDirectoryA(dir.c_str(), NULL);

  return dir + "\\macros.json";
}

HANDLE createConfigFile(const std::string &path) {
  HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE | GENERIC_READ,
                             0,           // no sharing
                             nullptr,     // default security
                             OPEN_ALWAYS, // open or create
                             FILE_ATTRIBUTE_NORMAL, nullptr);
  return (hFile == INVALID_HANDLE_VALUE) ? nullptr : hFile;
}

bool writeConfig(HANDLE hFile, const std::string &text) {
  // (Optional) move to start or end as you wish:
  SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
  // truncate if you want a fresh file
  SetEndOfFile(hFile);

  const char *buf = text.data();
  size_t left = text.size();
  while (left > 0) {
    DWORD chunk = left > 0xFFFFFFFFULL ? 0xFFFFFFFFu : static_cast<DWORD>(left);
    DWORD written = 0;
    if (!WriteFile(hFile, buf, chunk, &written, nullptr)) {
      CloseHandle(hFile);
      return false;
    }
    buf += written;
    left -= written;
  }

  // done writing
  CloseHandle(hFile);
  return true;
}

std::vector<MacroConfig> getMacroConfig() {
  std::vector<MacroConfig> macros;
  std::string path = getAppDataMacroPath();
  if (path.empty()) {
    OutputDebugStringA("[CONFIG] Path is empty.\n");
    return macros;
  }

  std::ifstream infile(path);
  if (!infile.is_open()) {
    OutputDebugStringA("[CONFIG] Macros.json not found.\n");
    HANDLE hFile = createConfigFile(path);
    if (hFile) {
      // write json content
      json defaultConfig =
          json::array({{{"keys", "Ctr+Shift+H"}, {"action", "Show Help"}}});

      // write
      std::string blob = defaultConfig.dump(4);
      if (!writeConfig(hFile, blob)) {
        OutputDebugStringA("[CONFIG] Failed to write default macros.json\n");
      } else {
        OutputDebugStringA("[CONFIG] Wrote default macros.json\n");
      }
    } else {
      OutputDebugStringA("[CONFIG] Failed to create macros.json");
    }
    return macros;
  }

  try {
    json j;
    infile >> j;

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

void setMacros(const std::vector<MacroConfig> &macros) { g_allMacros = macros; }
