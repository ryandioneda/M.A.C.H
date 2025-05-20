#include "config/configPaths.h"
#include <ShlObj.h>
#include <windows.h>

std::string getAppDataPath() {
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

std::string getConfigPath() {
  std::string base = getAppDataPath();
  if (base.empty())
    return "";

  std::string dir = base + "\\mach";

  // takes in path of directory to be created, and securityattributes, if NULL,
  // default
  CreateDirectoryA(dir.c_str(), NULL);

  return dir + "\\macros.json";
}
