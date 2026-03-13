#pragma once

#include <fmt/os.h>
#include <rh/String.hpp>

namespace rh::log {

/**
 * Abstract output for log lines (i.e. file or console). Outputs already formatted messages.
 */
class Output {
public:
  virtual ~Output() = default;

public:
  // Output formatted line. "text" will NOT have line feed at the end.
  virtual void Print(String const& text) = 0;

  // Does this output support ANSI colors? (i.e. console)
  virtual bool DoesSupportColors() = 0;
};

/**
 * Implementation of LoggingOutput that does not log anything
 */
class EmptyOutput final : public Output {
public:
  constexpr EmptyOutput() = default;
  inline ~EmptyOutput() override = default;

public:
  void Print(String const& text) override;
  bool DoesSupportColors() override;
};

/**
 * Implementation of LoggingOutput that logs lines to the stdout of the process
 */
class ConsoleOutput final : public Output {
public:
  constexpr ConsoleOutput() = default;
  inline ~ConsoleOutput() override = default;

public:
  void Print(String const& text) override;
  bool DoesSupportColors() override;
};

/**
 * Implementation of LoggingOutput that logs lines to the stderr of the process
 */
class ConsoleErrorOutput final : public Output {
public:
  constexpr ConsoleErrorOutput() = default;
  inline ~ConsoleErrorOutput() override = default;

public:
  void Print(String const& text) override;
  bool DoesSupportColors() override;
};

/**
 * Implementation of LoggingOutput that logs lines to an object of type fmt::ostream
 */
class FmtStreamOutput final : public Output {
public:
  inline explicit FmtStreamOutput(fmt::ostream&& stream) : m_stream(std::move(stream)) {}
  ~FmtStreamOutput() override;

public:
  void Print(String const& text) override;
  bool DoesSupportColors() override;

private:
  fmt::ostream m_stream;
};

} // namespace rh::log
