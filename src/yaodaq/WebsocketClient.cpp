/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketClient.hpp"

#include <chrono>
#include <ixwebsocket/IXNetSystem.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <thread>

namespace yaodaq
{

WebsocketClient::WebsocketClient( const std::string& name, const std::string& type ) : m_Identifier( Class::WebsocketClient, type, name ), m_Logger( m_Identifier.get() )
{
  ix::initNetSystem();
  m_Logger.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
  setOnMessageCallback(
    [this]( const ix::WebSocketMessagePtr& msg )
    {
      if( msg->type == ix::WebSocketMessageType::Message ) { logger()->error( "{}", msg->str ); }
    } );
}

WebsocketClient::~WebsocketClient()
{
  stop();
  ix::uninitNetSystem();
}

void WebsocketClient::start()
{
  if( getReadyState() != ix::ReadyState::Open )
  {
    logger()->trace( "Client started" );
    ix::WebSocket::start();
    while( getReadyState() != ix::ReadyState::Open && m_Looper.getSignal() == Signal::NO ) { std::this_thread::sleep_for( std::chrono::microseconds( 1 ) ); }
  }
}

void WebsocketClient::stop()
{
  if( getReadyState() != ix::ReadyState::Closed )
  {
    logger()->trace( "Client stopped" );
    ix::WebSocket::stop();
    while( getReadyState() != ix::ReadyState::Closed ) { std::this_thread::sleep_for( std::chrono::microseconds( 1 ) ); }
  }
}

void WebsocketClient::loop()
{
  start();
  m_Looper.supressInstance();
  onRaisingSignal();
}

void WebsocketClient::onRaisingSignal()
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
