#ifndef YAODAQ_WEBSOCKETSERVER
#define YAODAQ_WEBSOCKETSERVER

/**
\copyright Copyright 2022 flagarde
*/

#include "ixwebsocket/IXWebSocketServer.h"
#include "yaodaq/Identifier.hpp"

#include <string>

namespace yaodaq
{

class WebsocketServer : public ix::WebSocketServer
{
public:
  explicit WebsocketServer( const std::string& name, const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
                            const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = ix::WebSocketServer::kDefaultHandShakeTimeoutSecs, const int& addressFamily = ix::SocketServer::kDefaultAddressFamily,
                            const std::string& type = "Default" );
  virtual ~WebsocketServer();
  void loop();

private:
  Identifier m_Identifier;
};

}  // namespace yaodaq

#endif  // YAODAQ_WEBSOCKETSERVER
