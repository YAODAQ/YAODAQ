/**
\copyright Copyright 2022 flagarde
*/
#include "yaodaq/YaodaqInfos.hpp"

#include <doctest/doctest.h>
#include <iostream>

TEST_CASE( "YaodaqInfos" )
{
  std::cout << yaodaq::yaodaq_description << std::endl;
  std::cout << yaodaq::yaodaq_url << std::endl;
  std::cout << yaodaq::yaodaq_online_documentation << std::endl;
  std::cout << yaodaq::yaodaq_logo << std::endl;
  std::cout << yaodaq::yaodaq_version.to_string() << std::endl;
}
