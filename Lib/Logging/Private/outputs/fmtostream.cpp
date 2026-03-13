#include <rh/log/output.hpp>

using rh::log::FmtStreamOutput;

FmtStreamOutput::~FmtStreamOutput() = default;

void FmtStreamOutput::Print(String const& text) {
  m_stream.print("{}\n", text);
}

bool FmtStreamOutput::DoesSupportColors() {
  return false;
}
