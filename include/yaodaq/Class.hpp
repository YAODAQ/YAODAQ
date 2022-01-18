#ifndef YAODAQ_CLASS_HPP
#define YAODAQ_CLASS_HPP

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

  WebsocketServer = Module + 1,
  WebsocketClient = Module + 2,
};

}  // namespace yaodaq

#endif  // YAODAQ_CLASS_HPP
