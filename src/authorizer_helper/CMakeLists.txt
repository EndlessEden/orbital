pkg_check_modules(WaylandClient wayland-client REQUIRED)

find_package(Qt5Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES main.cpp)

wayland_add_protocol_client(SOURCES ../../protocol/orbital-authorizer-helper.xml authorizer-helper)

list(APPEND defines "LIBEXEC_PATH=\"${CMAKE_INSTALL_PREFIX}/libexec\"")
list(APPEND defines "QT_MESSAGELOGCONTEXT")

add_executable(orbital-authorizer-helper ${SOURCES} ${RESOURCES})
target_link_libraries(orbital-authorizer-helper wayland-client Qt5::Core)
set_target_properties(orbital-authorizer-helper PROPERTIES COMPILE_DEFINITIONS "${defines}")

install(TARGETS orbital-authorizer-helper DESTINATION libexec)
