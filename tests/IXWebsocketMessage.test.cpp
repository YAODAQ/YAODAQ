/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/IXWebsocketMessage.hpp"

#include "doctest/doctest.h"

#include <map>
#include <string>
#include <iostream>

TEST_CASE( "IXWebsocketMessage : Open" )
{

  ix::WebSocketOpenInfo openInfo("http::127.0.0.1",{{"a","1"},{"b","2"}},"MyProtocol");
  yaodaq::Open open(openInfo);
  std::cout<<open.dump(2)<<std::endl;

  CHECK_EQ( open.getURI(), openInfo.uri );
  CHECK_EQ( open.getProtocol(), openInfo.protocol );

}
