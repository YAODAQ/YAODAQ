#ifndef YAODAQ_LOGGERHANDLER
#define YAODAQ_LOGGERHANDLER

/**
\copyright Copyright 2022 flagarde
*/

#include <memory>
#include <spdlog/fwd.h>
#include <string>
#include <vector>

namespace spdlog
{
using sink_ptr = std::shared_ptr<spdlog::sinks::sink>;
}

namespace yaodaq
{

class LoggerHandler
{
public:
  enum class Verbosity
  {
    Off,
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
  };
  LoggerHandler();
  ~LoggerHandler();
  void                            setVerbosity( const Verbosity& verbosity );
  void                            setName( const std::string& );
  std::shared_ptr<spdlog::logger> logger();
  void                            addSink( const spdlog::sink_ptr& );
  void                            clearSinks();

private:
  std::shared_ptr<spdlog::logger> m_Logger{ nullptr };
  std::vector<spdlog::sink_ptr>   m_Sinks;
  std::string                     m_Name{ "Unknown" };
  Verbosity                       m_Verbosity{ Verbosity::Trace };
  void                            init();
};

}  // namespace yaodaq

#endif
