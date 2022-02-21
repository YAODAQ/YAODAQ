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
  WRONG_NUMBER_PARAMETERS,
  MESSAGE_TYPE_UNKNOWN,
  INTERNAL_ERROR,
  CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED = 4999,
};

}  // namespace yaodaq

#endif
