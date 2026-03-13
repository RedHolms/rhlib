#include <fmt/chrono.h>
#include <iterator>
#include <rh/log.hpp>

class MyLogFormatter final : public rh::log::Formatter {
public:
  String Format(rh::log::Message const& message, bool withColors) override {
    using namespace std::chrono;

    std::string result;

    auto out = std::back_inserter(result);

    if (withColors)
      fmt::format_to(out, "\x1B[90m");

    fmt::format_to(
      out,
      "[{:%H:%M:%S}] ({}/{}) ",
      floor<microseconds>(message.time),
      message.thread.name,
      rh::log::LevelToString(message.level)
    );

    if (withColors)
      fmt::format_to(out, "\x1B[39m");

    fmt::format_to(out, "{}", message.text);

    return String::FromUTF8(result);
  }
};

int main() {
  (void)SetThreadDescription(GetCurrentThread(), L"App Main Thread");

  rh::log::Initialize(
    rh::log::Config()
      .AddStdOutOutput("console")
      .AddFileOutput("file", "example.log")
      .AddFormatter<MyLogFormatter>("my_formatter")
      .Rule()
        .ForAnyLevel()
        .Output("console")
        .Formatter("my_formatter")
      .End()
      .Rule()
        .ForAnyLevel()
        .Output("file")
        .Formatter("my_formatter")
      .End()
  );

  Log::Verbose("Verbose {}", 1);
  Log::Debug("Hello!");
  Log::Info("World");
  Log::Fatal("We have troubles...");

  rh::log::Terminate();
  return 0;
}
