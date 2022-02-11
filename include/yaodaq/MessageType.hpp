#ifndef YAODAQ_MESSAGETYPE
#define YAODAQ_MESSAGETYPE

/**
\copyright Copyright 2022 flagarde
*/
#include <cstdint>
#include <iosfwd>

#include "yaodaq/Interrupt.hpp"

namespace yaodaq
{

enum class MessageType : std::int_least8_t
{
  //IXWebSocket MessageType (Message is not set here)
  Open = -1,
  Close = -2,
  ConnectionError = -3,
  Ping = -4,
  Pong = -5,
  Fragment = -6,
  // Unknown should not be used !
  Unknown = 0,

};

inline std::ostream& operator<<(std::ostream& os,const MessageType& messageTypes)
{
  return os<<static_cast<std::int_least8_t>(messageTypes)+0;
}

}  // namespace yaodaq

#endif  // YAODAQ_MESSAGETYPE
