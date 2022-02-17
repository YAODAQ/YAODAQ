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
        // sendToLoggers(ixmessage,webSocket);
        onMessage( ixmessage );
      }
      else if( msg->type == ix::WebSocketMessageType::Open )
      {
        // Check if a client with the same name is already connected;
        connection->computeId( getHost() + ":" + std::to_string( getPort() ), Identifier::parse( msg->openInfo.headers["id"] ) );
        if( connection->isTerminated() )
        {
          logger()->error( fmt::format( fg( fmt::color::red ) | fmt::emphasis::bold, "One client with the name \"{}\" is already connected !", Identifier::parse( msg->openInfo.headers["id"] ).getName() ) );
          MessageException message_exception(
            Exception( StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED, fmt::format( "One client with the name \"{}\" is already connected to ws{}://{}:{} !", Identifier::parse( msg->openInfo.headers["id"] ).getName(), "", getHost(), getPort() ) ) );

          // Send to the client on exception;
          webSocket.send( message_exception.dump() );
          // Send to loggers except the client as it has been send before;
          // sendToLoggers(message_exception,webSocket);

          webSocket.stop( magic_enum::enum_integer( StatusCode::CLIENT_WITH_SAME_NAME_ALREADY_CONNECTED ),
                          fmt::format( "One client with the name \"{}\" is already connected to ws{}://{}:{} !", Identifier::parse( msg->openInfo.headers["id"] ).getName(), "", getHost(), getPort() ) );
          std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
          return;
        }
        addClient( Identifier::parse( msg->openInfo.headers["id"] ), webSocket );
        Open open( msg->openInfo, connection );
        // sendToLoggers( open, webSocket );
        onOpen( open );
      }
      else if( msg->type == ix::WebSocketMessageType::Close )
      {
        Close close( msg->closeInfo, connection );
        // sendToLoggers( close, webSocket );
        onClose( close );
        removeClient( webSocket );
      }
      else if( msg->type == ix::WebSocketMessageType::Error )
      {
        Error error( msg->errorInfo, connection );
        // sendToLoggers( error, webSocket );
        onError( error );
      }
      else if( msg->type == ix::WebSocketMessageType::Ping )
      {
        Ping ping( msg, connection );
        // sendToLoggers( ping, webSocket );
        onPing( ping );
      }
      else if( msg->type == ix::WebSocketMessageType::Pong )
      {
        Pong pong( msg, connection );
        // sendToLoggers( pong, webSocket );
        onPong( pong );
      }
      else if( msg->type == ix::WebSocketMessageType::Fragment )
      {
        Fragment fragment( msg, connection );
        // sendToLoggers( fragment, webSocket );
        onFragment( fragment );
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

void WebsocketServer::onMessage( Message& message )
{
  switch( message.getTypeValue() )
  {
    case MessageType::Exception:
      MessageException& message_exception = reinterpret_cast<MessageException&>( message );
      onException( message_exception );
      break;
    default: logger()->critical( "SSSS" );
  }
}

void WebsocketServer::onOpen( Open& open )
{
  std::string headers;
  for( auto it: open.getHeaders() ) { headers += fmt::format( "\t{}: {}\n", it.first, it.second ); }
  logger()->debug( fmt::format( fg( fmt::color::green ), "Connection opened:\nId: {}\nRemote IP: {}\nRemote port: {}\nURI: {}\nProtocol: {}\nHeaders:\n{}", open.getId(), open.getRemoteIp(), open.getRemotePort(), open.getURI(), open.getProtocol(), headers ) );
}

void WebsocketServer::onClose( Close& close )
{
  logger()->debug(
    fmt::format( fg( fmt::color::green ), "Connection closed:\nId: {}\nRemote IP: {}\nRemote port: {}\nCode: {}\nReason: {}\nRemote: {}", close.getId(), close.getRemoteIp(), close.getRemotePort(), close.getCode(), close.getReason(), close.getRemote() ) );
}

void WebsocketServer::onError( Error& error )
{
  logger()->error( fmt::format( fg( fmt::color::red ), "Error:\nId: {}\nRemote IP: {}\nRemote port: {}\nRetries: {}\nWait time: {}\nHTTP status: {}\nReason: {}\nCompression error: {}", error.getId(), error.getRemoteIp(), error.getRemotePort(),
                                error.getRetries(), error.getWaitTime(), error.getHttpStatus(), error.getDecompressionError() ) );
}

void WebsocketServer::onPing( Ping& ping ) { logger()->debug( fmt::format( fg( fmt::color::green ), "Ping:\n{}", ping.getContent().dump( 2 ) ) ); }

void WebsocketServer::onPong( Pong& pong ) { logger()->debug( fmt::format( fg( fmt::color::green ), "Pong:\n{}", pong.getContent().dump( 2 ) ) ); }

void WebsocketServer::onFragment( Fragment& fragment ) {}

void WebsocketServer::onException( MessageException& message ) {}

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
