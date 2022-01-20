#ifndef YAODAQ_WEBSOCKETSERVER
#define YAODAQ_WEBSOCKETSERVER

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"
#include "yaodaq/Interrupt.hpp"
#include "yaodaq/LoggerHandler.hpp"
#include "yaodaq/Looper.hpp"

#include <ixwebsocket/IXWebSocketServer.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

namespace yaodaq
{

class WebsocketServer : public ix::WebSocketServer
{
public:
  explicit WebsocketServer( const std::string& name, const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
                            const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = ix::WebSocketServer::kDefaultHandShakeTimeoutSecs, const int& addressFamily = ix::SocketServer::kDefaultAddressFamily,
                            const std::string& type = "DefaultWebsocketServer" );
  virtual ~WebsocketServer();
  void loop();
  void start();
  void stop( bool useless = true );
  void listen();

  void setVerbosity( const yaodaq::LoggerHandler::Verbosity& verbosity );

  std::shared_ptr<spdlog::logger> logger() { return m_Logger.logger(); }

private:
  void          onRaisingSignal();
  bool          m_isListening{ false };
  Identifier    m_Identifier;
  LoggerHandler m_Logger;
  Interrupt     m_Interrupt;
  Looper        m_Looper;
  bool          m_isStopped{ false };
  bool          m_isStarted{ false };
};

}  // namespace yaodaq

#endif  // YAODAQ_WEBSOCKETSERVER
