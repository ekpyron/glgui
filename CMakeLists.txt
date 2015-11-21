cmake_minimum_required(VERSION 3.3)
project(glgui)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

include_directories (${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

configure_file (glgui-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/glgui-config.cmake @ONLY)

add_subdirectory (contrib)
add_subdirectory (glgui)
add_subdirectory (test)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/glgui-config.cmake DESTINATION lib/cmake/glgui)
install (DIRECTORY glgui DESTINATION include FILES_MATCHING PATTERN "*.h")
install (EXPORT glgui DESTINATION lib/cmake/glgui)
