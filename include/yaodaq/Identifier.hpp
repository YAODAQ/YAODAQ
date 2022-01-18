#ifndef YAODAQ_IDENTIFIER_HPP
#define YAODAQ_IDENTIFIER_HPP

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Class.hpp"

#include <string>

namespace yaodaq
{

class Identifier
{
public:
  Identifier() = default;
  Identifier(const Class& aClass, const std::string& type, const std::string& name);
  [[nodiscard]] std::string getClass() const;
  [[nodiscard]] std::string getType() const;
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] Class       getClassId() const;
  [[nodiscard]] std::string get() const;

private:
  Class       m_Class{Class::Unknown};
  std::string m_Type{"Unknown"};
  std::string m_Name{"Unknown"};
};

}  // namespace yaodaq

#endif  // YAODAQ_IDENTIFIER_HPP
