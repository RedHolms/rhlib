#pragma once

#include <fmt/os.h>
#include <rh/internal/LogUtils.hpp>
#include <stdint.h>

enum class LogLevel : uint8_t {
  Verbose,
  Debug,
  Info,
  Warning,
  Error,
  Fatal
};

class Logger {
public:
  static inline Logger* Instance = nullptr;

public:
  explicit Logger(const char* logFilePath);
  ~Logger();

public:
  void Print(LogLevel level, std::string_view text, const char* file, int line);

private:
  fmt::ostream m_file;
};

namespace Log {

#define MAKE_LEVEL_LOG(Level)                                                                      \
  template <typename... ArgsT>                                                                     \
  struct Level {                                                                                   \
    __forceinline Level(                                                                           \
      fmt::format_string<ArgsT...> fmt,                                                            \
      ArgsT&&... args,                                                                             \
      std::array<char, LogUtils::MAX_LENGTH> const& file =                                         \
        LogUtils::MakeRelPath(__builtin_FILE()),                                                   \
      int line = __builtin_LINE()                                                                  \
    ) {                                                                                            \
      Logger::Instance->Print(                                                                     \
        LogLevel::Level, fmt::format(fmt, std::forward<ArgsT>(args)...), file.data(), line         \
      );                                                                                           \
    }                                                                                              \
  };                                                                                               \
  template <typename... ArgsT>                                                                     \
  Level(auto fmt, ArgsT&&...)->Level<ArgsT...>

MAKE_LEVEL_LOG(Verbose);
MAKE_LEVEL_LOG(Debug);
MAKE_LEVEL_LOG(Info);
MAKE_LEVEL_LOG(Warning);
MAKE_LEVEL_LOG(Error);
MAKE_LEVEL_LOG(Fatal);

#undef MAKE_LEVEL_LOG

} // namespace Log
