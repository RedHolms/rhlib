#include <rh/log/output.hpp>

#include "win32console.hpp"

using rh::log::ConsoleErrorOutput;

void ConsoleErrorOutput::Print(String const& text) {
  auto encoded = text.toSTL<wchar_t>();
  WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), encoded.c_str(), encoded.size(), nullptr, nullptr);
}

bool ConsoleErrorOutput::DoesSupportColors() {
  return DoWin32ConSupportsColors(STD_ERROR_HANDLE);
}
