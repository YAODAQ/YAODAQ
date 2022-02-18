#ifndef YAODAQ_WEBSOCKETSERVER
#define YAODAQ_WEBSOCKETSERVER

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"
#include "yaodaq/LoggerHandler.hpp"
#include "yaodaq/Looper.hpp"

#include <ixwebsocket/IXWebSocketServer.h>
#include <map>
#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>

namespace yaodaq
{

class Message;
class Open;
class Close;
class Error;
class Ping;
class Pong;
class Fragment;
class MessageException;

class WebsocketServer : public ix::WebSocketServer
{
public:
  explicit WebsocketServer( const std::string& name, const int& port = ix::SocketServer::kDefaultPort, const std::string& host = ix::SocketServer::kDefaultHost, const int& backlog = ix::SocketServer::kDefaultTcpBacklog,
                            const std::size_t& maxConnections = ix::SocketServer::kDefaultMaxConnections, const int& handshakeTimeoutSecs = ix::WebSocketServer::kDefaultHandShakeTimeoutSecs, const int& addressFamily = ix::SocketServer::kDefaultAddressFamily,
                            const std::string& type = "YAODAQWebsocketServer" );
  virtual ~WebsocketServer();
  void loop();
  void start();
  void stop( bool useless = true );
  void listen();

  // IXWebsocket
  virtual void onMessage( Message& message );
  virtual void onOpen( Open& open );
  virtual void onClose( Close& close );
  virtual void onError( Error& error );
  virtual void onPing( Ping& ping );
  virtual void onPong( Pong& pong );
  virtual void onFragment( Fragment& fragment );

  virtual void onException(MessageException& );
  virtual void onUnknown(Message&);

  void setVerbosity( const yaodaq::LoggerHandler::Verbosity& verbosity );

  std::shared_ptr<spdlog::logger> logger() { return m_Logger.logger(); }

  void sendToLoggers( Message& message );
  void sendToLoggers( const Message& message );
  void sendToLoggers( Message& message, ix::WebSocket& webSocket );
  void sendToLoggers( const Message& message, ix::WebSocket& webSocket );

private:
  void                                 addClient( const Identifier&, ix::WebSocket& );
  void                                 removeClient( ix::WebSocket& );
  void                                 onRaisingSignal();
  bool                                 m_isListening{ false };
  Identifier                           m_Identifier;
  LoggerHandler                        m_Logger;
  Looper                               m_Looper;
  bool                                 m_isStopped{ false };
  bool                                 m_isStarted{ false };
  std::map<Identifier, ix::WebSocket&> m_Clients;
  std::mutex                           m_Mutex;
};

}  // namespace yaodaq

#endif  // YAODAQ_WEBSOCKETSERVER
