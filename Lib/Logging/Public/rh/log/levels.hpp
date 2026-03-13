#pragma once

#ifndef RHLIB_LOG_LEVELS_FILE
#define RHLIB_LOG_LEVELS_FILE <rh/internal/DefaultLogLevels.def>
#endif

namespace rh::log {

enum class Level {
#define LOG_LEVEL(NAME) NAME,
#include RHLIB_LOG_LEVELS_FILE
#undef LOG_LEVEL
};

} // namespace rh::log
