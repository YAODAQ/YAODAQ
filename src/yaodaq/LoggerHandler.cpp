/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/LoggerHandler.hpp"

#include "spdlog/spdlog.h"

namespace yaodaq
{

LoggerHandler::LoggerHandler() { init(); }

void LoggerHandler::setName( const std::string& name )
{
  m_Name = name;
  init();
}

LoggerHandler::~LoggerHandler() {}

void LoggerHandler::setVerbosity( const LogLevel& verbosity )
{
  m_Verbosity = verbosity;
  init();
}

void LoggerHandler::init()
{
  m_Logger = std::make_shared<spdlog::logger>( m_Name, std::begin( m_Sinks ), std::end( m_Sinks ) );
  switch( m_Verbosity )
  {
    case LogLevel::Off: m_Logger->set_level( spdlog::level::off ); break;
    case LogLevel::Trace: m_Logger->set_level( spdlog::level::trace ); break;
    case LogLevel::Debug: m_Logger->set_level( spdlog::level::debug ); break;
    case LogLevel::Info: m_Logger->set_level( spdlog::level::info ); break;
    case LogLevel::Warn: m_Logger->set_level( spdlog::level::warn ); break;
    case LogLevel::Error: m_Logger->set_level( spdlog::level::err ); break;
    case LogLevel::Critical: m_Logger->set_level( spdlog::level::critical ); break;
  }
}

void LoggerHandler::printLog(const LogLevel& level,const std::string& str)
{
  switch( level )
  {
    case LogLevel::Off :
    {
      break;
    }
    case LogLevel::Trace:
    {
      logger()->trace(str);
      break;
    }
    case LogLevel::Debug:
    {
      logger()->debug(str);
      break;
    }
    case LogLevel::Info:
    {
      logger()->info(str);
      break;
    }
    case LogLevel::Warn:
    {
      logger()->warn(str);
      break;
    }
    case LogLevel::Error:
    {
      logger()->error(str);
      break;
    }
    case LogLevel::Critical:
    {
      logger()->critical(str);
      break;
    }
  }
}

std::shared_ptr<spdlog::logger> LoggerHandler::logger() { return std::shared_ptr<spdlog::logger>( m_Logger ); }

void LoggerHandler::addSink( const spdlog::sink_ptr& sink )
{
  m_Sinks.push_back( sink );
  init();
}

void LoggerHandler::clearSinks()
{
  m_Sinks.clear();
  init();
}

}  // namespace yaodaq
