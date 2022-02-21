#ifndef YAODAQ_WEBSOCKETSERVER
#define YAODAQ_WEBSOCKETSERVER

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"
#include "yaodaq/LoggerHandler.hpp"
#include "yaodaq/Looper.hpp"
#include "yaodaq/Message.hpp"

#include <ixwebsocket/IXWebSocketServer.h>
#include <map>
#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>

namespace yaodaq
{

class IXOpen;
class IXClose;
class IXError;
class IXPing;
class IXPong;
class IXFragment;

class Message;

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
  virtual void onIXOpen( IXOpen& );
  virtual void onIXClose( IXClose& );
  virtual void onIXError( IXError& );
  virtual void onIXPing( IXPing& );
  virtual void onIXPong( IXPong& );
  virtual void onIXFragment( IXFragment& );

  // YAODAQ
  virtual void onMessage( Message&, ix::WebSocket& );

  virtual void onException( MessageException& );
  virtual void onLog( Log& );
  virtual void onUserType( UserType& );

  void setVerbosity( const LogLevel& verbosity );

  void sendToLoggers( Message& message );
  void sendToLoggers( const Message& message );
  void sendToLoggers( Message& message, ix::WebSocket& webSocket );
  void sendToLoggers( const Message& message, ix::WebSocket& webSocket );

  void send( Message& message );
  void send( const Message& message );
  void send( Message& message, ix::WebSocket& webSocket );
  void send( const Message& message, ix::WebSocket& webSocket );

  // Log
  template<typename... Args> inline void trace( fmt::string_view fmt, const Args&... args ) { m_Logger.trace( fmt, args... ); }
  template<typename... Args> inline void debug( fmt::string_view fmt, const Args&... args ) { m_Logger.debug( fmt, args... ); }
  template<typename... Args> inline void info( fmt::string_view fmt, const Args&... args ) { m_Logger.info( fmt, args... ); }
  template<typename... Args> inline void warn( fmt::string_view fmt, const Args&... args ) { m_Logger.warn( fmt, args... ); }
  template<typename... Args> inline void error( fmt::string_view fmt, const Args&... args ) { m_Logger.error( fmt, args... ); }
  template<typename... Args> inline void critical( fmt::string_view fmt, const Args&... args ) { m_Logger.critical( fmt, args... ); }

private:
  // On on internal error
  void                                 ShouldNotHappens( Message& message, ix::WebSocket& webSocket );
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
