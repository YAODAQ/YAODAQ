include(Doctest)

set(YAODAQ_INSTALL_TESTDIR "${CMAKE_INSTALL_BINDIR}/tests")

add_executable(Version.test Version.test.cpp)
target_link_libraries(Version.test PRIVATE YAODAQ::Version doctest_with_main)
doctest_discover_tests(Version.test)

add_executable(YaodaqInfos.test YaodaqInfos.test.cpp)
target_link_libraries(YaodaqInfos.test PRIVATE YAODAQ::YaodaqInfos doctest_with_main)
doctest_discover_tests(YaodaqInfos.test)

add_executable(Exception.test Exception.test.cpp)
target_link_libraries(Exception.test PRIVATE YAODAQ::Exception doctest_with_main)
doctest_discover_tests(Exception.test)

add_executable(Identifier.test Identifier.test.cpp)
target_link_libraries(Identifier.test PRIVATE YAODAQ::Identifier doctest_with_main)
doctest_discover_tests(Identifier.test)

add_executable(LoggerHandler.test LoggerHandler.test.cpp)
target_link_libraries(LoggerHandler.test PRIVATE YAODAQ::LoggerHandler doctest_with_main)
doctest_discover_tests(LoggerHandler.test)

add_executable(WebsocketServer.test WebsocketServer.test.cpp)
target_link_libraries(WebsocketServer.test PRIVATE YAODAQ::WebsocketServer doctest_with_main)
doctest_discover_tests(WebsocketServer.test)

add_executable(WebsocketClient.test WebsocketClient.test.cpp)
target_link_libraries(WebsocketClient.test PRIVATE YAODAQ::WebsocketClient doctest_with_main)
doctest_discover_tests(WebsocketClient.test)

add_executable(WebsocketClientsServers.test WebsocketClientsServers.test.cpp)
target_link_libraries(WebsocketClientsServers.test PRIVATE YAODAQ::WebsocketClient YAODAQ::WebsocketServer doctest_with_main)
doctest_discover_tests(WebsocketClientsServers.test)

add_executable(Message.test Message.test.cpp)
target_link_libraries(Message.test PRIVATE YAODAQ::WebsocketClient YAODAQ::Message doctest_with_main)
doctest_discover_tests(Message.test)

add_executable(IXWebsocketMessage.test IXWebsocketMessage.test.cpp)
target_link_libraries(IXWebsocketMessage.test PRIVATE YAODAQ::IXWebsocketMessage doctest_with_main)
doctest_discover_tests(IXWebsocketMessage.test)

install(TARGETS Version.test YaodaqInfos.test Exception.test Identifier.test LoggerHandler.test WebsocketServer.test WebsocketClient.test WebsocketClientsServers.test Message.test
        RUNTIME DESTINATION "${YAODAQ_INSTALL_TESTDIR}")
