find_package (oglp REQUIRED)
find_package (Freetype REQUIRED)
find_package (HarfBuzz REQUIRED)
find_package (glutil REQUIRED)

add_subdirectory (renderer_gl3)

file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders/font/)
glutil_add_shader ("glgui;shader;font;fragment" ${CMAKE_CURRENT_SOURCE_DIR}/shaders/font/fragment.glsl ${CMAKE_CURRENT_BINARY_DIR}/shaders/font/fragment.cpp)
glutil_add_shader ("glgui;shader;font;vertex" ${CMAKE_CURRENT_SOURCE_DIR}/shaders/font/vertex.glsl ${CMAKE_CURRENT_BINARY_DIR}/shaders/font/vertex.cpp)


set (GLGUI_SOURCES main.cpp Widget.cpp Widget.h Window.cpp Window.h Box.cpp Box.h Renderer.cpp Renderer.h Text.cpp
        Text.h Font.cpp Font.h Cube.cpp Cube.h Atlas.cpp Atlas.h Glyph.cpp Glyph.h TextRenderer.cpp TextRenderer.h
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/font/fragment.cpp ${CMAKE_CURRENT_BINARY_DIR}/shaders/font/vertex.cpp)
add_library (glgui STATIC ${GLGUI_SOURCES})

message ("Lib: ${GLUTIL_LIBRARIES}")

target_link_libraries (glgui glgui_renderer_gl3 ${FREETYPE_LIBRARIES} ${HARFBUZZ_LIBRARIES} ${GLUTIL_LIBRARIES})
target_include_directories (glgui SYSTEM PRIVATE ${OGLP_INCLUDE_DIRS} ${GLUTIL_INCLUDE_DIRS})
target_include_directories (glgui SYSTEM PUBLIC ${FREETYPE_INCLUDE_DIRS} ${HARFBUZZ_INCLUDE_DIRS})
