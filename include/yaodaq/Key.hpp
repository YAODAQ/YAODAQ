#ifndef YAODAQ_KEY_HPP
#define YAODAQ_KEY_HPP

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>

#include "yaodaq/Classification.hpp"

namespace yaodaq
{

class Key
{
private:
  std::int_least32_t m_Key{0};

public:
  explicit Key() = default;
  explicit Key(const Domain& domain, const Class& c_lass, const Family& family);
  [[nodiscard]] std::int_least8_t getDomain() const;
  [[nodiscard]] std::int_least8_t getClass() const;
  [[nodiscard]] std::int_least16_t getFamily() const;
  [[nodiscard]] std::int_least32_t getKey() const;
};

}  // namespace yaodaq

#endif  // YAODAQ_KEY_HPP
