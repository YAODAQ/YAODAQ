/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketServer.hpp"

#include "yaodaq/Exception.hpp"
#include "yaodaq/StatusCode.hpp"

#include <iostream>
#include <ixwebsocket/IXNetSystem.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <utility>

namespace yaodaq
{

WebsocketServer::WebsocketServer( const std::string& name, const int& port, const std::string& host, const int& backlog, const std::size_t& maxConnections, const int& handshakeTimeoutSecs, const int& addressFamily, const std::string& type ) :
  ix::WebSocketServer( port, host, backlog, maxConnections, handshakeTimeoutSecs, addressFamily ), m_Identifier( Class::WebsocketServer, type, name ), m_Logger( m_Identifier.get() )
{
  ix::initNetSystem();
  m_Logger.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
  setOnClientMessageCallback(
    []( std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg )
    {
      // The ConnectionState object contains information about the connection,
      // at this point only the client ip address and the port.
      std::cout << "Remote ip: " << connectionState->getRemoteIp() << std::endl;

      if( msg->type == ix::WebSocketMessageType::Open )
      {
        std::cout << "New connection" << std::endl;

        // A connection state object is available, and has a default id
        // You can subclass ConnectionState and pass an alternate factory
        // to override it. It is useful if you want to store custom
        // attributes per connection (authenticated bool flag, attributes, etc...)
        std::cout << "id: " << connectionState->getId() << std::endl;

        // The uri the client did connect to.
        std::cout << "Uri: " << msg->openInfo.uri << std::endl;

        std::cout << "Headers:" << std::endl;
        for( auto it: msg->openInfo.headers ) { std::cout << "\t" << it.first << ": " << it.second << std::endl; }
      }
      else if( msg->type == ix::WebSocketMessageType::Message )
      {
        // For an echo server, we just send back to the client whatever was received by the server
        // All connected clients are available in an std::set. See the broadcast cpp example.
        // Second parameter tells whether we are sending the message in binary or text mode.
        // Here we send it in the same mode as it was received.
        std::cout << "Received: " << msg->str << std::endl;

        webSocket.send( msg->str, msg->binary );
      }
    } );
}

void WebsocketServer::listen()
{
  if( !m_isListening )
  {
    std::pair<bool, std::string> ret = ix::WebSocketServer::listen();
    if( ret.first )
    {
      m_isListening = ret.first;
      logger()->info( "Server listening on host {0} port {1}", getHost(), getPort() );
    }
    else
      throw Exception( StatusCode::LISTEN_ERROR, ret.second );
  }
}

void WebsocketServer::start()
{
  if( !m_isStarted )
  {
    m_isStarted = true;
    logger()->trace( "Server started" );
    ix::WebSocketServer::start();
  }
}

void WebsocketServer::stop( bool useless )
{
  if( !m_isStopped )
  {
    m_isStopped = true;
    useless     = !useless;
    logger()->trace( "Server stopped" );
    ix::WebSocketServer::stop();
  }
}

void WebsocketServer::setVerbosity( const yaodaq::LoggerHandler::Verbosity& verbosity ) { m_Logger.setVerbosity( verbosity ); }

WebsocketServer::~WebsocketServer()
{
  stop();
  ix::uninitNetSystem();
}

void WebsocketServer::loop()
{
  listen();
  start();
  m_Looper.supressInstance();
  onRaisingSignal();
}

void WebsocketServer::onRaisingSignal()
{
  Signal signal = m_Looper.loop();
  if( m_Looper.getInstance() == 0 )
  {
    int value = magic_enum::enum_integer( signal );
    if( value >= magic_enum::enum_integer( yaodaq::Severity::Critical ) ) { logger()->critical( "Signal SIG{} raised !", magic_enum::enum_name( signal ) ); }
    else if( value >= magic_enum::enum_integer( yaodaq::Severity::Error ) )
    {
      logger()->error( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    else if( value >= magic_enum::enum_integer( yaodaq::Severity::Warning ) )
    {
      fmt::print( "\n" );
      logger()->warn( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    else if( value >= magic_enum::enum_integer( yaodaq::Severity::Info ) )
    {
      fmt::print( "\n" );
      logger()->info( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    else
    {
      fmt::print( "\n" );
      logger()->trace( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    if( magic_enum::enum_integer( signal ) >= magic_enum::enum_integer( Severity::Critical ) ) std::exit( magic_enum::enum_integer( signal ) );
  }
}

}  // namespace yaodaq
