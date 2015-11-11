set (GLGUI_RENDERER_GL3_SOURCES GL3Renderer.cpp oglp.cpp GL3BoxRenderer.cpp GL3BoxRenderer.h GL3Renderable.h
        LoadProgram.cpp LoadProgram.h)
add_library (glgui_renderer_gl3 STATIC ${GLGUI_RENDERER_GL3_SOURCES})

target_link_libraries (glgui_renderer_gl3 glyphy glgui lz4)
