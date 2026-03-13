#pragma once

#include <chrono>

namespace rh::log {

/**
 * Main chrono clock used for logging.
 * FIXME: Use something instead of C++'s disgusting <chrono>
 */
using Clock = std::chrono::system_clock;

/**
 * Localized time point
 */
using LocalTimepoint = std::chrono::local_time<Clock::duration>;

/**
 * Returns current local time (in local timezone)
 */
forceinline LocalTimepoint GetLocalTimepoint() {
  using namespace std::chrono;
  return zoned_time(current_zone(), Clock::now()).get_local_time();
}

} // namespace rh::log
