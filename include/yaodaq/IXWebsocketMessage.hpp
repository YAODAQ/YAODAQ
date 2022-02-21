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

class IXOpen : public IXMessage
{
public:
  explicit IXOpen( const ix::WebSocketOpenInfo& openInfo );
  IXOpen( const ix::WebSocketOpenInfo& openInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::string                        getURI() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string                        getProtocol() const;
};

class IXClose : public IXMessage
{
public:
  explicit IXClose( const ix::WebSocketCloseInfo& closeInfo );
  IXClose( const ix::WebSocketCloseInfo& closeInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::uint16_t getCode() const;
  std::string   getReason() const;
  bool          getRemote() const;
};

class IXError : public IXMessage
{
public:
  explicit IXError( const ix::WebSocketErrorInfo& errorInfo );
  IXError( const ix::WebSocketErrorInfo& errorInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::uint16_t getRetries() const;
  double        getWaitTime() const;
  int           getHttpStatus() const;
  std::string   getReason() const;
  bool          getDecompressionError() const;
};

class IXPing : public IXMessage
{
public:
  explicit IXPing( const ix::WebSocketMessagePtr& ping );
  IXPing( const ix::WebSocketMessagePtr& ping, std::shared_ptr<ConnectionState>& connectionState );
};

class IXPong : public IXMessage
{
public:
  explicit IXPong( const ix::WebSocketMessagePtr& pong );
  IXPong( const ix::WebSocketMessagePtr& pong, std::shared_ptr<ConnectionState>& connectionState );
};

class IXFragment : public IXMessage
{
public:
  explicit IXFragment( const ix::WebSocketMessagePtr& fragment );
  IXFragment( const ix::WebSocketMessagePtr& fragment, std::shared_ptr<ConnectionState>& connectionState );
};

}  // namespace yaodaq
#endif
