#ifndef YAODAQ_IDENTIFIER_HPP
#define YAODAQ_IDENTIFIER_HPP

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Key.hpp"

#include <cstdint>
#include <string>

namespace yaodaq
{

class Identifier
{
public:
  Identifier() = default;
  Identifier( const std::string& type, const std::string& name );
  void                      generateKey( const Domain& domain = Domain::Unknown, const Class& c_lass = Class::Unknown, const Family& family = Family::Unknown );
  [[nodiscard]] std::string getDomain() const;
  [[nodiscard]] std::string getClass() const;
  [[nodiscard]] std::string getFamily() const;
  [[nodiscard]] std::string getType() const;
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] Key         getKey() const;
  [[nodiscard]] std::string get() const;
  bool                      empty() const;
  static Identifier         parse( const std::string& );

private:
  std::string m_Type{ "Unknown" };
  std::string m_Name{ "Unknown" };
  Key         m_Key;
};

}  // namespace yaodaq

#endif  // YAODAQ_IDENTIFIER_HPP
