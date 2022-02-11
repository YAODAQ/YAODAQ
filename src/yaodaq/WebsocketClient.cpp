/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketClient.hpp"
#include "yaodaq/IXWebsocketMessage.hpp"
#include "yaodaq/StatusCode.hpp"
#include "yaodaq/Exception.hpp"

#include <chrono>
#include <ixwebsocket/IXNetSystem.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <thread>

namespace yaodaq
{

WebsocketClient::WebsocketClient( const std::string& name, const std::string& type ) : m_Identifier(type, name )
{
  ix::initNetSystem();

  m_Identifier.generateKey(Domain::Application,Class::Client,Family::WebSocketClient);
  m_Logger.setName( m_Identifier.get() );
  m_Logger.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );

  ix::WebSocketHttpHeaders header{ { "id", m_Identifier.get() } };
  setExtraHeaders( header );

  setOnMessageCallback(
    [this]( const ix::WebSocketMessagePtr& msg )
    {
      if( msg->type == ix::WebSocketMessageType::Message ) { logger()->error( "{}", msg->str ); }
      else if (msg->type == ix::WebSocketMessageType::Error)
      {
        std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
      }
      else if (msg->type == ix::WebSocketMessageType::Close)
      {
        disableAutomaticReconnection();
        if(msg->closeInfo.code== magic_enum::enum_integer(StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED))
        {
          logger()->critical(fmt::format(fg(fmt::color::red) | fmt::emphasis::bold,msg->closeInfo.reason));
          throw Exception(StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED,msg->closeInfo.reason);
        }
      }
    }


    );
}

WebsocketClient::~WebsocketClient()
{
  stop();
  ix::uninitNetSystem();
}

void WebsocketClient::start()
{
  if( getReadyState() == ix::ReadyState::Closed || getReadyState() == ix::ReadyState::Closing )
  {
    logger()->trace( "Client started. Connected to {}", getUrl() );
    ix::WebSocket::start();
  }
}

void WebsocketClient::stop()
{
  if( getReadyState() == ix::ReadyState::Open || getReadyState() == ix::ReadyState::Connecting )
  {
    logger()->trace( "Client stopped" );
    ix::WebSocket::stop();
    while( getReadyState() != ix::ReadyState::Closed ) { std::this_thread::sleep_for( std::chrono::microseconds( 1 ) ); }
  }
}

void WebsocketClient::loop()
{
  WebsocketClient::start();
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
