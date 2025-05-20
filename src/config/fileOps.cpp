#include "config/fileOps.h"
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
