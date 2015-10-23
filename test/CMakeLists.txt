find_package (glfw3 REQUIRED)

set (GLGUITEST_SOURCES main.cpp)

include_directories (${GLFW3_INCLUDE_DIR})

add_executable (glguitest ${GLGUITEST_SOURCES})

target_link_libraries (glguitest glgui ${GLFW3_LIBRARY})
