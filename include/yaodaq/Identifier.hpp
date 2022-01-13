#ifndef YAODAQ_IDENTIFIER_H_
#define YAODAQ_IDENTIFIER_H_

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

#endif
