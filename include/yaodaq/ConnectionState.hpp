#ifndef YAODAQ_CONNECTIONSTATE
#define YAODAQ_CONNECTIONSTATE

/**
\copyright Copyright 2022 flagarde
*/

#include <algorithm>
#include <iostream>
#include <ixwebsocket/IXConnectionState.h>
#include <list>
#include <mutex>
#include <string>
#include <utility>

namespace yaodaq
{

class Identifier;

class ConnectionState : public ix::ConnectionState
{
public:
  virtual void computeId( const std::string& host, const Identifier& id ) final;
  ConnectionState();
  virtual ~ConnectionState();

private:
  static std::list<std::pair<std::string, std::string>> m_Ids;
  std::pair<std::string, std::string>                   m_Pair;
  std::mutex                                            m_Mutex;
};

}  // namespace yaodaq

#endif
