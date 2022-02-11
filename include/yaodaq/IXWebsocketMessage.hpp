#ifndef YAODAQ_IXWEBSOCKETMESSAGE
#define YAODAQ_IXWEBSOCKETMESSAGE

/**
\copyright Copyright 2022 flagarde
*/

#include <string>
#include <map>
#include <memory>

#include "yaodaq/Message.hpp"
#include "yaodaq/ConnectionState.hpp"

#include <ixwebsocket/IXWebSocketOpenInfo.h>

namespace yaodaq
{

class Open : public Message
{
public:
  Open(const ix::WebSocketOpenInfo& openInfo);
  Open(const ix::WebSocketOpenInfo& openInfo,std::shared_ptr<ConnectionState>& connectionState);
  std::string getURI() const;
  std::map<std::string,std::string> getHeaders() const;
  std::string getProtocol() const;
  /*
  std::string getKeyStr() const;
  std::string getIdStr() const;*/
};

}
#endif
