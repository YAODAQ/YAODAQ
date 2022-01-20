#ifndef YAODAQ_SEVERITY
#define YAODAQ_SEVERITY

#include <cstdint>

/**
\copyright Copyright 2022 flagarde
*/

namespace yaodaq
{

enum class Severity : std::int_least16_t
{
  Info     = 1,
  Warning  = 10,
  Error    = 100,
  Critical = 1000,
};

}  // namespace yaodaq

#endif  // YAODAQ_SEVERITY
