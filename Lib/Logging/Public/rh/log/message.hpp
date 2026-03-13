#pragma once

#include <rh/log/clock.hpp>
#include <rh/log/levels.hpp>
#include <rh/log/thread.hpp>
#include <rh/String.hpp>
#include <source_location>

namespace rh::log {

/**
 * Single logging unit
 */
struct Message {
  String text;
  LocalTimepoint time;
  Level level;
  std::source_location source;
  ThreadInfo thread;
};

} // namespace rh::log
