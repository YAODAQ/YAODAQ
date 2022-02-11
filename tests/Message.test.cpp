/**
\copyright Copyright 2022 flagarde
*/
#include "yaodaq/Message.hpp"

#include "doctest/doctest.h"
#include "yaodaq/Identifier.hpp"

#include <iostream>

TEST_CASE( "Testing the Message class." )
{
  // Default
  yaodaq::Message message;
  std::cout << "message : \n" << message.dump( 1, ' ' ) << std::endl;
  std::cout << message.getTypeName() << " " << message.getTypeValue() << " " << message.getContent() << std::endl;
  // Json
  nlohmann::json  content_json = nlohmann::json::parse( R"({"happy": true, "pi": 3.141})" );
  yaodaq::Message message_json( content_json, yaodaq::MessageType::Open );
  std::cout << "message_json : \n" << message_json.dump( 1, ' ' ) << std::endl;

  // With const char*
  yaodaq::Message message_const_char_star( "happy", yaodaq::MessageType::Open );
  std::cout << "message_const_char_star : \n" << message_const_char_star.dump( 1, ' ' ) << std::endl;
  std::cout << message_const_char_star.getTypeName() << " " << message_const_char_star.getTypeValue() << " " << message_const_char_star.getContent() << std::endl;

  // With const char* with json
  yaodaq::Message message_const_char_star_json( R"({"happy": true, "pi": 3.141})", yaodaq::MessageType::Open );
  std::cout << "message_const_char_star_json : \n" << message_const_char_star_json.dump( 1, ' ' ) << std::endl;

  // With std::string
  std::string     content{ "happy" };
  yaodaq::Message message_string( content, yaodaq::MessageType::Open );
  std::cout << "message_string : \n" << message_string.dump( 1, ' ' ) << std::endl;

  // With std::string with json
  std::string     content_string_json{ R"({"happy": true, "e": 2.71828})" };
  yaodaq::Message message_content_string_json( content_string_json, yaodaq::MessageType::Open );
  std::cout << "message_content_string_json : \n" << message_content_string_json.dump( 1, ' ' ) << std::endl;

  std::cout << message_content_string_json.getTypeName() << " " << message_content_string_json.getTypeValue() << " " << message_content_string_json.getContent() << " " << message_content_string_json.getTime() << " "
            << message_content_string_json.getTimestamp() << std::endl;
}

TEST_CASE( "Testing the setFrom." )
{
  yaodaq::Identifier identifier( "MyName", "MyType" );
  identifier.generateKey( yaodaq::Domain::Application, yaodaq::Class::Module, yaodaq::Family::FileWriter );
  yaodaq::Message message( "My message", yaodaq::MessageType::Close );
  message.setFrom( identifier );
  std::cout << message.dump( 2 ) << std::endl;

  std::cout << message.getIdentifier().getDomain() << std::endl;
  std::cout << message.getIdentifier().getClass() << std::endl;
  std::cout << message.getIdentifier().getFamily() << std::endl;
  std::cout << message.getIdentifier().getType() << std::endl;
  std::cout << message.getIdentifier().getName() << std::endl;
  CHECK( message.getIdentifier().getDomain() == identifier.getDomain() );
  CHECK( message.getIdentifier().getClass() == identifier.getClass() );
  CHECK( message.getIdentifier().getFamily() == identifier.getFamily() );
  CHECK( message.getIdentifier().getType() == identifier.getType() );
  CHECK( message.getIdentifier().getName() == identifier.getName() );
}
