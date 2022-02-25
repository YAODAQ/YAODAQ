/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/IXWebsocketMessage.hpp"

namespace yaodaq
{

Open::Open( const ix::WebSocketOpenInfo& openInfo ) : Message( MessageType::Open )
{
  nlohmann::json j;
  j["uri"]      = openInfo.uri;
  j["headers"]  = openInfo.headers;
  j["protocol"] = openInfo.protocol;
  setContent( j );
}

Open::Open( const ix::WebSocketOpenInfo& openInfo, std::shared_ptr<ConnectionState>& connectionState ) : Open( openInfo )
{
  nlohmann::json j = get();
  j["id"]          = connectionState->getId();
  j["remote_ip"]   = connectionState->getRemoteIp();
  j["remote_port"] = connectionState->getRemotePort();
  setContent( j );
}

std::string Open::getURI() const { return get()["content"]["uri"].get<std::string>(); }

std::map<std::string, std::string> Open::getHeaders() const
{
  std::map<std::string, std::string> ret = get()["content"]["headers"].get<std::map<std::string, std::string>>();
  return ret;
}

std::string Open::getProtocol() const { return get()["content"]["protocol"].get<std::string>(); }

}  // namespace yaodaq
