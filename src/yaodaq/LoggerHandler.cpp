/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/LoggerHandler.hpp"

#include "spdlog/spdlog.h"

namespace yaodaq
{

LoggerHandler::LoggerHandler() { init(); }

LoggerHandler::LoggerHandler( const std::string& name ) : m_Name( name ) { init(); }

LoggerHandler::~LoggerHandler() {}

void LoggerHandler::setVerbosity( const Verbosity& verbosity )
{
  m_Verbosity = verbosity;
  init();
}

void LoggerHandler::init()
{
  m_Logger = std::make_shared<spdlog::logger>( m_Name, std::begin( m_Sinks ), std::end( m_Sinks ) );
  switch( m_Verbosity )
  {
    case Verbosity::Off: m_Logger->set_level( spdlog::level::off ); break;
    case Verbosity::Trace: m_Logger->set_level( spdlog::level::trace ); break;
    case Verbosity::Debug: m_Logger->set_level( spdlog::level::debug ); break;
    case Verbosity::Info: m_Logger->set_level( spdlog::level::info ); break;
    case Verbosity::Warn: m_Logger->set_level( spdlog::level::warn ); break;
    case Verbosity::Error: m_Logger->set_level( spdlog::level::err ); break;
    case Verbosity::Critical: m_Logger->set_level( spdlog::level::critical ); break;
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
