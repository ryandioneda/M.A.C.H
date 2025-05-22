#include "config/fileOps.h"
#include "config/macroConfig.h"
#include <debugapi.h>
#include <fstream>
#include <json/json.hpp>
#include <sstream>
#include <windows.h>

HANDLE openConfigFile(const std::string &path) {
  HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE | GENERIC_READ,
                             0,           // no sharing
                             nullptr,     // default security
                             OPEN_ALWAYS, // open or create
                             FILE_ATTRIBUTE_NORMAL, nullptr);
  return (hFile == INVALID_HANDLE_VALUE) ? nullptr : hFile;
}

bool writeTextToFile(HANDLE hFile, const std::string &text) {
  SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
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

  CloseHandle(hFile);
  return true;
}

using json = nlohmann::json;

void readConfigMacros(std::string &path, std::vector<MacroConfig> &macros) {
  std::ifstream in(path); // open new stream
  if (!in.is_open()) {
    OutputDebugStringA("[MANAGER] Cannot open macros.json for reading.\n");
    return;
  }

  try {
    json j;
    in >> j;
    for (const auto &entry : j) {
      if (entry.contains("keys") && entry.contains("action")) {
        macros.push_back({entry["keys"], entry["action"]});
      }
    }
    OutputDebugStringA("[MANAGER] Loaded macros.json successfully.\n");
  } catch (const std::exception &e) {
    std::ostringstream err;
    err << "[MANAGER] JSON parse error: " << e.what() << '\n';
    OutputDebugStringA(err.str().c_str());
  }
}
