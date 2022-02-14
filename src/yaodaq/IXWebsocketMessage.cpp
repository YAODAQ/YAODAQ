/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/IXWebsocketMessage.hpp"

namespace yaodaq
{

IXMessage::IXMessage( const MessageType& messageType ) : Message( messageType ) {}

void IXMessage::setConnectionStateInfos( std::shared_ptr<ConnectionState>& connectionState )
{
  nlohmann::json j = getContent();
  j["id"]          = connectionState->getId();
  j["remote_ip"]   = connectionState->getRemoteIp();
  j["remote_port"] = connectionState->getRemotePort();
  setContent( j );
}

// Open
Open::Open( const ix::WebSocketOpenInfo& openInfo ) : IXMessage( MessageType::Open )
{
  nlohmann::json j = getContent();
  j["uri"]         = openInfo.uri;
  j["headers"]     = openInfo.headers;
  j["protocol"]    = openInfo.protocol;
  setContent( j );
}

Open::Open( const ix::WebSocketOpenInfo& openInfo, std::shared_ptr<ConnectionState>& connectionState ) : Open( openInfo ) { setConnectionStateInfos( connectionState ); }

std::string Open::getURI() const { return get()["content"]["uri"].get<std::string>(); }

std::map<std::string, std::string> Open::getHeaders() const
{
  std::map<std::string, std::string> ret = get()["content"]["headers"].get<std::map<std::string, std::string>>();
  return ret;
}

std::string Open::getProtocol() const { return get()["content"]["protocol"].get<std::string>(); }

// Close
Close::Close( const ix::WebSocketCloseInfo& closeInfo ) : IXMessage( MessageType::Close )
{
  nlohmann::json j;
  j["code"]   = closeInfo.code;
  j["reason"] = closeInfo.reason;
  j["remote"] = closeInfo.remote;
  setContent( j );
}

Close::Close( const ix::WebSocketCloseInfo& closeInfo, std::shared_ptr<ConnectionState>& connectionState ) : Close( closeInfo ) { setConnectionStateInfos( connectionState ); }

std::uint16_t Close::getCode() const { return get()["content"]["code"].get<std::uint16_t>(); }
std::string   Close::getReason() const { return get()["content"]["reason"].get<std::string>(); }
bool          Close::getRemote() const { return get()["content"]["remote"].get<bool>(); }

// Error
Error::Error( const ix::WebSocketErrorInfo& errorInfo ) : IXMessage( MessageType::Error )
{
  nlohmann::json j;
  j["retries"]             = errorInfo.retries;
  j["wait_time"]           = errorInfo.wait_time;
  j["http_status"]         = errorInfo.http_status;
  j["reason"]              = errorInfo.reason;
  j["decompression_error"] = errorInfo.decompressionError;
  setContent( j );
}

Error::Error( const ix::WebSocketErrorInfo& errorInfo, std::shared_ptr<ConnectionState>& connectionState ) : Error( errorInfo ) { setConnectionStateInfos( connectionState ); }

std::uint16_t Error::getRetries() const { return get()["content"]["retries"].get<std::uint16_t>(); }

double Error::getWaitTime() const { return get()["content"]["wait_time"].get<double>(); }

int Error::getHttpStatus() const { return get()["content"]["http_status"].get<int>(); }

std::string Error::getReason() const { return get()["content"]["reason"].get<std::string>(); }

bool Error::getDecompressionError() const { return get()["content"]["decompression_error"].get<bool>(); }

// Ping
Ping::Ping( const ix::WebSocketMessagePtr& ping ) : IXMessage( MessageType::Ping ) {}

Ping::Ping( const ix::WebSocketMessagePtr& ping, std::shared_ptr<ConnectionState>& connectionState ) : Ping( ping ) { setConnectionStateInfos( connectionState ); }

// Pong
Pong::Pong( const ix::WebSocketMessagePtr& pong ) : IXMessage( MessageType::Pong ) {}

Pong::Pong( const ix::WebSocketMessagePtr& pong, std::shared_ptr<ConnectionState>& connectionState ) : Pong( pong ) { setConnectionStateInfos( connectionState ); }

// Fragment
Fragment::Fragment( const ix::WebSocketMessagePtr& fragment ) : IXMessage( MessageType::Fragment ) {}

Fragment::Fragment( const ix::WebSocketMessagePtr& fragment, std::shared_ptr<ConnectionState>& connectionState ) : Fragment( fragment ) { setConnectionStateInfos( connectionState ); }

}  // namespace yaodaq
