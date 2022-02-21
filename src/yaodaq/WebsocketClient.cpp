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

std::function<bool( int current, int total )> WebsocketClient::m_ProgressCallBack = []( int current, int total ) -> bool
{
  std::cout << "\r"
            << "Downloaded " << current << " bytes out of " << total;
  return true;
};

void WebsocketClient::throwGeneralIfSameName( const bool& activate ) { m_ThrowGeneralIfSameName = activate; }

WebsocketClient::WebsocketClient( const std::string& name, const std::string& type ) : m_Identifier( type, name )
{
  ix::initNetSystem();
  enablePerMessageDeflate();
  enablePong();
  setPingInterval( 10 );
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
        IXOpen open( msg->openInfo );
        onIXOpen( open );
      }
      else if( msg->type == ix::WebSocketMessageType::Close )
      {
        IXClose close( msg->closeInfo );
        onIXClose( close );
      }
      else if( msg->type == ix::WebSocketMessageType::Error )
      {
        IXError error( msg->errorInfo );
        onIXError( error );
      }
      else if( msg->type == ix::WebSocketMessageType::Ping )
      {
        IXPing ping( msg );
        onIXPing( ping );
      }
      else if( msg->type == ix::WebSocketMessageType::Pong )
      {
        IXPong pong( msg );
        onIXPong( pong );
      }
      else if( msg->type == ix::WebSocketMessageType::Fragment )
      {
        IXFragment fragment( msg );
        onIXFragment( fragment );
      }
    } );
}

void WebsocketClient::onMessage( Message& message )
{
  switch( message.getTypeValue() )
  {
    case MessageType::Exception:
    {
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
    case MessageType::Log:
    {
      Log& log = reinterpret_cast<Log&>( message );
      onLog( log );
      break;
    }
    case MessageType::UserType:
    {
      UserType& user_type = reinterpret_cast<UserType&>( message );
      onUserType( user_type );
      break;
    }
    case MessageType::Open:
    {
      IXOpen& open = reinterpret_cast<IXOpen&>( message );
      onOpen( open );
      break;
    }
    case MessageType::Close:
    {
      IXClose& close = reinterpret_cast<IXClose&>( message );
      onClose( close );
      break;
    }
    case MessageType::Error:
    {
      IXError& error = reinterpret_cast<IXError&>( message );
      onError( error );
      break;
    }
    case MessageType::Ping:
    {
      IXPing& ping = reinterpret_cast<IXPing&>( message );
      onPing( ping );
      break;
    }
    case MessageType::Pong:
    {
      IXPong& pong = reinterpret_cast<IXPong&>( message );
      onPong( pong );
      break;
    }
    case MessageType::Fragment:
    {
      IXFragment& fragment = reinterpret_cast<IXFragment&>( message );
      onFragment( fragment );
      break;
    }
    case MessageType::Unknown:
    {
      Exception exception( StatusCode::MESSAGE_TYPE_UNKNOWN, message.dump() );
      // On logger not throw because it't just some info. On other it should never happen so throw if it happens for some strange reasons (BUG).
      if( m_Identifier.getFamily() == magic_enum::enum_name( Family::Logger ) )
      {
        MessageException message_exception( exception );
        onException( message_exception );
      }
      else
        throw exception;
    }
  }
}

void WebsocketClient::onIXOpen( IXOpen& open )
{
  std::string headers;
  for( auto it: open.getHeaders() ) { headers += fmt::format( "\t{}: {}\n", it.first, it.second ); }
  debug( fmt::format( fg( fmt::color::green ), "Connection opened:\nURI: {}\nProtocol: {}\nHeaders:\n{}", open.getURI(), open.getProtocol(), headers ) );
}

void WebsocketClient::onIXClose( IXClose& close ) { debug( fmt::format( fg( fmt::color::green ), "Connection closed:\nCode: {}\nReason: {}\nRemote: {}", close.getCode(), close.getReason(), close.getRemote() ) ); }

void WebsocketClient::onIXError( IXError& err )
{
  error( fmt::format( fg( fmt::color::red ), "Error:\nRetries: {}\nWait time: {}\nHTTP status: {}\nReason: {}\nCompression error: {}", err.getRetries(), err.getWaitTime(), err.getHttpStatus(), err.getReason(), err.getDecompressionError() ) );
}

void WebsocketClient::onIXPing( IXPing& ping ) { trace( fmt::format( fg( fmt::color::green ), "Ping:\n{}", to_fmt( ping.getContent().dump( 2 ) ) ) ); }

void WebsocketClient::onIXPong( IXPong& pong ) { trace( fmt::format( fg( fmt::color::green ), "Pong:\n{}", to_fmt( pong.getContent().dump( 2 ) ) ) ); }

void WebsocketClient::onIXFragment( IXFragment& fragment ) { debug( fmt::format( fg( fmt::color::green ), "Fragment:\n{}", to_fmt( fragment.getContent().dump( 2 ) ) ) ); }

void WebsocketClient::onOpen( IXOpen& open )
{
  // By default use the same has IX ones
  onIXOpen( open );
}

void WebsocketClient::onClose( IXClose& close )
{
  // By default use the same has IX ones
  onIXClose( close );
}

void WebsocketClient::onError( IXError& error )
{
  // By default use the same has IX ones
  onIXError( error );
}

void WebsocketClient::onPing( IXPing& ping )
{
  // By default use the same has IX ones
  onIXPing( ping );
}

void WebsocketClient::onPong( IXPong& pong )
{
  // By default use the same has IX ones
  onIXPong( pong );
}

void WebsocketClient::onFragment( IXFragment& fragment )
{
  // By default use the same has IX ones
  onIXFragment( fragment );
}

void WebsocketClient::onException( MessageException& message )
{
  Exception exception( static_cast<StatusCode>( message.getCode() ), message.getDescription() );
  critical( "Exception:\n{}", exception.what() );
}

void WebsocketClient::onLog( Log& log ) { m_Logger.log( log.getLevel(), fmt::format( "{} (from {})", log.getLog(), log.getIdentifier().getName() ) ); }

void WebsocketClient::onUserType( UserType& user_type ) { debug( fmt::format( fg( fmt::color::green ), "Fragment:\n{}", to_fmt( user_type.dump( 2 ) ) ) ); }

WebsocketClient::~WebsocketClient()
{
  stop();
  ix::uninitNetSystem();
}

void WebsocketClient::start()
{
  if( getReadyState() == ix::ReadyState::Closed || getReadyState() == ix::ReadyState::Closing )
  {
    trace( "Client started. Connected to {}", getUrl() );
    ix::WebSocket::start();
  }
}

void WebsocketClient::stop()
{
  if( getReadyState() == ix::ReadyState::Open || getReadyState() == ix::ReadyState::Connecting )
  {
    trace( "Client stopped" );
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
    if( value >= magic_enum::enum_integer( yaodaq::Severity::Critical ) ) { critical( "Signal SIG{} raised !", magic_enum::enum_name( signal ) ); }
    else if( value >= magic_enum::enum_integer( yaodaq::Severity::Error ) )
    {
      error( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    else if( value >= magic_enum::enum_integer( yaodaq::Severity::Warning ) )
    {
      fmt::print( "\n" );
      warn( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    else if( value >= magic_enum::enum_integer( yaodaq::Severity::Info ) )
    {
      fmt::print( "\n" );
      info( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    else
    {
      fmt::print( "\n" );
      trace( "Signal SIG{} raised !", magic_enum::enum_name( signal ) );
    }
    if( magic_enum::enum_integer( signal ) >= magic_enum::enum_integer( Severity::Critical ) ) std::exit( magic_enum::enum_integer( signal ) );
  }
}

ix::WebSocketSendInfo WebsocketClient::send( const std::string& data, bool binary, const ix::OnProgressCallback& onProgressCallback ) { return ix::WebSocket::send( data, binary, onProgressCallback ); }

ix::WebSocketSendInfo WebsocketClient::sendBinary( const std::string& data, const ix::OnProgressCallback& onProgressCallback ) { return ix::WebSocket::sendBinary( data, onProgressCallback ); }

ix::WebSocketSendInfo WebsocketClient::sendBinary( const ix::IXWebSocketSendData& data, const ix::OnProgressCallback& onProgressCallback ) { return ix::WebSocket::sendBinary( data, onProgressCallback ); }

// does not check for valid UTF-8 characters. Caller must check that.
ix::WebSocketSendInfo WebsocketClient::sendUtf8Text( const std::string& text, const ix::OnProgressCallback& onProgressCallback ) { return ix::WebSocket::sendUtf8Text( text, onProgressCallback ); }

// does not check for valid UTF-8 characters. Caller must check that.
ix::WebSocketSendInfo WebsocketClient::sendUtf8Text( const ix::IXWebSocketSendData& text, const ix::OnProgressCallback& onProgressCallback ) { return ix::WebSocket::sendUtf8Text( text, onProgressCallback ); }

ix::WebSocketSendInfo WebsocketClient::sendText( const std::string& text, const ix::OnProgressCallback& onProgressCallback ) { return ix::WebSocket::sendText( text, onProgressCallback ); }

}  // namespace yaodaq
