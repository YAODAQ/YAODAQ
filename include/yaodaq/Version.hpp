#ifndef YAODAQ_VERSION
#define YAODAQ_VERSION

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>
#include <semver.hpp>
#include <string>

namespace yaodaq
{

class Version : public semver::version
{
public:
  constexpr Version( const std::uint8_t& mj, const std::uint8_t& mn, const std::uint8_t& pt, const semver::prerelease& prt = semver::prerelease::none, const std::uint8_t& prn = 0 ) noexcept : semver::version( mj, mn, pt, prt, prn ) {}
  explicit constexpr Version( const std::string_view& str ) : semver::version( str ) {}
  constexpr Version() = default;
  std::uint8_t getMajor();
  std::uint8_t getMinor();
  std::uint8_t getPatch();
  std::string  getPreRelease();
  std::uint8_t getPreReleaseNumber();
};

}  // namespace yaodaq

#endif  // YAODAQ_VERSION
