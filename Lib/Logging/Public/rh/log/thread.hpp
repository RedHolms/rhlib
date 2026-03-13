#pragma once

#include <rh/String.hpp>

namespace rh::log {

struct ThreadInfo {
  size_t id;
  void* handle;
  String name;
};

ThreadInfo GetCurrentThreadInfo();

} // namespace rh::log
