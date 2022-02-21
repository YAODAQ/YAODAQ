#ifndef YAODAQ_WEBSOCKETCLIENT
#define YAODAQ_WEBSOCKETCLIENT

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"
#include "yaodaq/LoggerHandler.hpp"
#include "yaodaq/Looper.hpp"
#include "yaodaq/Message.hpp"

#include <functional>
#include <iostream>
#include <ixwebsocket/IXWebSocket.h>
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

class WebsocketClient : public ix::WebSocket
{
public:
  explicit WebsocketClient( const std::string& name, const std::string& type = "YAODAQWebsocketClient" );
  virtual ~WebsocketClient();
  void        start();
  void        stop();
  void        loop();
  static void throwGeneralIfSameName( const bool& );

  // IXWebsocket
  virtual void onIXOpen( IXOpen& open );
  virtual void onIXClose( IXClose& close );
  virtual void onIXError( IXError& error );
  virtual void onIXPing( IXPing& ping );
  virtual void onIXPong( IXPong& pong );
  virtual void onIXFragment( IXFragment& fragment );

  // YAODAQ
  virtual void onMessage( Message& );

  virtual void onOpen( IXOpen& open );
  virtual void onClose( IXClose& close );
  virtual void onError( IXError& error );
  virtual void onPing( IXPing& ping );
  virtual void onPong( IXPong& pong );
  virtual void onFragment( IXFragment& fragment );
  virtual void onException( MessageException& );
  virtual void onLog( Log& );
  virtual void onUserType( UserType& );

  // Log
  template<typename... Args> inline void trace( fmt::string_view fmt, const Args&... args ) { m_Logger.trace( fmt, args... ); }
  template<typename... Args> inline void debug( fmt::string_view fmt, const Args&... args ) { m_Logger.debug( fmt, args... ); }
  template<typename... Args> inline void info( fmt::string_view fmt, const Args&... args ) { m_Logger.info( fmt, args... ); }
  template<typename... Args> inline void warn( fmt::string_view fmt, const Args&... args ) { m_Logger.warn( fmt, args... ); }
  template<typename... Args> inline void error( fmt::string_view fmt, const Args&... args ) { m_Logger.error( fmt, args... ); }
  template<typename... Args> inline void critical( fmt::string_view fmt, const Args&... args ) { m_Logger.critical( fmt, args... ); }

  ix::WebSocketSendInfo send( Message& message, const ix::OnProgressCallback& onProgressCallback = nullptr )
  {
    message.setFrom( m_Identifier );
    return ix::WebSocket::sendUtf8Text( message.dump(), onProgressCallback );
  }

  ix::WebSocketSendInfo send( const Message& message, const ix::OnProgressCallback& onProgressCallback = nullptr )
  {
    Message sent{ message };
    sent.setFrom( m_Identifier );
    return ix::WebSocket::sendUtf8Text( sent.dump(), onProgressCallback );
  }

protected:
  // send is in text mode by default
  ix::WebSocketSendInfo send( const std::string& data, bool binary = false, const ix::OnProgressCallback& onProgressCallback = m_ProgressCallBack );
  ix::WebSocketSendInfo sendBinary( const std::string& data, const ix::OnProgressCallback& onProgressCallback = m_ProgressCallBack );
  ix::WebSocketSendInfo sendBinary( const ix::IXWebSocketSendData& data, const ix::OnProgressCallback& onProgressCallback = m_ProgressCallBack );
  // does not check for valid UTF-8 characters. Caller must check that.
  ix::WebSocketSendInfo sendUtf8Text( const std::string& text, const ix::OnProgressCallback& onProgressCallback = m_ProgressCallBack );
  // does not check for valid UTF-8 characters. Caller must check that.
  ix::WebSocketSendInfo sendUtf8Text( const ix::IXWebSocketSendData& text, const ix::OnProgressCallback& onProgressCallback = m_ProgressCallBack );
  ix::WebSocketSendInfo sendText( const std::string& text, const ix::OnProgressCallback& onProgressCallback = m_ProgressCallBack );

private:
  void                                                 onRaisingSignal();
  Identifier                                           m_Identifier;
  LoggerHandler                                        m_Logger;
  Looper                                               m_Looper;
  static bool                                          m_ThrowGeneralIfSameName;
  static std::function<bool( int current, int total )> m_ProgressCallBack;
};

}  // namespace yaodaq

#endif
