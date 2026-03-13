#include <rh/log/output.hpp>

#include "win32console.hpp"

using rh::log::ConsoleOutput;

void ConsoleOutput::Print(String const& text) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

  auto encoded = text.toSTL<wchar_t>();
  WriteConsoleW(handle, encoded.c_str(), encoded.size(), nullptr, nullptr);
  WriteConsoleW(handle, L"\n", 1, nullptr, nullptr);
}

bool ConsoleOutput::DoesSupportColors() {
  return DoWin32ConSupportsColors(STD_OUTPUT_HANDLE);
}
