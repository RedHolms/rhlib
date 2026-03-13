#pragma once

#include <Windows.h>

forceinline bool DoWin32ConSupportsColors(int stdHandle) {
  HANDLE handle = GetStdHandle(stdHandle);

  DWORD mode;
  GetConsoleMode(handle, &mode);

  return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}
