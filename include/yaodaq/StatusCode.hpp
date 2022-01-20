#ifndef YAODAQ_STATUSCODE
#define YAODAQ_STATUSCODE

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>

namespace yaodaq
{

enum class StatusCode : std::int_least32_t
{
  SUCCESS = 0,
  LISTEN_ERROR,
};

}

#endif
