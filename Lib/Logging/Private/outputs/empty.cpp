#include <rh/log/output.hpp>

using rh::log::EmptyOutput;

void EmptyOutput::Print(String const& text) {
  (void)text;
}

bool EmptyOutput::DoesSupportColors() {
  return false;
}
