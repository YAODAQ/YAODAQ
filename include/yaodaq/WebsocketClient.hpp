#ifndef YAODAQ_WEBSOCKETCLIENT
#define YAODAQ_WEBSOCKETCLIENT

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"
#include "yaodaq/Interrupt.hpp"
#include "yaodaq/LoggerHandler.hpp"
#include "yaodaq/Looper.hpp"

#include <ixwebsocket/IXWebSocket.h>
#include <memory>
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

class WebsocketClient : public ix::WebSocket
{
public:
  explicit WebsocketClient( const std::string& name, const std::string& type = "YAODAQWebsocketClient" );
  virtual ~WebsocketClient();
  void                            start();
  void                            stop();
  void                            loop();
  std::shared_ptr<spdlog::logger> logger() { return m_Logger.logger(); }

  virtual void onMessage( Message& message );
  virtual void onOpen( Open& open );
  virtual void onClose( Close& close );
  virtual void onError( Error& error );
  virtual void onPing( Ping& ping );
  virtual void onPong( Pong& pong );
  virtual void onFragment( Fragment& fragment );

private:
  void          onRaisingSignal();
  Identifier    m_Identifier;
  LoggerHandler m_Logger;
  Looper        m_Looper;
};

}  // namespace yaodaq

#endif
