#include <rh/log/output.hpp>

#include "win32console.hpp"

using rh::log::ConsoleOutput;

void ConsoleOutput::Print(String const& text) {
  auto encoded = text.toSTL<wchar_t>();
  WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), encoded.c_str(), encoded.size(), nullptr, nullptr);
}

bool ConsoleOutput::DoesSupportColors() {
  return DoWin32ConSupportsColors(STD_OUTPUT_HANDLE);
}
