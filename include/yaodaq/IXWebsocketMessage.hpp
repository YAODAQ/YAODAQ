#ifndef YAODAQ_IXWEBSOCKETMESSAGE
#define YAODAQ_IXWEBSOCKETMESSAGE

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/ConnectionState.hpp"
#include "yaodaq/Message.hpp"

#include <ixwebsocket/IXWebSocketOpenInfo.h>
#include <map>
#include <memory>
#include <string>

namespace yaodaq
{

class Open : public Message
{
public:
  explicit Open( const ix::WebSocketOpenInfo& openInfo );
  Open( const ix::WebSocketOpenInfo& openInfo, std::shared_ptr<ConnectionState>& connectionState );
  std::string                        getURI() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string                        getProtocol() const;
};

}  // namespace yaodaq
#endif
