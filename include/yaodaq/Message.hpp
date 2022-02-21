#ifndef YAODAQ_MESSAGE
#define YAODAQ_MESSAGE

/**
\copyright Copyright 2022 flagarde
*/

#include "nlohmann/json.hpp"

#include "yaodaq/MessageType.hpp"
#include "yaodaq/LogLevel.hpp"
#include "yaodaq/Utility.hpp"

#include <string>

namespace yaodaq
{

class Identifier;
class Exception;

class Message
{
public:
  Message();
  explicit Message( const nlohmann::json& content, const MessageType& messageType = MessageType::Unknown );
  explicit Message( const std::string& content, const MessageType& messageType = MessageType::Unknown );
  explicit Message( const char* content, const MessageType& messageType = MessageType::Unknown );
  std::string    dump( const int& indent = -1, const char& indent_char = ' ', const bool& ensure_ascii = false, const nlohmann::detail::error_handler_t& error_handler = nlohmann::detail::error_handler_t::strict ) const;
  nlohmann::json get() const;
  nlohmann::json getContent() const;
  std::string    getTypeName() const;
  MessageType    getTypeValue() const;
  std::string    getTimestamp() const;
  std::time_t    getTime() const;
  Identifier     getIdentifier() const;
  void           setFrom( const Identifier& );

protected:
  explicit Message( const MessageType& messageType );
  void           setContent( const nlohmann::json& content );
  void           setContent( const std::string& content );
  void           setContent( const char* content );
  nlohmann::json m_JSON;
};

// Exception
class MessageException : public Message
{
public:
  explicit MessageException( const Exception& content );
  std::int_least32_t getCode();
  std::string        getDescription();
  std::int_least32_t getLine();
  std::int_least32_t getColumn();
  std::string        getFileName();
  std::string        getFunctionName();
};

// Log and co.
class Log : public Message
{
public:
  Log(const LogLevel& level,const std::string& log);
  Log(const std::string& log);
  LogLevel getLevel() const;
  std::string getLog() const;
};

class Trace : public Log
{
public:
  Trace(const std::string& log);
};

class Debug : public Log
{
public:
  Debug(const std::string& log);
};

class Info : public Log
{
public:
  Info(const std::string& log);
};

class Warn : public Log
{
public:
  Warn(const std::string& log);
};

class Error : public Log
{
public:
  Error(const std::string& log);
};

class Critical : public Log
{
public:
  Critical(const std::string& log);
};

class UserType : public Message
{
public:
  explicit UserType(const std::string& type, const nlohmann::json& content);
  explicit UserType(const std::string& type, const std::string& content);
  explicit UserType(const std::string& type, const char* content);
  std::string getType();
};

}  // namespace yaodaq

#endif  // YAODAQ_MESSAGE
