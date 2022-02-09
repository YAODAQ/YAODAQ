/**
\copyright Copyright 2022 flagarde
*/

#include "doctest/doctest.h"
#include "yaodaq/WebsocketClient.hpp"
#include "yaodaq/WebsocketServer.hpp"

#include <chrono>
#include <thread>

void quit_clients( yaodaq::WebsocketClient& client1, yaodaq::WebsocketClient& client2, yaodaq::WebsocketClient& client3 )
{
  for( int i = 10; i > 0; --i )
  {
    client1.logger()->warn( "Killing client1 in {} seconds", i );
    client2.logger()->warn( "Killing client2 in {} seconds", i );
    client3.logger()->warn( "Killing client2 in {} seconds", i );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
  client1.stop();
  client2.stop();
  client3.stop();
}

void quit_servers( yaodaq::WebsocketServer& server1, yaodaq::WebsocketServer& server2 )
{
  for( int i = 30; i > 0; --i )
  {
    server1.logger()->warn( "Killing server1 in {} seconds", i );
    server2.logger()->warn( "Killing server2 in {} seconds", i );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
  server1.stop();
  server2.stop();
  std::exit( 0 );
}

TEST_CASE( "WebsocketClientsServers" )
{
  yaodaq::WebsocketServer server1( "MyWebsocketServer", 8080, "127.0.0.1" );
  yaodaq::WebsocketServer server2( "MyWebsocketServer", 8888, "127.0.0.1" );
  yaodaq::WebsocketClient client1( "MyWebsocketClient1" );
  yaodaq::WebsocketClient client2( "MyWebsocketClient2" );
  yaodaq::WebsocketClient client3( "MyWebsocketClient3" );

  client1.setUrl( "ws://127.0.0.1:8888/" );
  client2.setUrl( "ws://127.0.0.1:8888/" );
  client3.setUrl( "ws://127.0.0.1:8080/" );

  std::thread m_quit_clients( quit_clients, std::ref( client1 ), std::ref( client2 ), std::ref( client3 ) );
  m_quit_clients.detach();

  std::thread m_quit_servers( quit_servers, std::ref( server1 ), std::ref( server2 ) );
  m_quit_servers.detach();

  client1.loop();
  client2.loop();
  client3.loop();

  server1.loop();
  server2.loop();
}
