#ifndef YAODAQ_LOOPER
#define YAODAQ_LOOPER

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Interrupt.hpp"

#include <mutex>

namespace yaodaq
{

enum class Signal;

class Looper
{
public:
  Looper();
  Signal     loop();
  Signal     getSignal();
  static int getInstance();
  void       supressInstance();
  ~Looper();

private:
  static int       m_instance;
  bool             m_hasBeenAdded{ false };
  bool             m_hasBeenSupressed{ false };
  static Interrupt m_Interrupt;
  static bool      m_Initialized;
};

}  // namespace yaodaq

#endif  // YAODAQ_LOOPER
