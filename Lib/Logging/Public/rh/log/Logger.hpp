#pragma once

#include <mutex>
#include <rh/log/config.hpp>
#include <rh/log/message.hpp>

namespace rh::log {

/**
 * Center of logging system
 */
class Logger {
  friend void Initialize(Config& config);
  friend void Terminate();

private:
  struct Rule {
    LevelsBitSet targetLevels;
    std::string outputId;
    std::string formatterId;

    forceinline bool DoesApplyToLevel(Level level) const noexcept {
      return (targetLevels & (1 << (size_t)level)) != 0;
    }
  };

  /**
   * Output pointer with mutex for per-output thread-safety
   */
  struct TSOutput {
    forceinline explicit TSOutput(std::unique_ptr<Output>&& output)
      : output(std::move(output)),
        hasColors(this->output->DoesSupportColors()) {}

    mutable std::mutex mutex;
    std::unique_ptr<Output> output;
    bool hasColors;
  };

  /**
   * Formatter pointer with mutex for per-formatter thread-safety
   */
  struct TSFormatter {
    forceinline explicit TSFormatter(std::unique_ptr<Formatter>&& formatter)
      : formatter(std::move(formatter)) {}

    mutable std::mutex mutex;
    std::unique_ptr<Formatter> formatter;
  };

private:
  explicit Logger(Config& config);
  ~Logger();

public:
  /**
   * Log a message. Thread-safe.
   */
  void Log(Message const& message) const;

private:
  bool DoMessageSatisfyRule(Message const& message, Rule const& rule) const;

private:
  std::map<std::string, TSOutput> m_outputs;
  std::map<std::string, TSFormatter> m_formatters;
  std::vector<Rule> m_rules;
};

} // namespace rh::log
