#pragma once

#include <chrono>

namespace rh::log {

/**
 * Main chrono clock used for logging.
 * FIXME: Use something instead of C++'s disgusting <chrono>
 */
using clock = std::chrono::steady_clock;

} // namespace rh::log
