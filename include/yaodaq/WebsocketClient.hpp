#ifndef YAODAQ_WEBSOCKETCLIENT
#define YAODAQ_WEBSOCKETCLIENT

/**
\copyright Copyright 2022 flagarde
*/

#include "ixwebsocket/IXWebSocket.h"
#include "yaodaq/Identifier.hpp"

#include <string>

namespace yaodaq
{

class WebsocketClient : public ix::WebSocket
{
public:
  WebsocketClient( const std::string& name, const std::string& type );
  virtual ~WebsocketClient();

private:
  Identifier m_Identifier;
};

}  // namespace yaodaq

#endif
