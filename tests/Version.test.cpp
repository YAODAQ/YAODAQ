/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Version.hpp"

#include "doctest/doctest.h"

TEST_CASE( "Version" )
{
  yaodaq::Version version_string( "1.2.3-rc.4" );
  CHECK_EQ( version_string.getMajor(), 1 );
  CHECK_EQ( version_string.getMinor(), 2 );
  CHECK_EQ( version_string.getPatch(), 3 );
  CHECK_EQ( version_string.getPreRelease(), "rc" );
  CHECK_EQ( version_string.getPreReleaseNumber(), 4 );

  yaodaq::Version version( 6, 7, 8, semver::prerelease::alpha, 10 );
  CHECK_EQ( version.getMajor(), 6 );
  CHECK_EQ( version.getMinor(), 7 );
  CHECK_EQ( version.getPatch(), 8 );
  CHECK_EQ( version.getPreRelease(), "alpha" );
  CHECK_EQ( version.getPreReleaseNumber(), 10 );
}
