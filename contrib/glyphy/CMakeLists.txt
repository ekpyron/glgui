find_package (Freetype REQUIRED)
find_package (glutil REQUIRED)

set (GLYPHY_SOURCES glyphy-arc.cc glyphy-arcs.cc glyphy-blob.cc glyphy-extents.cc glyphy-outline.cc glyphy-sdf.cc)

file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders/glyphy/)

glutil_add_shader ("glyphy;sdf" glyphy-sdf.glsl ${CMAKE_CURRENT_BINARY_DIR}/glyphy-sdf-shader.cpp -I ${CMAKE_CURRENT_SOURCE_DIR})

add_library (glyphy STATIC EXCLUDE_FROM_ALL ${GLYPHY_SOURCES} ${GLYPHY_SHADER_SOURCE}
        ${CMAKE_CURRENT_BINARY_DIR}/glyphy-sdf-shader.cpp)

target_include_directories (glyphy PUBLIC .)
target_include_directories (glyphy SYSTEM PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_include_directories (glyphy PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries (glyphy ${FREETYPE_LIBRARIES})

set_property (TARGET glyphy PROPERTY POSITION_INDEPENDENT_CODE True)
