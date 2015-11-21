set (GLGUI_RENDERER_GL3_SOURCES GL3Renderer.cpp GL3BoxRenderer.cpp GL3BoxRenderer.h GL3Renderable.h)
add_library (glgui_renderer_gl3 OBJECT ${GLGUI_RENDERER_GL3_SOURCES})
set_target_properties (glgui_renderer_gl3 PROPERTIES POSITION_INDEPENDENT_CODE True)