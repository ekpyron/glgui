cmake_minimum_required(VERSION 3.3)
project(glgui)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

include_directories (${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory (contrib)
add_subdirectory (glgui)
add_subdirectory (test)
