/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/IXWebsocketMessage.hpp"

namespace yaodaq
{

IXMessage::IXMessage( const MessageType& messageType ) : Message( messageType ) {}

IXMessage::IXMessage( const ix::WebSocketMessagePtr& msg ) : Message()
{
  // FIXME
  nlohmann::json json = nlohmann::json::parse( msg->str, nullptr, false );
  if( json.is_discarded() ) { m_JSON["content"] = static_cast<std::string>( msg->str ); }
  else
    m_JSON = json;
}

void IXMessage::setConnectionStateInfos( std::shared_ptr<ConnectionState>& connectionState )
{
  nlohmann::json j = getContent();
  j["id"]          = connectionState->getId();
  j["remote_ip"]   = connectionState->getRemoteIp();
  j["remote_port"] = connectionState->getRemotePort();
  setContent( j );
}

std::string IXMessage::getId() const { return get()["content"]["id"].get<std::string>(); }

std::string IXMessage::getRemoteIp() const { return get()["content"]["remote_ip"].get<std::string>(); }

int IXMessage::getRemotePort() const { return get()["content"]["remote_port"].get<int>(); }

// IXOpen
IXOpen::IXOpen( const ix::WebSocketOpenInfo& openInfo ) : IXMessage( MessageType::Open )
{
  nlohmann::json j = getContent();
  j["uri"]         = openInfo.uri;
  j["headers"]     = openInfo.headers;
  j["protocol"]    = openInfo.protocol;
  setContent( j );
}

IXOpen::IXOpen( const ix::WebSocketOpenInfo& openInfo, std::shared_ptr<ConnectionState>& connectionState ) : IXOpen( openInfo ) { setConnectionStateInfos( connectionState ); }

std::string IXOpen::getURI() const { return get()["content"]["uri"].get<std::string>(); }

std::map<std::string, std::string> IXOpen::getHeaders() const
{
  std::map<std::string, std::string> ret = get()["content"]["headers"].get<std::map<std::string, std::string>>();
  return ret;
}

std::string IXOpen::getProtocol() const { return get()["content"]["protocol"].get<std::string>(); }

// IXClose
IXClose::IXClose( const ix::WebSocketCloseInfo& closeInfo ) : IXMessage( MessageType::Close )
{
  nlohmann::json j;
  j["code"]   = closeInfo.code;
  j["reason"] = closeInfo.reason;
  j["remote"] = closeInfo.remote;
  setContent( j );
}

IXClose::IXClose( const ix::WebSocketCloseInfo& closeInfo, std::shared_ptr<ConnectionState>& connectionState ) : IXClose( closeInfo ) { setConnectionStateInfos( connectionState ); }

std::uint16_t IXClose::getCode() const { return get()["content"]["code"].get<std::uint16_t>(); }
std::string   IXClose::getReason() const { return get()["content"]["reason"].get<std::string>(); }
bool          IXClose::getRemote() const { return get()["content"]["remote"].get<bool>(); }

// IXError
IXError::IXError( const ix::WebSocketErrorInfo& errorInfo ) : IXMessage( MessageType::Error )
{
  nlohmann::json j;
  j["retries"]             = errorInfo.retries;
  j["wait_time"]           = errorInfo.wait_time;
  j["http_status"]         = errorInfo.http_status;
  j["reason"]              = errorInfo.reason;
  j["decompression_error"] = errorInfo.decompressionError;
  setContent( j );
}

IXError::IXError( const ix::WebSocketErrorInfo& errorInfo, std::shared_ptr<ConnectionState>& connectionState ) : IXError( errorInfo ) { setConnectionStateInfos( connectionState ); }

std::uint16_t IXError::getRetries() const { return get()["content"]["retries"].get<std::uint16_t>(); }

double IXError::getWaitTime() const { return get()["content"]["wait_time"].get<double>(); }

int IXError::getHttpStatus() const { return get()["content"]["http_status"].get<int>(); }

std::string IXError::getReason() const { return get()["content"]["reason"].get<std::string>(); }

bool IXError::getDecompressionError() const { return get()["content"]["decompression_error"].get<bool>(); }

// Ping
IXPing::IXPing( const ix::WebSocketMessagePtr& ping ) : IXMessage( MessageType::Ping ) {}

IXPing::IXPing( const ix::WebSocketMessagePtr& ping, std::shared_ptr<ConnectionState>& connectionState ) : IXPing( ping ) { setConnectionStateInfos( connectionState ); }

// Pong
IXPong::IXPong( const ix::WebSocketMessagePtr& pong ) : IXMessage( MessageType::Pong ) {}

IXPong::IXPong( const ix::WebSocketMessagePtr& pong, std::shared_ptr<ConnectionState>& connectionState ) : IXPong( pong ) { setConnectionStateInfos( connectionState ); }

// Fragment
IXFragment::IXFragment( const ix::WebSocketMessagePtr& fragment ) : IXMessage( MessageType::Fragment ) {}

IXFragment::IXFragment( const ix::WebSocketMessagePtr& fragment, std::shared_ptr<ConnectionState>& connectionState ) : IXFragment( fragment ) { setConnectionStateInfos( connectionState ); }

}  // namespace yaodaq
