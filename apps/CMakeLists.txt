include(CLI11)

#if(BUILD_WEBSOCKETSERVER)
add_executable(WebsocketServerApp WebsocketServer.app.cpp)
target_link_libraries(WebsocketServerApp PRIVATE YAODAQ::WebsocketServer CLI11::CLI11)
set_target_properties(WebsocketServerApp PROPERTIES OUTPUT_NAME WebsocketServer)
install(TARGETS WebsocketServerApp)
#endif()
