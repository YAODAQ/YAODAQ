/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketServer.hpp"

#include "ixwebsocket/IXNetSystem.h"

namespace yaodaq
{

WebsocketServer::WebsocketServer( const std::string& name, const int& port, const std::string& host, const int& backlog, const std::size_t& maxConnections, const int& handshakeTimeoutSecs, const int& addressFamily, const std::string& type ) :
  ix::WebSocketServer( port, host, backlog, maxConnections, handshakeTimeoutSecs, addressFamily ), m_Identifier( Class::WebsocketServer, type, name )
{
  ix::initNetSystem();
}

WebsocketServer::~WebsocketServer() { ix::uninitNetSystem(); }

void WebsocketServer::loop() {}

}  // namespace yaodaq
