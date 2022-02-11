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

class WebsocketClient : public ix::WebSocket
{
public:
  explicit WebsocketClient( const std::string& name, const std::string& type = "YAODAQWebsocketClient" );
  virtual ~WebsocketClient();
  void                            start();
  void                            stop();
  void                            loop();
  std::shared_ptr<spdlog::logger> logger() { return m_Logger.logger(); }

private:
  void          onRaisingSignal();
  Identifier    m_Identifier;
  LoggerHandler m_Logger;
  Looper        m_Looper;
};

}  // namespace yaodaq

#endif
