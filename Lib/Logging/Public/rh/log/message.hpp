#pragma once

#include <rh/log/clock.hpp>
#include <rh/log/levels.hpp>
#include <rh/String.hpp>
#include <source_location>

namespace rh::log {

/**
 * Single logging unit
 */
struct Message {
  String text;
  clock::time_point time;
  Level level;

  std::source_location source;

  struct {
    size_t id;
    void* handle;
    std::string name;
  } thread;
};

} // namespace rh::log
