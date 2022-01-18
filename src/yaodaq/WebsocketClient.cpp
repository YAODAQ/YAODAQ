/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketClient.hpp"

#include "ixwebsocket/IXNetSystem.h"

namespace yaodaq
{

WebsocketClient::WebsocketClient( const std::string& name, const std::string& type ) : m_Identifier( Class::WebsocketClient, type, name ) { ix::initNetSystem(); }

WebsocketClient::~WebsocketClient() { ix::uninitNetSystem(); }

}  // namespace yaodaq
