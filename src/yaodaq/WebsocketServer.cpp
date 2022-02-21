/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketServer.hpp"

#include "yaodaq/Classification.hpp"
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
      if( msg->type == ix::WebSocketMessageType::Message )
      {
        IXMessage ixmessage( msg );
        onMessage( ixmessage, webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Open )
      {
        // Check if a client with the same name is already connected;
        connection->computeId( getHost() + ":" + std::to_string( getPort() ), Identifier::parse( msg->openInfo.headers["id"] ) );
        if( connection->isTerminated() )
        {
          error( fmt::format( fg( fmt::color::red ) | fmt::emphasis::bold, "One client with the name \"{}\" is already connected !", Identifier::parse( msg->openInfo.headers["id"] ).getName() ) );
          MessageException message_exception(
            Exception( StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED, fmt::format( "One client with the name \"{}\" is already connected to ws{}://{}:{} !", Identifier::parse( msg->openInfo.headers["id"] ).getName(), "", getHost(), getPort() ) ) );

          // Send to the client on exception;
          webSocket.send( message_exception.dump() );
          // Send to loggers except the client as it has been send before;
          sendToLoggers( message_exception, webSocket );

          webSocket.stop( magic_enum::enum_integer( StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED ),
                          fmt::format( "One client with the name \"{}\" is already connected to ws{}://{}:{} !", Identifier::parse( msg->openInfo.headers["id"] ).getName(), "", getHost(), getPort() ) );
          return;
        }
        addClient( Identifier::parse( msg->openInfo.headers["id"] ), webSocket );
        IXOpen open( msg->openInfo, connection );
        onIXOpen( open );
        sendToLoggers( open, webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Close )
      {
        IXClose close( msg->closeInfo, connection );
        onIXClose( close );
        sendToLoggers( close, webSocket );
        removeClient( webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Error )
      {
        IXError error( msg->errorInfo, connection );
        onIXError( error );
        sendToLoggers( error, webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Ping )
      {
        IXPing ping( msg, connection );
        onIXPing( ping );
        sendToLoggers( ping, webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Pong )
      {
        IXPong pong( msg, connection );
        onIXPong( pong );
        sendToLoggers( pong, webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Fragment )
      {
        IXFragment fragment( msg, connection );
        onIXFragment( fragment );
        sendToLoggers( fragment, webSocket );
      }
    } );
}

void WebsocketServer::addClient( const Identifier& identifier, ix::WebSocket& websocket )
{
  std::lock_guard<std::mutex> guard( m_Mutex );
  m_Clients.try_emplace( identifier, websocket );
}

void WebsocketServer::removeClient( ix::WebSocket& websocket )
{
  std::lock_guard<std::mutex> guard( m_Mutex );
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( &it->second == &websocket )
    {
      m_Clients.erase( it->first );
      break;
    }
  }
}

void WebsocketServer::sendToLoggers( Message& message, ix::WebSocket& webSocket )
{
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( magic_enum::enum_cast<Family>( it->first.getFamily() ).value() == Family::Logger && &webSocket != &it->second ) it->second.send( message.dump() );
  }
}

void WebsocketServer::sendToLoggers( Message& message )
{
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( magic_enum::enum_cast<Family>( it->first.getFamily() ).value() == Family::Logger ) it->second.send( message.dump() );
  }
}

void WebsocketServer::sendToLoggers( const Message& message, ix::WebSocket& webSocket )
{
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( magic_enum::enum_cast<Family>( it->first.getFamily() ).value() == Family::Logger && &webSocket != &it->second ) it->second.send( message.dump() );
  }
}

void WebsocketServer::sendToLoggers( const Message& message )
{
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( magic_enum::enum_cast<Family>( it->first.getFamily() ).value() == Family::Logger ) it->second.send( message.dump() );
  }
}

void WebsocketServer::onMessage( Message& message, ix::WebSocket& webSocket )
{
  switch( message.getTypeValue() )
  {
    case MessageType::Exception:
    {
      MessageException& message_exception = reinterpret_cast<MessageException&>( message );
      onException( message_exception );
      sendToLoggers( message_exception, webSocket );
      break;
    }
    case MessageType::Log:
    {
      Log& log = reinterpret_cast<Log&>( message );
      onLog( log );
      sendToLoggers( log, webSocket );
      break;
    }
    case MessageType::UserType:
    {
      UserType& user_type = reinterpret_cast<UserType&>( message );
      onUserType( user_type );
      send( user_type, webSocket );
      break;
    }
    // Receiving here should never happens;
    case MessageType::Open:
    {
      ShouldNotHappens( message, webSocket );
      break;
    }
    case MessageType::Close:
    {
      ShouldNotHappens( message, webSocket );
      break;
    }
    case MessageType::Error:
    {
      ShouldNotHappens( message, webSocket );
      break;
    }
    case MessageType::Ping:
    {
      ShouldNotHappens( message, webSocket );
      break;
    }
    case MessageType::Pong:
    {
      ShouldNotHappens( message, webSocket );
      break;
    }
    case MessageType::Fragment:
    {
      ShouldNotHappens( message, webSocket );
      break;
    }
    case MessageType::Unknown:
    {
      Exception        exception( StatusCode::MESSAGE_TYPE_UNKNOWN, message.dump() );
      MessageException message_exception( exception );
      onException( message_exception );
      // Send back to the cliemt it sent an Unknown type!
      webSocket.send( message_exception.dump() );
      sendToLoggers( message_exception, webSocket );
      break;
    }
  }
}

void WebsocketServer::onIXOpen( IXOpen& open )
{
  std::string headers;
  for( auto it: open.getHeaders() ) { headers += fmt::format( "\t{}: {}\n", it.first, it.second ); }
  debug( fmt::format( fg( fmt::color::green ), "Connection opened:\nId: {}\nRemote IP: {}\nRemote port: {}\nURI: {}\nProtocol: {}\nHeaders:\n{}", open.getId(), open.getRemoteIp(), open.getRemotePort(), open.getURI(), open.getProtocol(), headers ) );
}

void WebsocketServer::onIXClose( IXClose& close )
{
  debug( fmt::format( fg( fmt::color::green ), "Connection closed:\nId: {}\nRemote IP: {}\nRemote port: {}\nCode: {}\nReason: {}\nRemote: {}", close.getId(), close.getRemoteIp(), close.getRemotePort(), close.getCode(), close.getReason(), close.getRemote() ) );
}

void WebsocketServer::onIXError( IXError& err )
{
  error( fmt::format( fg( fmt::color::red ), "Error:\nId: {}\nRemote IP: {}\nRemote port: {}\nRetries: {}\nWait time: {}\nHTTP status: {}\nReason: {}\nCompression error: {}", err.getId(), err.getRemoteIp(), err.getRemotePort(), err.getRetries(),
                      err.getWaitTime(), err.getHttpStatus(), err.getReason(), err.getDecompressionError() ) );
}

void WebsocketServer::onIXPing( IXPing& ping ) { trace( fmt::format( fg( fmt::color::green ), "Ping:\n{}", to_fmt( ping.getContent().dump( 2 ) ) ) ); }

void WebsocketServer::onIXPong( IXPong& pong ) { trace( fmt::format( fg( fmt::color::green ), "Pong:\n{}", to_fmt( pong.getContent().dump( 2 ) ) ) ); }

void WebsocketServer::onIXFragment( IXFragment& fragment ) { debug( fmt::format( fg( fmt::color::green ), "Fragment:\n{}", to_fmt( fragment.getContent().dump( 2 ) ) ) ); }

void WebsocketServer::ShouldNotHappens( Message& message, ix::WebSocket& webSocket )
{
  Exception        exception( StatusCode::INTERNAL_ERROR, fmt::format( "Internal error : {} should not received this message in this function\nMessage received :\n{}", m_Identifier.get(), message.dump() ) );
  MessageException message_exception( exception );
  onException( message_exception );
  // Notify loggers the client and then crash !
  webSocket.send( message_exception.dump() );
  sendToLoggers( message_exception, webSocket );
}

void WebsocketServer::onException( MessageException& message )
{
  Exception exception( static_cast<StatusCode>( message.getCode() ), message.getDescription() );
  critical( "Exception:\n{}", exception.what() );
}

void WebsocketServer::onLog( Log& log ) { m_Logger.log( log.getLevel(), fmt::format( "{} (from {})", log.getLog(), log.getIdentifier().getName() ) ); }

void WebsocketServer::onUserType( UserType& user_type ) { debug( fmt::format( fg( fmt::color::green ), "Fragment:\n{}", to_fmt( user_type.dump( 2 ) ) ) ); }

void WebsocketServer::listen()
{
  if( !m_isListening )
  {
    std::pair<bool, std::string> ret = ix::WebSocketServer::listen();
    if( ret.first )
    {
      m_isListening = ret.first;
      info( "Server listening on {0}:{1}", getHost(), getPort() );
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
    trace( "Server started" );
    ix::WebSocketServer::start();
  }
}

void WebsocketServer::stop( bool useless )
{
  if( !m_isStopped )
  {
    m_isStopped = true;
    useless     = !useless;
    trace( "Server stopped" );
    ix::WebSocketServer::stop();
  }
}

void WebsocketServer::setVerbosity( const LogLevel& verbosity ) { m_Logger.setVerbosity( verbosity ); }

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

void WebsocketServer::send( Message& message )
{
  if( message.getIdentifier().empty() ) message.setFrom( m_Identifier );
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it ) { it->second.send( message.dump() ); }
}

void WebsocketServer::send( const Message& message )
{
  Message ret{ message };
  if( message.getIdentifier().empty() ) ret.setFrom( m_Identifier );
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it ) { it->second.send( ret.dump() ); }
}

void WebsocketServer::send( Message& message, ix::WebSocket& webSocket )
{
  if( message.getIdentifier().empty() ) message.setFrom( m_Identifier );
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( &webSocket != &it->second ) it->second.send( message.dump() );
  }
}

void WebsocketServer::send( const Message& message, ix::WebSocket& webSocket )
{
  Message ret{ message };
  if( message.getIdentifier().empty() ) ret.setFrom( m_Identifier );
  for( std::map<Identifier, ix::WebSocket&>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it )
  {
    if( &webSocket != &it->second ) it->second.send( ret.dump() );
  }
}

}  // namespace yaodaq
