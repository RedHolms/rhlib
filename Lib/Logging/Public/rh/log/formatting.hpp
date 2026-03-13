#pragma once

#include <rh/log/message.hpp>

namespace rh::log {

/**
 * Abstract formatter of rh::log::Message to a String
 */
class Formatter {
public:
  virtual ~Formatter() = default;

public:
  // Format the message. "withColors" toggles ANSI color sequences.
  // Guaranteed to be called only from one thread at a time
  virtual String Format(Message const& message, bool withColors) = 0;
};

} // namespace rh::log
