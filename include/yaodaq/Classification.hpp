#ifndef YAODAQ_CLASSIFICATION
#define YAODAQ_CLASSIFICATION

/**
\copyright Copyright 2022 flagarde
*/

#include <cstdint>

namespace yaodaq
{

/* The domain specify if we are on browser or standalone program */
enum class Domain : std::uint_least8_t
{
  Unknown     = 0,
  Application = 1,
  Web         = 2,
};

/* The class define if we are a server, module, or board */
enum class Class : std::uint_least8_t
{
  Unknown = 0,
  Server,
  Client,
  // Module is a client with start stop etc...
  Module,
  // Board is a module with a connector
  Board,
};

/* the family */
enum class Family : std::uint_least16_t
{
  Unknown = 0,
  WebSocketServer,
  WebSocketClient,
  Logger,
  Controller,
  Configurator,
  SlowController,
  Viewer,
  Analyser,
  FileWriter,
};

}  // namespace yaodaq

#endif  // YAODAQ_CLASSIFICATION
