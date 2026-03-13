#include <rh/log/output.hpp>

using rh::log::FmtStreamOutput;

FmtStreamOutput::~FmtStreamOutput() = default;

void FmtStreamOutput::Print(String const& text) {
  m_stream.print("{}", text);
}

bool FmtStreamOutput::DoesSupportColors() {
  return false;
}
