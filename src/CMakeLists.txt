set(YAODAQ_INCLUDES_DIR "${PROJECT_SOURCE_DIR}/include/yaodaq")

add_subdirectory(yaodaq)

include(CMakePackageConfigHelpers)

configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/YAODAQConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/YAODAQConfig.cmake" INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/YAODAQ")
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/YAODAQVersion.cmake" COMPATIBILITY SameMajorVersion)
install(EXPORT YAODAQTargets FILE YAODAQTargets.cmake NAMESPACE YAODAQ:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/YAODAQ")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/YAODAQConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/YAODAQVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/YAODAQ")

export(EXPORT YAODAQTargets FILE "${PROJECT_BINARY_DIR}/cmake/YAODAQConfig.cmake" NAMESPACE YAODAQ::)
