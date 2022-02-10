/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketServer.hpp"

#include "doctest/doctest.h"

#include <chrono>
#include <thread>

TEST_CASE( "WebsocketServer" )
{
  yaodaq::WebsocketServer server( "MyWebsocketServer", 8080, "127.0.0.1" );
  server.listen();
  server.start();
  for( int i = 10; i > 0; --i )
  {
    server.logger()->warn( "Killing server in {} seconds", i );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
  server.stop();
}
