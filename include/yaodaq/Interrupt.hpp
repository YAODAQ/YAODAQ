#ifndef YAODAQ_HANDLER_HPP
#define YAODAQ_HANDLER_HPP

/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Signal.hpp"

#include <atomic>
#include <csignal>
#include <mutex>

namespace yaodaq
{

enum class Signal;

class Interrupt
{
public:
  Interrupt();
  void   init();
  void   restore();
  Signal getSignal();
  ~Interrupt();

private:
  volatile static std::atomic<Signal> m_Signal;
  void                                setSignal( const Signal& signal );
  std::mutex                          m_mutex;
};

}  // namespace yaodaq

#endif  // YAODAQ_HANDLER_HPP
