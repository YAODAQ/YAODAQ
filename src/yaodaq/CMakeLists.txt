include(MagicEnum)
include(IXWebSocket)
include(Spdlog)
include(Nlohmann)
include(SourceLocation)
include(Semver)

add_library(Headers INTERFACE)
target_include_directories(Headers INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
add_library(YAODAQ::Headers ALIAS Headers)

add_library(Exception Exception.cpp)
target_include_directories(Exception PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Exception PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Exception.hpp;${YAODAQ_INCLUDES_DIR}/StatusCode.hpp")
target_link_libraries(Exception PUBLIC fmt::fmt source_location::source_location)
add_library(YAODAQ::Exception ALIAS Exception)

semverfile(INPUT "${CMAKE_CURRENT_SOURCE_DIR}/YaodaqVersion.hpp.in" OUTPUT "${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqVersion.hpp")
add_library(Version Version.cpp)
target_include_directories(Version PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
set_target_properties(Version PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Version.hpp;${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqVersion.hpp")
target_link_libraries(Version PUBLIC YAODAQ::Headers semver::semver PRIVATE magic_enum::magic_enum)
add_library(YAODAQ::Version ALIAS Version)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/YaodaqInfos.hpp.in" "${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqInfos.hpp" @ONLY)
add_library(YaodaqInfos INTERFACE)
target_include_directories(YaodaqInfos INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
set_target_properties(YaodaqInfos PROPERTIES PUBLIC_HEADER "${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqInfos.hpp")
target_link_libraries(YaodaqInfos INTERFACE fmt::fmt YAODAQ::Version)
add_library(YAODAQ::YaodaqInfos ALIAS YaodaqInfos)

add_library(Key Key.cpp)
set_target_properties(Key PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Classification.hpp;${YAODAQ_INCLUDES_DIR}/Key.hpp")
target_link_libraries(Key PUBLIC YAODAQ::Headers)
add_library(YAODAQ::Key ALIAS Key)

add_library(Identifier Identifier.cpp)
target_include_directories(Identifier PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Identifier PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Identifier.hpp;${YAODAQ_INCLUDES_DIR}/Classification.hpp")
target_link_libraries(Identifier PRIVATE magic_enum::magic_enum fmt::fmt source_location::source_location YAODAQ::Key YAODAQ::Exception)
add_library(YAODAQ::Identifier ALIAS Identifier)

add_library(Message Message.cpp)
set_target_properties(Message PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Message.hpp;${YAODAQ_INCLUDES_DIR}/MessageType.hpp")
target_link_libraries(Message PUBLIC YAODAQ::Headers nlohmann_json::nlohmann_json YAODAQ::Version PRIVATE magic_enum::magic_enum fmt::fmt YAODAQ::Identifier ixwebsocket::ixwebsocket)
add_library(YAODAQ::Message ALIAS Message)

add_library(IXWebsocketMessage IXWebsocketMessage.cpp)
set_target_properties(IXWebsocketMessage PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/IXWebsocketMessage.hpp")
target_link_libraries(IXWebsocketMessage PUBLIC YAODAQ::Message ixwebsocket::ixwebsocket)
add_library(YAODAQ::IXWebsocketMessage ALIAS IXWebsocketMessage)

add_library(ConnectionState ConnectionState.cpp)
set_target_properties(ConnectionState PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/ConnectionState.hpp")
target_link_libraries(ConnectionState PUBLIC YAODAQ::Headers ixwebsocket::ixwebsocket YAODAQ::Identifier)
add_library(YAODAQ::ConnectionState ALIAS ConnectionState)

add_library(Looper Looper.cpp)
set_target_properties(Looper PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Looper.hpp")
target_link_libraries(Looper PRIVATE Interrupt magic_enum::magic_enum fmt::fmt)
add_library(YAODAQ::Looper ALIAS Looper)

add_library(Interrupt Interrupt.cpp)
set_target_properties(Interrupt PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Interrupt.hpp;${YAODAQ_INCLUDES_DIR}/Signal.hpp;${YAODAQ_INCLUDES_DIR}/Severity.hpp")
target_link_libraries(Interrupt PUBLIC YAODAQ::Headers)
add_library(YAODAQ::Interrupt ALIAS Interrupt)

add_library(LoggerHandler LoggerHandler.cpp)
set_target_properties(LoggerHandler PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/LoggerHandler.hpp")
target_link_libraries(LoggerHandler PUBLIC spdlog::spdlog YAODAQ::Headers)
add_library(YAODAQ::LoggerHandler ALIAS LoggerHandler)

add_library(WebsocketServer WebsocketServer.cpp)
set_target_properties(WebsocketServer PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/WebsocketServer.hpp")
target_link_libraries(WebsocketServer PRIVATE Identifier LoggerHandler Looper YAODAQ::ConnectionState PUBLIC YAODAQ::Exception ixwebsocket::ixwebsocket spdlog::spdlog magic_enum::magic_enum YAODAQ::IXWebsocketMessage)
add_library(YAODAQ::WebsocketServer ALIAS WebsocketServer)

add_library(WebsocketClient WebsocketClient.cpp)
set_target_properties(WebsocketClient PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/WebsocketClient.hpp")
target_link_libraries(WebsocketClient PRIVATE Identifier LoggerHandler YAODAQ::Exception Looper PUBLIC ixwebsocket::ixwebsocket spdlog::spdlog magic_enum::magic_enum YAODAQ::IXWebsocketMessage)
add_library(YAODAQ::WebsocketClient ALIAS WebsocketClient)

# Install
install(TARGETS Headers Key Identifier WebsocketServer WebsocketClient LoggerHandler spdlog Exception Interrupt Looper Version YaodaqInfos IXWebsocketMessage Message ConnectionState
        EXPORT YAODAQTargets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yaodaq/"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yaodaq/")
