/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketServer.hpp"

#include "yaodaq/ConnectionState.hpp"
#include "yaodaq/Exception.hpp"
#include "yaodaq/IXWebsocketMessage.hpp"
#include "yaodaq/Identifier.hpp"
#include "yaodaq/StatusCode.hpp"

#include <chrono>
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
  ix::WebSocketServer( port, host, backlog, maxConnections, handshakeTimeoutSecs, addressFamily ), m_Identifier( type, name )
{
  ix::initNetSystem();

  m_Identifier.generateKey( Domain::Application, Class::Server, Family::WebSocketServer );
  m_Logger.setName( m_Identifier.get() );
  m_Logger.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );

  setConnectionStateFactory( []() { return std::make_shared<ConnectionState>(); } );

  setOnClientMessageCallback(
    [this]( std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg )
    {
      // The ConnectionState object contains information about the connection
      std::shared_ptr<ConnectionState> connection = std::static_pointer_cast<ConnectionState>( connectionState );

      if( msg->type == ix::WebSocketMessageType::Open )
      {
        // Check if a client with the same name is already connected;
        logger()->critical( fmt::format( fg( fmt::color::red ) | fmt::emphasis::bold, getHost() + ":" + std::to_string( getPort() ) ) );
        connection->computeId( getHost() + ":" + std::to_string( getPort() ), Identifier::parse( msg->openInfo.headers["id"] ) );
        if( connection->isTerminated() )
        {
          logger()->error( fmt::format( fg( fmt::color::red ) | fmt::emphasis::bold, "One client with the name \"{}\" is already connected !", Identifier::parse( msg->openInfo.headers["id"] ).getName() ) );
          webSocket.stop( magic_enum::enum_integer( StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED ),
                          fmt::format( "One client with the name \"{}\" is already connected to ws{}://{}:{} !", Identifier::parse( msg->openInfo.headers["id"] ).getName(), "", getHost(), getPort() ) );
          std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
          return;
        }
      }
      else if( msg->type == ix::WebSocketMessageType::Message )
      {
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
      logger()->info( "Server listening on {0}:{1}", getHost(), getPort() );
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
