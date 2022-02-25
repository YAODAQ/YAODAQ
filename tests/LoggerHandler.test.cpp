/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/LoggerHandler.hpp"

#include "doctest/doctest.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

TEST_CASE( "LoggerHandler" )
{
  yaodaq::LoggerHandler logger1;
  logger1.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
  yaodaq::LoggerHandler logger2;
  logger2.setName( "Logger 2" );
  logger2.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );

  logger1.logger()->trace( "Trace" );
  logger1.logger()->debug( "Debug" );
  logger1.logger()->info( "Info" );
  logger1.logger()->warn( "Warn" );
  logger1.logger()->error( "Error" );
  logger1.logger()->critical( "Critical" );

  logger1.setVerbosity( yaodaq::LoggerHandler::Verbosity::Warn );
  logger1.logger()->trace( "Trace (verbosity Warn)" );
  logger1.logger()->debug( "Debug (verbosity Warn)" );
  logger1.logger()->info( "Info (verbosity Warn)" );
  logger1.logger()->warn( "Warn (verbosity Warn)" );
  logger1.logger()->error( "Error (verbosity Warn)" );
  logger1.logger()->critical( "Critical (verbosity Warn)" );

  logger2.logger()->trace( "Trace" );
  logger2.logger()->debug( "Debug" );
  logger2.logger()->info( "Info" );
  logger2.logger()->warn( "Warn" );
  logger2.logger()->error( "Error" );
  logger2.logger()->critical( "Critical" );
}
