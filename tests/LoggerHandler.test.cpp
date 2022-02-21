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

  logger1.trace( "Trace" );
  logger1.debug( "Debug" );
  logger1.info( "Info" );
  logger1.warn( "Warn" );
  logger1.error( "Error" );
  logger1.critical( "Critical" );

  logger1.setVerbosity( yaodaq::LogLevel::Warn );
  logger1.trace( "Trace (verbosity Warn)" );
  logger1.debug( "Debug (verbosity Warn)" );
  logger1.info( "Info (verbosity Warn)" );
  logger1.warn( "Warn (verbosity Warn)" );
  logger1.error( "Error (verbosity Warn)" );
  logger1.critical( "Critical (verbosity Warn)" );

  logger2.trace( "Trace" );
  logger2.debug( "Debug" );
  logger2.info( "Info" );
  logger2.warn( "Warn" );
  logger2.error( "Error" );
  logger2.critical( "Critical" );
}
