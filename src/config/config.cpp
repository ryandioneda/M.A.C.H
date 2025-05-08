#include "config.h"

#include <ShlObj.h>
#include <fileapi.h>
#include <fstream>
#include <knownfolders.h>
#include <minwindef.h>
#include <shlobj_core.h>
#include <sstream>
#include <stringapiset.h>
#include <windows.h>

std::string retrieveAppDataPath() {
  /**
   * Helper function that retrieves %APPDATA%\ directory
   */
  PWSTR widePath = nullptr;

  HRESULT hr = (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL,
                                     &widePath) != S_OK);

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

// TODO:  build path function (%APPDATA%\MacroKeyTool\macros.json)
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
}

// TODO:  reads JSON config from read file (loads config)
