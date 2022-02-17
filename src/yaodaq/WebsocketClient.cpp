/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketClient.hpp"

#include "yaodaq/Exception.hpp"
#include "yaodaq/IXWebsocketMessage.hpp"
#include "yaodaq/Message.hpp"
#include "yaodaq/StatusCode.hpp"

#include <chrono>
#include <ixwebsocket/IXNetSystem.h>
#include <magic_enum.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <thread>

namespace yaodaq
{

bool WebsocketClient::m_ThrowGeneralIfSameName{ true };

void WebsocketClient::throwGeneralIfSameName( const bool& activate ) { m_ThrowGeneralIfSameName = activate; }

WebsocketClient::WebsocketClient( const std::string& name, const std::string& type ) : m_Identifier( type, name )
{
  ix::initNetSystem();

  m_Identifier.generateKey( Domain::Application, Class::Client, Family::WebSocketClient );
  m_Logger.setName( m_Identifier.get() );
  m_Logger.addSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );

  ix::WebSocketHttpHeaders header{ { "id", m_Identifier.get() } };
  setExtraHeaders( header );

  setOnMessageCallback(
    [this]( const ix::WebSocketMessagePtr& msg )
    {
      if( msg->type == ix::WebSocketMessageType::Message )
      {
        IXMessage ixmessage( msg );
        onMessage( ixmessage );
      }
      else if( msg->type == ix::WebSocketMessageType::Open )
      {
        Open open( msg->openInfo );
        onOpen( open );
      }
      else if( msg->type == ix::WebSocketMessageType::Close )
      {
        Close close( msg->closeInfo );
        onClose( close );
      }
      else if( msg->type == ix::WebSocketMessageType::Error )
      {
        Error error( msg->errorInfo );
        onError( error );
      }
      else if( msg->type == ix::WebSocketMessageType::Ping )
      {
        Ping ping( msg );
        onPing( ping );
      }
      else if( msg->type == ix::WebSocketMessageType::Pong )
      {
        Pong pong( msg );
        onPong( pong );
      }
      else if( msg->type == ix::WebSocketMessageType::Fragment )
      {
        Fragment fragment( msg );
        onFragment( fragment );
      }
    } );
}

void WebsocketClient::onMessage( Message& message )
{
  switch( message.getTypeValue() )
  {
    case MessageType::Exception:
      MessageException& message_exception = reinterpret_cast<MessageException&>( message );
      // Special case for connection to the server with the same name as an other client !
      if( static_cast<StatusCode>( message_exception.getCode() ) == StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED )
      {
        disableAutomaticReconnection();
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        close( message_exception.getCode(), message_exception.getDescription() );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        if( m_ThrowGeneralIfSameName ) throw Exception( static_cast<StatusCode>( message_exception.getCode() ), message_exception.getDescription() );
      }
      onException( message_exception );
      break;
  }
}

void WebsocketClient::onOpen( Open& open )
{
  std::string headers;
  for( auto it: open.getHeaders() ) { headers += fmt::format( "\t{}: {}\n", it.first, it.second ); }
  logger()->debug( fmt::format( fg( fmt::color::green ), "Connection opened:\nURI: {}\nProtocol: {}\nHeaders:\n{}", open.getURI(), open.getProtocol(), headers ) );
}

void WebsocketClient::onClose( Close& close ) { logger()->debug( fmt::format( fg( fmt::color::green ), "Connection closed:\nCode: {}\nReason: {}\nRemote: {}", close.getCode(), close.getReason(), close.getRemote() ) ); }

void WebsocketClient::onError( Error& error )
{
  logger()->error( fmt::format( fg( fmt::color::red ), "Error:\nRetries: {}\nWait time: {}\nHTTP status: {}\nReason: {}\nCompression error: {}", error.getRetries(), error.getWaitTime(), error.getHttpStatus(), error.getDecompressionError() ) );
}

void WebsocketClient::onPing( Ping& ping ) { logger()->debug( fmt::format( fg( fmt::color::green ), "Ping:\n{}", ping.getContent().dump( 2 ) ) ); }

void WebsocketClient::onPong( Pong& pong ) { logger()->debug( fmt::format( fg( fmt::color::green ), "Pong:\n{}", pong.getContent().dump( 2 ) ) ); }

void WebsocketClient::onFragment( Fragment& fragment ) {}

void WebsocketClient::onException( MessageException& message )
{
  Exception exception( StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED, message.getDescription() );
  logger()->critical( "{Exception }", exception.what() );
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
