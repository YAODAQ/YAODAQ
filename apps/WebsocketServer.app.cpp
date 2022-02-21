/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketServer.hpp"

#include "yaodaq/Exception.hpp"
#include "yaodaq/StatusCode.hpp"

#include <CLI/CLI.hpp>
#include <cstddef>
#include <map>
#include <string>

int main( int argc, char** argv )
{
  CLI::App app{ "Websocket Server" };
  try
  {
    int port{ 8080 };
    app.add_option( "-p,--port", port, "Port to listen" )->check( CLI::Range( 0, 65535 ) );
    std::string host{ "127.0.0.1" };
    app.add_option( "-i,--ip", host, "IP of the server" )->check( CLI::ValidIPV4 );
    int backlog{ 5 };
    app.add_option( "-b,--backlog", backlog, "Backlog" )->check( CLI::Range( 0, 5 ) );
    std::size_t maxConnections{ 256 };
    app.add_option( "-m,--max", maxConnections, "Maximum connections" )->check( CLI::PositiveNumber );
    int handshakeTimeoutSecs{ 3 };
    app.add_option( "-t,--timeout", handshakeTimeoutSecs, "Timeout in seconds" )->check( CLI::PositiveNumber );
    yaodaq::LogLevel verbosity{ yaodaq::LogLevel::Trace };
    app.add_option( "-v,--verbosity", verbosity, "Verbosity" )
      ->ignore_case()
      ->transform( CLI::Transformer( std::map<std::string, yaodaq::LogLevel>( {
                     { "off", yaodaq::LogLevel::Off },
                     { "trace", yaodaq::LogLevel::Trace },
                     { "debug", yaodaq::LogLevel::Debug },
                     { "info", yaodaq::LogLevel::Info },
                     { "warn", yaodaq::LogLevel::Warn },
                     { "error", yaodaq::LogLevel::Error },
                     { "critical", yaodaq::LogLevel::Critical },
                   } ) ),
                   "Log level", "Select the log level (off, trace, debug, info, warn, error, critical)" );
    std::string name{ "WebsocketServer" };
    app.add_option( "-n,--name", name, "Name of the WebSocketServer." );

    app.parse( argc, argv );
    yaodaq::WebsocketServer server( "1", port, host, backlog, maxConnections, handshakeTimeoutSecs );
    yaodaq::WebsocketServer server2( "2", 8081, host, backlog, maxConnections, handshakeTimeoutSecs );
    server.setVerbosity( verbosity );
    server2.setVerbosity( verbosity );
    server.loop();
    server2.loop();
  }
  catch( const CLI::ParseError& e )
  {
    if( app.exit( e ) == 0 ) spdlog::info( "{}", e.what() );
    else
      spdlog::error( "{}", e.what() );
    return app.exit( e );
  }
  catch( const yaodaq::Exception& e )
  {
    spdlog::error( "{}", e.what() );
    return e.code();
  }
}
