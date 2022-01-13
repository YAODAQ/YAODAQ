#ifndef YAODAQ_CLASS_H_
#define YAODAQ_CLASS_H_

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

}

#endif
