#pragma once

#ifndef RHLIB_LOG_LEVELS_FILE
#define RHLIB_LOG_LEVELS_FILE <rh/internal/DefaultLogLevels.def>
#endif

namespace rh::log {

enum class Level {
#define LOG_LEVEL(NAME) NAME,
#include RHLIB_LOG_LEVELS_FILE

#undef LOG_LEVEL
  _COUNT
};

forceinline const char* LevelToString(Level level) {
  switch (level) {
#define LOG_LEVEL(NAME)                                                                            \
    case Level::NAME:                                                                              \
      return #NAME;
#include RHLIB_LOG_LEVELS_FILE
    default:
      break;
#undef LOG_LEVEL
  }

  return "<unknown level>";
}

constexpr size_t LevelsCount = static_cast<size_t>(Level::_COUNT);

// clang-format off
/**
 * Lowest integer type that can represent "LevelsCount"-bits set
 */
using LevelsBitSet = std::conditional_t<
  (LevelsCount <= 8), uint8_t, std::conditional_t<
  (LevelsCount <= 16), uint16_t, std::conditional_t<
  (LevelsCount <= 32), uint32_t, std::conditional_t<
  (LevelsCount <= 64), uint64_t, void>>>>;
// clang-format on

static_assert(!std::is_void_v<LevelsBitSet>, "Too much log levels (64 is maximum)");

} // namespace rh::log
