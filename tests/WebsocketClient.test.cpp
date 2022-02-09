/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketClient.hpp"

#include "doctest/doctest.h"

#include <chrono>
#include <csignal>
#include <thread>

void quit_function( yaodaq::WebsocketClient& client1, yaodaq::WebsocketClient& client2 )
{
  for( int i = 10; i > 0; --i )
  {
    client1.logger()->warn( "Killing client1 in {} seconds", i );
    client2.logger()->warn( "Killing client2 in {} seconds", i );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
  client1.stop();
  client2.stop();
  std::exit( 0 );
}

TEST_CASE( "WebsocketClient" )
{
  yaodaq::WebsocketClient client1( "MyWebsocketClient1" );
  yaodaq::WebsocketClient client2( "MyWebsocketClient2" );
  std::thread             quit( quit_function, std::ref( client1 ), std::ref( client2 ) );
  client1.setUrl( "ws://127.0.0.1:8888/" );
  client2.setUrl( "ws://127.0.0.1:8888/" );
  quit.detach();
  client1.loop();
  client2.loop();
}
