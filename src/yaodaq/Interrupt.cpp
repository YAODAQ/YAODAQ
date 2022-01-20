/**
\copyright Copyright 2022 flagarde
*/

#include "yaodaq/Interrupt.hpp"

#include "yaodaq/Signal.hpp"

#include <atomic>
#include <csignal>
#include <mutex>
#include <thread>

namespace yaodaq
{

volatile std::atomic<Signal> Interrupt::m_Signal = Signal::NO;

Interrupt::Interrupt() { init(); }

void Interrupt::restore()
{
  std::signal( SIGTERM, SIG_DFL );
  std::signal( SIGSEGV, SIG_DFL );
  std::signal( SIGINT, SIG_DFL );
  std::signal( SIGILL, SIG_DFL );
  std::signal( SIGABRT, SIG_DFL );
  std::signal( SIGFPE, SIG_DFL );
}

void Interrupt::init()
{
  setSignal( Signal::TERM );
  setSignal( Signal::TERM );
  setSignal( Signal::SEGV );
  setSignal( Signal::INT );
  setSignal( Signal::ILL );
  setSignal( Signal::ABRT );
  setSignal( Signal::FPE );
}

Interrupt::~Interrupt() { restore(); }

Signal Interrupt::getSignal()
{
  if( m_Signal.load() != Signal::NO )
  {
    std::lock_guard<std::mutex> guard( m_mutex );
    init();
  }
  return m_Signal.load();
}

void Interrupt::setSignal( const Signal& signal )
{
  switch( signal )
  {
    case Signal::ABRT: std::signal( SIGABRT, []( int a ) -> void { m_Signal.store( Signal::ABRT ); } ); break;
    case Signal::FPE: std::signal( SIGFPE, []( int a ) -> void { m_Signal.store( Signal::FPE ); } ); break;
    case Signal::ILL: std::signal( SIGILL, []( int a ) -> void { m_Signal.store( Signal::ILL ); } ); break;
    case Signal::SEGV: std::signal( SIGSEGV, []( int a ) -> void { m_Signal.store( Signal::SEGV ); } ); break;
    case Signal::INT: std::signal( SIGINT, []( int a ) -> void { m_Signal.store( Signal::INT ); } ); break;
    case Signal::TERM: std::signal( SIGTERM, []( int a ) -> void { m_Signal.store( Signal::TERM ); } ); break;
    default: break;
  }
}

}  // namespace yaodaq
