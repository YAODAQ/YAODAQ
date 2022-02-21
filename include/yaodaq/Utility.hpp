#ifndef YAODAQ_UTILITY
#define YAODAQ_UTILITY

/**
\copyright Copyright 2022 flagarde
*/

#include <string>

namespace yaodaq
{

static inline void ReplaceStringInPlace(std::string& subject,const std::string& search,const std::string& replace)
{
  std::size_t pos{0};
  while((pos = subject.find(search, pos)) != std::string::npos)
  {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

static inline std::string to_fmt(const std::string& subject)
{
  std::string ret{subject};
  ReplaceStringInPlace(ret,"{","{{");
  ReplaceStringInPlace(ret,"}","}}");
  return ret;
}

}  // namespace yaodaq

#endif
