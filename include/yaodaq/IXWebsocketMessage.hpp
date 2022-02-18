#ifndef YAODAQ_IXWEBSOCKETMESSAGE
#define YAODAQ_IXWEBSOCKETMESSAGE

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/ConnectionState.hpp"
#include "yaodaq/Message.hpp"

#include <ixwebsocket/IXWebSocketCloseInfo.h>
#include <ixwebsocket/IXWebSocketErrorInfo.h>
#include <ixwebsocket/IXWebSocketMessage.h>
#include <ixwebsocket/IXWebSocketOpenInfo.h>
#include <map>
#include <memory>
#include <string>

namespace yaodaq
{

class IXMessage : public Message
{
public:
  explicit IXMessage( const MessageType& messageType );
  explicit IXMessage( const ix::WebSocketMessagePtr& msg );
  std::string getId() const;
  std::string getRemoteIp() const;
  int getRemotePort() const;
protected:
  void setConnectionStateInfos( std::shared_ptr<ConnectionState>& connectionState );
};

class Open : public IXMessage
{
public:
  explicit Open( const ix::WebSocketOpenInfo& openInfo );
  Open( const ix::WebSocketOpenInfo& openInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::string                        getURI() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string                        getProtocol() const;
};

class Close : public IXMessage
{
public:
  explicit Close( const ix::WebSocketCloseInfo& closeInfo );
  Close( const ix::WebSocketCloseInfo& closeInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::uint16_t getCode() const;
  std::string   getReason() const;
  bool          getRemote() const;
};

class Error : public IXMessage
{
public:
  explicit Error( const ix::WebSocketErrorInfo& errorInfo );
  Error( const ix::WebSocketErrorInfo& errorInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::uint16_t getRetries() const;
  double        getWaitTime() const;
  int           getHttpStatus() const;
  std::string   getReason() const;
  bool          getDecompressionError() const;
};

class Ping : public IXMessage
{
public:
  explicit Ping( const ix::WebSocketMessagePtr& ping );
  Ping( const ix::WebSocketMessagePtr& ping, std::shared_ptr<ConnectionState>& connectionState );
};

class Pong : public IXMessage
{
public:
  explicit Pong( const ix::WebSocketMessagePtr& pong );
  Pong( const ix::WebSocketMessagePtr& pong, std::shared_ptr<ConnectionState>& connectionState );
};

class Fragment : public IXMessage
{
public:
  explicit Fragment( const ix::WebSocketMessagePtr& fragment );
  Fragment( const ix::WebSocketMessagePtr& fragment, std::shared_ptr<ConnectionState>& connectionState );
};

}  // namespace yaodaq
#endif
