#include <rh/log.hpp>

using rh::log::Logger;

static Logger* sLoggerInstance = nullptr;

void rh::log::Initialize(Config& config) {
  if (sLoggerInstance != nullptr)
    return;

  sLoggerInstance = new Logger(config);
}

Logger* rh::log::GetLogger() {
  return sLoggerInstance;
}

void rh::log::Terminate() {
  delete sLoggerInstance;
  sLoggerInstance = nullptr;
}
