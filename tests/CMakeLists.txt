include(Doctest)

set(YAODAQ_INSTALL_TESTDIR "${CMAKE_INSTALL_BINDIR}/tests")

add_executable(Identifier.test Identifier.test.cpp)
target_link_libraries(Identifier.test PRIVATE YAODAQ::Identifier doctest_with_main)
doctest_discover_tests(Identifier.test)

add_executable(LoggerHandler.test LoggerHandler.test.cpp)
target_link_libraries(LoggerHandler.test PRIVATE YAODAQ::LoggerHandler doctest_with_main)
doctest_discover_tests(LoggerHandler.test)

install(TARGETS Identifier.test
        RUNTIME DESTINATION "${YAODAQ_INSTALL_TESTDIR}")
