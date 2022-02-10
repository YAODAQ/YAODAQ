include(MagicEnum)
include(IXWebSocket)
include(Spdlog)
include(Nlohmann)
include(SourceLocation)
include(Semver)

semverfile(INPUT "${CMAKE_CURRENT_SOURCE_DIR}/YaodaqVersion.hpp.in" OUTPUT "${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqVersion.hpp")
add_library(Version Version.cpp)
target_include_directories(Version PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> PRIVATE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
set_target_properties(Version PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Version.hpp;${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqVersion.hpp")
target_link_libraries(Version PUBLIC semver::semver PRIVATE magic_enum::magic_enum)
add_library(YAODAQ::Version ALIAS Version)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/YaodaqInfos.hpp.in" "${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqInfos.hpp" @ONLY)
add_library(YaodaqInfos INTERFACE)
target_include_directories(Version INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
set_target_properties(YaodaqInfos PROPERTIES PUBLIC_HEADER "${CMAKE_BINARY_DIR}/include/yaodaq/YaodaqInfos.hpp")
target_link_libraries(YaodaqInfos INTERFACE fmt::fmt YAODAQ::Version)
add_library(YAODAQ::YaodaqInfos ALIAS YaodaqInfos)

add_library(Looper Looper.cpp)
target_include_directories(Looper PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Looper PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Looper.hpp")
target_link_libraries(Looper PRIVATE Interrupt magic_enum::magic_enum fmt::fmt)
add_library(YAODAQ::Looper ALIAS Looper)

add_library(Interrupt Interrupt.cpp)
target_include_directories(Interrupt PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Interrupt PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Interrupt.hpp;${YAODAQ_INCLUDES_DIR}/Signal.hpp;${YAODAQ_INCLUDES_DIR}/Severity.hpp")
add_library(YAODAQ::Interrupt ALIAS Interrupt)

add_library(Exception Exception.cpp)
target_include_directories(Exception PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Exception PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Exception.hpp;${YAODAQ_INCLUDES_DIR}/StatusCode.hpp")
target_link_libraries(Exception PUBLIC fmt::fmt source_location::source_location)
add_library(YAODAQ::Exception ALIAS Exception)

add_library(LoggerHandler LoggerHandler.cpp)
target_include_directories(LoggerHandler PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(LoggerHandler PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/LoggerHandler.hpp")
target_link_libraries(LoggerHandler PUBLIC spdlog::spdlog)
add_library(YAODAQ::LoggerHandler ALIAS LoggerHandler)

add_library(Identifier Identifier.cpp)
target_include_directories(Identifier PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Identifier PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Identifier.hpp;${YAODAQ_INCLUDES_DIR}/Class.hpp")
target_link_libraries(Identifier PRIVATE magic_enum::magic_enum fmt::fmt)
add_library(YAODAQ::Identifier ALIAS Identifier)

add_library(WebsocketServer WebsocketServer.cpp)
target_include_directories(WebsocketServer PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(WebsocketServer PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/WebsocketServer.hpp")
target_link_libraries(WebsocketServer PRIVATE Identifier LoggerHandler Looper PUBLIC Exception ixwebsocket::ixwebsocket spdlog::spdlog magic_enum::magic_enum YAODAQ::Version)
add_library(YAODAQ::WebsocketServer ALIAS WebsocketServer)

add_library(WebsocketClient WebsocketClient.cpp)
target_include_directories(WebsocketClient PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(WebsocketClient PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/WebsocketClient.hpp")
target_link_libraries(WebsocketClient PRIVATE Identifier LoggerHandler Exception Looper PUBLIC ixwebsocket::ixwebsocket spdlog::spdlog magic_enum::magic_enum YAODAQ::Version)
add_library(YAODAQ::WebsocketClient ALIAS WebsocketClient)

# Install
install(TARGETS Identifier WebsocketServer WebsocketClient LoggerHandler spdlog Exception Interrupt Looper Version YaodaqInfos
        EXPORT YAODAQTargets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yaodaq/"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yaodaq/")
