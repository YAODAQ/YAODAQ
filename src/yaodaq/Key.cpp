/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Key.hpp"

#include <cstdint>

namespace yaodaq
{
Key::Key( const Domain& domain, const Class& c_lass, const Family& family ) { m_Key = ( static_cast<std::int_least8_t>( domain ) << 24 ) + ( static_cast<std::int_least8_t>( c_lass ) << 16 ) + static_cast<std::int_least16_t>( family ); }

std::int_least8_t Key::getDomain() const { return ( m_Key >> 24 ) & 0xFF; }

std::int_least8_t Key::getClass() const { return ( m_Key >> 16 ) & 0xFF; }

std::int_least16_t Key::getFamily() const { return ( m_Key ) & 0xFFFF; }

std::int_least32_t Key::getKey() const { return m_Key; }

}  // namespace yaodaq
