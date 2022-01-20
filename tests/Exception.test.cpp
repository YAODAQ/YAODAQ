/**
\copyright Copyright 2022 flagarde
*/
#include "yaodaq/Exception.hpp"

#include "yaodaq/StatusCode.hpp"

#include <doctest/doctest.h>
#include <iostream>

TEST_CASE( "Exception" )
{
  yaodaq::Exception exception( yaodaq::StatusCode::LISTEN_ERROR, "My exception" );
  try
  {
    throw exception;
  }
  catch( const yaodaq::Exception& ex )
  {
    std::cout << ex.code() << std::endl;
    std::cout << ex.what() << std::endl;
    std::cout << ex.description() << std::endl;
  }
}
