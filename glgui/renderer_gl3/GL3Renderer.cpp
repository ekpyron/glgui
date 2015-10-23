/*
 * Copyright 2015 Daniel Kirchner
 *
 * This file is part of GLGUI.
 *
 * GLGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLGUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GL3Renderer.h"
#include <oglp/oglp.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glgui/Widget.h>

namespace glgui {

GL3Renderer::GL3Renderer (void) : selectiontexture (GL_TEXTURE_2D) {
    selectiontexture.Storage2D (1, GL_RG32UI, 1, 1);
    selectionframebuffer.Texture (GL_COLOR_ATTACHMENT0, selectiontexture, 0);
    selectionframebuffer.DrawBuffers ({ GL_COLOR_ATTACHMENT0 });
}

GL3Renderer::~GL3Renderer (void) {
}

void GL3Renderer::ResizeViewport (unsigned int _width, unsigned int _height, unsigned int depth) {
    width = _width;
    height = _height;

}

void GL3Renderer::Reset (void) {
    renderables.clear ();
}

void GL3Renderer::RenderBox (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr) {
    renderables.emplace (new GL3Box (transformation, color, ptr));
}

void GL3Renderer::RenderCube (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr) {
    renderables.emplace (new GL3Box (transformation, color, ptr));
}

void GL3Renderer::Display (Widget *widget) {
    widget->Paint (this);
    gl::Viewport (0, 0, width, height);
    gl::BlendEquation (GL_FUNC_ADD);
    gl::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::Enable (GL_BLEND);
    for (auto &renderable : renderables) {
        renderable->Render (this);
    }
}

void *GL3Renderer::Select (unsigned int x, unsigned int y) {
    glm::mat4 pickmatrix = glm::pickMatrix (glm::vec2 (x, y), glm::vec2 (1.0f, 1.0f), glm::vec4 (0, 0, width, height));
    selectionframebuffer.Bind (GL_FRAMEBUFFER);
    selectionframebuffer.Clear (GL_COLOR, 0, (const GLuint[]) { 0, 0, 0, 0 });
    gl::Viewport (0, 0, width, height);
    gl::Disable (GL_BLEND);
    for (auto &renderable : renderables) {
        renderable->Select (pickmatrix, this);
    }
    gl::Framebuffer::Unbind (GL_FRAMEBUFFER);
    void *ptr = nullptr;
    selectiontexture.GetTexImage (0, GL_RG_INTEGER, GL_UNSIGNED_INT, sizeof (void*), &ptr);
    return ptr;
}

} /* namespace glgui */
