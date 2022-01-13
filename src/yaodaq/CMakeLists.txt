include(MagicEnum)

add_library(Identifier Identifier.cpp)
target_include_directories(Identifier PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Identifier PROPERTIES PUBLIC_HEADER "${YAODAQ_INCLUDES_DIR}/Identifier.hpp;${YAODAQ_INCLUDES_DIR}/Class.hpp")
target_link_libraries(Identifier PRIVATE magic_enum::magic_enum)
add_library(YAODAQ::Identifier ALIAS Identifier)

# Install
install(TARGETS Identifier
        EXPORT YAODAQTargets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
