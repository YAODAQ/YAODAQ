/**
\copyright Copyright 2022 flagarde
*/

#include "doctest/doctest.h"
#include "yaodaq/Message.hpp"
#include "yaodaq/WebsocketClient.hpp"
#include "yaodaq/WebsocketServer.hpp"

#include <chrono>
#include <thread>

void quit_clients( yaodaq::WebsocketClient& client1, yaodaq::WebsocketClient& client2, yaodaq::WebsocketClient& client3, yaodaq::WebsocketClient& not_connected )
{
  // Client3 have the same name than client2 so it should be closed by the server
  std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  for( int i = 10; i > 0; --i )
  {
    client1.logger()->warn( "Killing client1 in {} seconds ({})", i, ix::WebSocket::readyStateToString( client1.getReadyState() ) );
    client1.send( "I'm client 1" );
    client2.logger()->warn( "Killing client2 in {} seconds ({})", i, ix::WebSocket::readyStateToString( client2.getReadyState() ) );
    client2.send( "I'm client 2" );
    client3.logger()->warn( "Killing client3 in {} seconds ({})", i, ix::WebSocket::readyStateToString( client3.getReadyState() ) );
    client3.send( "I'm client 3" );
    not_connected.logger()->warn( "Killing not_connected in {} seconds ({})", i, ix::WebSocket::readyStateToString( not_connected.getReadyState() ) );
    not_connected.send( "YOU SHOULD  NOT SEE THIS !!!!!" );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
  client1.stop();
  client2.stop();
  client3.stop();
  not_connected.stop();
}

void quit_servers( yaodaq::WebsocketServer& server1, yaodaq::WebsocketServer& server2 )
{
  for( int i = 30; i > 0; --i )
  {
    server1.logger()->warn( "Killing server1 in {} seconds", i );
    server2.logger()->warn( "Killing server2 in {} seconds", i );

    server1.sendToLoggers( yaodaq::Message( "Killing server1 in {} seconds" ) );
    server1.sendToLoggers( yaodaq::Message( "Killing server2 in {} seconds" ) );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
  server1.stop();
  server2.stop();
  std::exit( 0 );
}

TEST_CASE( "WebsocketClientsServers" )
{
  // Client3 have the same name than client2  but is not connected to the same server so it should be all good !
  // not_connected should not be connected because it has the same name and is connected to the same server

  yaodaq::WebsocketServer server1( "MyWebsocketServer", 8080, "127.0.0.1" );
  yaodaq::WebsocketServer server2( "MyWebsocketServer", 8888, "127.0.0.1" );

  yaodaq::WebsocketClient client1( "MyWebsocketClient1" );
  yaodaq::WebsocketClient client2( "MyWebsocketClient2" );
  yaodaq::WebsocketClient client3( "MyWebsocketClient2" );
  yaodaq::WebsocketClient not_connected( "MyWebsocketClient2" );

  client1.setUrl( "ws://127.0.0.1:8888/" );
  client2.setUrl( "ws://127.0.0.1:8888/" );
  not_connected.setUrl( "ws://127.0.0.1:8888/" );
  client3.setUrl( "ws://127.0.0.1:8080/" );

  std::thread m_quit_clients( quit_clients, std::ref( client1 ), std::ref( client2 ), std::ref( client3 ), std::ref( not_connected ) );
  m_quit_clients.detach();

  std::thread m_quit_servers( quit_servers, std::ref( server1 ), std::ref( server2 ) );
  m_quit_servers.detach();

  client1.loop();
  client2.loop();
  client3.loop();
  not_connected.loop();

  server1.loop();
  server2.loop();
}
