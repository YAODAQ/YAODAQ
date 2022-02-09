/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Identifier.hpp"

#include "yaodaq/Class.hpp"

#include <fmt/color.h>
#include <magic_enum.hpp>
#include <string>

namespace yaodaq
{

Identifier::Identifier( const Class& aClass, const std::string& type, const std::string& name ) : m_Class( aClass ), m_Type( type ), m_Name( name ) {}

std::string Identifier::getClass() const { return std::string( magic_enum::enum_name( m_Class ) ); }

std::string Identifier::getType() const { return m_Type; }

std::string Identifier::getName() const { return m_Name; }

Class Identifier::getClassId() const { return m_Class; }

std::string Identifier::get() const { return fmt::format( "{0}/{1}/{2}", getClass(), getType(), getName() ); }

}  // namespace yaodaq
