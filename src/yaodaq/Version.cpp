/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Version.hpp"

#include <magic_enum.hpp>

namespace yaodaq
{

std::uint8_t Version::getMajor() { return major; }

std::uint8_t Version::getMinor() { return minor; }

std::uint8_t Version::getPatch() { return patch; }

std::string Version::getPreRelease() { return std::string( magic_enum::enum_name( prerelease_type ) ); }

std::uint8_t Version::getPreReleaseNumber() { return prerelease_number; }

const static Version yaodaq_version;

}  // namespace yaodaq
