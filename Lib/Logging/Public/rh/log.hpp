#pragma once

#include <rh/log/Logger.hpp>

namespace rh::log {

/**
 * Create Logger class instance
 */
void Initialize(Config& config);

/**
 * Get current Logger class instance
 */
Logger* GetLogger();

/**
 * Destroy Logger class instance
 */
void Terminate();

} // namespace rh::log

namespace Log {
#define _DECLARE_LEVEL_LOG_HELPER(LEVEL)                                                           \
  template <typename... ArgsT>                                                                     \
  struct LEVEL {                                                                                   \
    forceinline LEVEL(                                                                             \
      fmt::format_string<ArgsT...> fmt,                                                            \
      ArgsT&&... args,                                                                             \
      std::source_location source = std::source_location::current()                                \
    ) {                                                                                            \
      auto logger = rh::log::GetLogger();                                                          \
      auto text = String::FromUTF8(fmt::format(fmt, std::forward<ArgsT>(args)...));                \
      logger->Log(                                                                                 \
        rh::log::Message { .text = text,                                                           \
                           .time = rh::log::GetLocalTimepoint(),                                   \
                           .level = rh::log::Level::LEVEL,                                         \
                           .source = source,                                                       \
                           .thread = rh::log::GetCurrentThreadInfo() }                             \
      );                                                                                           \
    }                                                                                              \
  };                                                                                               \
  template <typename... ArgsT>                                                                     \
  LEVEL(auto fmt, ArgsT&&...) -> LEVEL<ArgsT...>;

#define LOG_LEVEL(LEVEL) _DECLARE_LEVEL_LOG_HELPER(LEVEL)
#include RHLIB_LOG_LEVELS_FILE
#undef LOG_LEVEL

#undef _DECLARE_LEVEL_LOG_HELPER

} // namespace Log
