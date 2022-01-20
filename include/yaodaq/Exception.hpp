#ifndef YAODAQ_EXCEPTION
#define YAODAQ_EXCEPTION

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>
#include <exception>
#include <fmt/color.h>
#include <source_location/source_location.hpp>
#include <string>

namespace yaodaq
{

enum class StatusCode : std::int_least32_t;

class Exception : public std::exception, public source_location
{
public:
  Exception() = delete;

  static void setFormat( const std::string& format ) { m_Format = format; }

  static void setStyle( const fmt::text_style& style = {} ) { m_Style = style; }

  Exception( const StatusCode& statusCode, const std::string& description, const source_location& location = source_location::current() );
  ~Exception() noexcept override = default;
  [[nodiscard]] const char*        what() const noexcept final;
  [[nodiscard]] const char*        description() const noexcept;
  [[nodiscard]] std::int_least32_t code() const noexcept;

private:
  static fmt::text_style   m_Style;
  static std::string       m_Format;
  const std::int_least32_t m_Code{ 0 };
  std::string              m_Description;
  std::string              m_Message;
  void                     constructMessage();
};

}  // namespace yaodaq

#endif
