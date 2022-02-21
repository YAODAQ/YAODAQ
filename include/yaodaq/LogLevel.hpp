#ifndef YAODAQ_LOGLEVEL
#define YAODAQ_LOGLEVEL

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>

namespace yaodaq
{

enum class LogLevel : std::uint_least16_t
{
  Off,
  Trace,
  Debug,
  Info,
  Warn,
  Error,
  Critical
};

}  // namespace yaodaq

#endif
