#ifndef YAODAQ_CONNECTIONSTATE
#define YAODAQ_CONNECTIONSTATE

#include <ixwebsocket/IXConnectionState.h>

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <utility>
#include <mutex>

namespace yaodaq
{

class Identifier;

class ConnectionState : public ix::ConnectionState
{
public:
  virtual void computeId(const std::string& host, const Identifier& id) final;
  ConnectionState();
  virtual ~ConnectionState();
private:
  static std::list<std::pair<std::string,std::string>> m_Ids;
  std::pair<std::string,std::string> m_Pair;
  std::mutex m_Mutex;
};

}

#endif
