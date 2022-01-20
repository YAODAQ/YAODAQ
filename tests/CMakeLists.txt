include(Doctest)

set(YAODAQ_INSTALL_TESTDIR "${CMAKE_INSTALL_BINDIR}/tests")

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

install(TARGETS Identifier.test
        RUNTIME DESTINATION "${YAODAQ_INSTALL_TESTDIR}")
