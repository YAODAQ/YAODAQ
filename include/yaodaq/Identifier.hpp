#ifndef INCLUDE_YAODAQ_IDENTIFIER_HPP_
#define INCLUDE_YAODAQ_IDENTIFIER_HPP_

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
  Identifier(const Class&, const std::string&, const std::string&);
  std::string getClass() const;
  std::string getType() const;
  std::string getName() const;
  Class       getClassId() const;
  std::string get() const;

private:
  Class       m_Class{Class::Unknown};
  std::string m_Type{"Unknown"};
  std::string m_Name{"Unknown"};
};

}  // namespace yaodaq

#endif  // INCLUDE_YAODAQ_IDENTIFIER_HPP_
