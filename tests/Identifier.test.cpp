#include "yaodaq/Identifier.hpp"

#include "doctest/doctest.h"

TEST_CASE("Identifier")
{
  yaodaq::Identifier id(yaodaq::Class::WebSocketServer, "MyType", "MyName");
  CHECK(id.getName() == "MyName");
  CHECK(id.getType() == "MyType");
  CHECK(id.getClass() == "WebSocketServer");
  CHECK(id.getClassId() == yaodaq::Class::WebSocketServer);
}
