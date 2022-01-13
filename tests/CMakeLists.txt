include(Doctest)

set(YAODAQ_INSTALL_TESTDIR "${CMAKE_INSTALL_BINDIR}/tests")

add_executable(Identifier.test Identifier.test.cpp)
target_link_libraries(Identifier.test PRIVATE YAODAQ::Identifier doctest_with_main)
doctest_discover_tests(Identifier.test COMMAND IdentifierTest)

install(TARGETS Identifier.test
        RUNTIME DESTINATION "${YAODAQ_INSTALL_TESTDIR}")
