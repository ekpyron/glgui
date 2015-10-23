find_package (oglp REQUIRED)
find_package (Freetype REQUIRED)
find_package (HarfBuzz REQUIRED)
find_package (glutil REQUIRED)

add_subdirectory (renderer_gl3)

set (GLGUI_SOURCES main.cpp Widget.cpp Widget.h Window.cpp Window.h Box.cpp Box.h Renderer.cpp Renderer.h Text.cpp Text.h Font.cpp Font.h Cube.cpp Cube.h)
add_library (glgui STATIC ${GLGUI_SOURCES})

target_link_libraries (glgui glgui_renderer_gl3 ${FREETYPE_LIBRARIES} ${HARFBUZZ_LIBRARIES} ${GLUTIL_LIBRARIES})
target_include_directories (glgui SYSTEM PRIVATE ${OGLP_INCLUDE_DIRS} ${GLUTIL_INCLUDE_DIRS})
target_include_directories (glgui SYSTEM PUBLIC ${FREETYPE_INCLUDE_DIRS} ${HARFBUZZ_INCLUDE_DIRS})
