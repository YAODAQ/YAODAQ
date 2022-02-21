#ifndef YAODAQ_LOGGERHANDLER
#define YAODAQ_LOGGERHANDLER

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/LogLevel.hpp"

#include <fmt/format.h>
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

class Log;

class LoggerHandler
{
public:
  LoggerHandler();
  ~LoggerHandler();
  void                                   setVerbosity( const LogLevel& verbosity );
  void                                   setName( const std::string& );
  void                                   addSink( const spdlog::sink_ptr& );
  void                                   clearSinks();
  // Logs
  template<typename... Args> inline void log( const LogLevel& level, fmt::string_view str, const Args&... args ) { printLog( level, fmt::format( str, args... ) ); }
  template<typename... Args> inline void log( fmt::string_view str, const Args&... args ) { printLog( LogLevel::Info, fmt::format( str, args... ) ); }
  template<typename... Args> inline void trace( fmt::string_view fmt, const Args&... args ) { log( LogLevel::Trace, fmt, args... ); }
  template<typename... Args> inline void debug( fmt::string_view fmt, const Args&... args ) { log( LogLevel::Debug, fmt, args... ); }
  template<typename... Args> inline void info( fmt::string_view fmt, const Args&... args ) { log( LogLevel::Info, fmt, args... ); }
  template<typename... Args> inline void warn( fmt::string_view fmt, const Args&... args ) { log( LogLevel::Warn, fmt, args... ); }
  template<typename... Args> inline void error( fmt::string_view fmt, const Args&... args ) { log( LogLevel::Error, fmt, args... ); }
  template<typename... Args> inline void critical( fmt::string_view fmt, const Args&... args ) { log( LogLevel::Critical, fmt, args... ); }

protected:
  std::shared_ptr<spdlog::logger> logger();

private:
  std::shared_ptr<spdlog::logger> m_Logger{ nullptr };
  std::vector<spdlog::sink_ptr>   m_Sinks;
  std::string                     m_Name{ "Unknown" };
  LogLevel                        m_Verbosity{ LogLevel::Trace };
  void                            init();
  void                            printLog( const LogLevel&, const std::string& );
};

}  // namespace yaodaq

#endif
