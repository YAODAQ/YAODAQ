#ifndef YAODAQ_MESSAGE
#define YAODAQ_MESSAGE

/**
\copyright Copyright 2022 flagarde
*/

#include <string>

#include "nlohmann/json.hpp"

#include "yaodaq/MessageType.hpp"

namespace yaodaq
{

class Identifier;

class Message
{
public:
  Message();
  explicit Message(const nlohmann::json& content, const MessageType& messageType = MessageType::Unknown);
  explicit Message(const std::string& content, const MessageType& messageType = MessageType::Unknown);
  explicit Message(const char* content, const MessageType& messageType = MessageType::Unknown);
  std::string dump(const int& indent = -1,const char& indent_char = ' ',const bool& ensure_ascii = false, const nlohmann::detail::error_handler_t& error_handler = nlohmann::detail::error_handler_t::strict) const;
  nlohmann::json get() const;
  std::string getContent() const;
  std::string getTypeName() const;
  MessageType getTypeValue() const;
  std::string getTimestamp() const;
  std::time_t getTime() const;
  Identifier getIdentifier() const;
  void setFrom(const Identifier&);
protected:
  explicit Message(const MessageType& messageType);
  void setContent(const nlohmann::json& content);
  void setContent(const std::string& content);
  void setContent(const char* content);
private:
  nlohmann::json m_JSON;
};

}

#endif  // YAODAQ_MESSAGE
