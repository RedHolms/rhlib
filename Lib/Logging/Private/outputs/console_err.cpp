#include <rh/log/output.hpp>

#include "win32console.hpp"

using rh::log::ConsoleErrorOutput;

void ConsoleErrorOutput::Print(String const& text) {
  HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);

  auto encoded = text.toSTL<wchar_t>();
  WriteConsoleW(handle, encoded.c_str(), encoded.size(), nullptr, nullptr);
  WriteConsoleW(handle, L"\n", 1, nullptr, nullptr);
}

bool ConsoleErrorOutput::DoesSupportColors() {
  return DoWin32ConSupportsColors(STD_ERROR_HANDLE);
}
