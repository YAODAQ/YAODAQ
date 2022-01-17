#ifndef INCLUDE_YAODAQ_CLASS_HPP_
#define INCLUDE_YAODAQ_CLASS_HPP_

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>

namespace yaodaq
{

enum class Class : std::int_least16_t
{
  Unknown = -1,
  Module  = 0,
  Browser = 100,

  WebSocketServer = Module + 1,
};

}  // namespace yaodaq

#endif  // INCLUDE_YAODAQ_CLASS_HPP_
