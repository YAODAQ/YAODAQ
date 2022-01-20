/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/WebsocketClient.hpp"

#include "doctest/doctest.h"

#include <chrono>
#include <iostream>
#include <thread>

TEST_CASE( "WebsocketClient" )
{
  yaodaq::WebsocketClient client( "MyWebsocketClient1" );
  client.setUrl( "ws://127.0.0.1:8080/" );
  yaodaq::WebsocketClient client2( "MyWebsocketClient2" );
  client2.setUrl( "ws://127.0.0.1:8080/" );
  client.loop();
  client2.loop();
}
