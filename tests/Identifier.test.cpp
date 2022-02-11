/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"

#include "doctest/doctest.h"

TEST_CASE( "Identifier" )
{
  yaodaq::Identifier id( "MyType", "MyName" );
  id.generateKey( yaodaq::Domain::Application, yaodaq::Class::Module, yaodaq::Family::Logger );

  CHECK_EQ( id.getDomain(), "Application" );
  CHECK_EQ( id.getClass(), "Module" );
  CHECK_EQ( id.getFamily(), "Logger" );
  CHECK_EQ( id.getType(), "MyType" );
  CHECK_EQ( id.getName(), "MyName" );
  CHECK_EQ( id.get(), "Application/Module/Logger/MyType/MyName" );
}
