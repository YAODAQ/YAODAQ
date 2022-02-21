/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Message.hpp"

#include "fmt/chrono.h"
#include "magic_enum.hpp"
#include "yaodaq/Classification.hpp"
#include "yaodaq/Exception.hpp"
#include "yaodaq/Identifier.hpp"
#include "yaodaq/LogLevel.hpp"

#include <chrono>
#include <ctime>
#include <ixwebsocket/IXUuid.h>
#include <string>

// Versions numbers
#include <ixwebsocket/IXWebSocketVersion.h>
#include <yaodaq/YaodaqVersion.hpp>

namespace yaodaq
{

Message::Message()
{
  m_JSON["from"];
  m_JSON["to"];
  m_JSON["type"] = magic_enum::enum_name( MessageType::Unknown );
  m_JSON["uuid"] = ix::uuid4();
  m_JSON["content"];
  m_JSON["timestamp"]                       = fmt::format( "{:%F %T %z}", fmt::gmtime( std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() ) ) );
  m_JSON["meta"]["compiler"]                = nlohmann::json::meta()["compiler"];
  m_JSON["meta"]["platform"]                = nlohmann::json::meta()["platform"];
  m_JSON["meta"]["versions"]["json"]        = nlohmann::json::meta()["version"]["string"];
  m_JSON["meta"]["versions"]["yaodaq"]      = yaodaq_version.to_string();
  m_JSON["meta"]["versions"]["ixwebsocket"] = std::string( IX_WEBSOCKET_VERSION );
}

void Message::setContent( const nlohmann::json& content ) { m_JSON["content"] = static_cast<nlohmann::json>( content ); }

void Message::setContent( const std::string& content )
{
  m_JSON["content"] = nlohmann::json::parse( content, nullptr, false );
  if( m_JSON["content"].is_discarded() ) { m_JSON["content"] = static_cast<std::string>( content ); }
}

void Message::setContent( const char* content )
{
  m_JSON["content"] = nlohmann::json::parse( content, nullptr, false );
  if( m_JSON["content"].is_discarded() ) { m_JSON["content"] = static_cast<std::string>( content ); }
}

Message::Message( const nlohmann::json& content, const MessageType& messageType ) : Message( messageType ) { setContent( content ); }

Message::Message( const std::string& content, const MessageType& messageType ) : Message( messageType ) { setContent( content ); }

Message::Message( const char* content, const MessageType& messageType ) : Message( messageType ) { setContent( content ); }

std::string Message::dump( const int& indent, const char& indent_char, const bool& ensure_ascii, const nlohmann::detail::error_handler_t& error_handler ) const { return m_JSON.dump( indent, indent_char, ensure_ascii, error_handler ); }

nlohmann::json Message::get() const { return m_JSON; }

std::string Message::getTypeName() const { return m_JSON["type"].get<std::string>(); }

MessageType Message::getTypeValue() const { return magic_enum::enum_cast<MessageType>( m_JSON["type"].get<std::string>() ).value(); }

nlohmann::json Message::getContent() const { return m_JSON["content"]; }

std::string Message::getTimestamp() const { return m_JSON["timestamp"].get<std::string>(); }

std::time_t Message::getTime() const
{
  std::tm tm;
  memset( &tm, 0, sizeof( tm ) );
  std::istringstream ss( getTimestamp() );
  ss >> std::get_time( &tm, "%Y-%m-%d %H:%M:%S %z" );
  return mktime( &tm );
}

void Message::setFrom( const Identifier& identifier )
{
  m_JSON["from"]["name"]   = identifier.getName();
  m_JSON["from"]["type"]   = identifier.getType();
  m_JSON["from"]["family"] = identifier.getFamily();
  m_JSON["from"]["class"]  = identifier.getClass();
  m_JSON["from"]["domain"] = identifier.getDomain();
}

Identifier Message::getIdentifier() const
{
  if( m_JSON["from"].is_null() ) return {};
  else
  {
    Identifier id( m_JSON["from"]["type"].get<std::string>(), m_JSON["from"]["name"].get<std::string>() );
    id.generateKey( magic_enum::enum_cast<Domain>( m_JSON["from"]["domain"].get<std::string>() ).value(), magic_enum::enum_cast<Class>( m_JSON["from"]["class"].get<std::string>() ).value(),
                    magic_enum::enum_cast<Family>( m_JSON["from"]["family"].get<std::string>() ).value() );
    return id;
  }
}

Message::Message( const MessageType& messageType ) : Message() { m_JSON["type"] = magic_enum::enum_name( messageType ); }

// MessageException
MessageException::MessageException( const Exception& exception ) : Message( MessageType::Exception )
{
  nlohmann::json j;
  j["code"]          = exception.code();
  j["description"]   = exception.description();
  j["line"]          = exception.line();
  j["column"]        = exception.column();
  j["file_name"]     = exception.file_name();
  j["function_name"] = exception.function_name();
  setContent( j );
}

std::int_least32_t MessageException::getCode() { return get()["content"]["code"].get<std::int_least32_t>(); }

std::string MessageException::getDescription() { return get()["content"]["description"].get<std::string>(); }

std::int_least32_t MessageException::getLine() { return get()["content"]["line"].get<std::int_least32_t>(); }

std::int_least32_t MessageException::getColumn() { return get()["content"]["column"].get<std::int_least32_t>(); }

std::string MessageException::getFileName() { return get()["content"]["file_name"].get<std::string>(); }

std::string MessageException::getFunctionName() { return get()["content"]["function_name"].get<std::string>(); }

// Log
Log::Log(const LogLevel& level,const std::string& log) : Message( MessageType::Log )
{
  nlohmann::json j;
  j["verbosity"] = magic_enum::enum_name(level);
  j["log"] = log;
  setContent( j );
}

Log::Log(const std::string& log) : Message( MessageType::Log )
{
  nlohmann::json j;
  j["verbosity"] = magic_enum::enum_name(LogLevel::Info);
  j["log"] = log;
  setContent( j );
}

LogLevel Log::getLevel() const
{
  return magic_enum::enum_cast<LogLevel>(get()["content"]["verbosity"].get<std::string>()).value();
}

std::string Log::getLog() const
{
  return get()["content"]["log"].get<std::string>();
}

// Trace
Trace::Trace(const std::string& log) : Log(LogLevel::Trace,log){}

// Debug
Debug::Debug(const std::string& log) : Log(LogLevel::Debug,log){}

// Info
Info::Info(const std::string& log) : Log(LogLevel::Info,log){}

// Warn
Warn::Warn(const std::string& log) : Log(LogLevel::Warn,log){}

// Error
Error::Error(const std::string& log) : Log(LogLevel::Error,log){}

//Critical
Critical::Critical(const std::string& log) : Log(LogLevel::Critical,log){}

// UserType
UserType::UserType(const std::string& type, const nlohmann::json& content) : Message(content, MessageType::UserType )
{
  m_JSON["user_type"]=type;
}

UserType::UserType(const std::string& type, const std::string& content) : Message(content, MessageType::UserType )
{
  m_JSON["user_type"]=type;
}

UserType::UserType(const std::string& type, const char* content) : Message(content, MessageType::UserType )
{
  m_JSON["user_type"]=type;
}

std::string UserType::getType()
{
  return get()["user_type"].get<std::string>();
}

}  // namespace yaodaq
