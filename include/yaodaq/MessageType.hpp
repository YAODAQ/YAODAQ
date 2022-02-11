#ifndef YAODAQ_MESSAGETYPE
#define YAODAQ_MESSAGETYPE

/**
\copyright Copyright 2022 flagarde
*/
#include "yaodaq/Interrupt.hpp"

#include <cstdint>
#include <iosfwd>

namespace yaodaq
{

enum class MessageType : std::int_least16_t
{
  // IXWebSocket MessageType (Message is not set here)
  Open            = -1,
  Close           = -2,
  ConnectionError = -3,
  Ping            = -4,
  Pong            = -5,
  Fragment        = -6,
  // Unknown should not be used !
  Unknown         = 0,
};

inline std::ostream& operator<<( std::ostream& os, const MessageType& messageTypes ) { return os << static_cast<std::int_least8_t>( messageTypes ); }

}  // namespace yaodaq

#endif  // YAODAQ_MESSAGETYPE
