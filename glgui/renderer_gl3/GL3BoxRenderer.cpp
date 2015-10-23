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

#include "GL3BoxRenderer.h"
#include "GL3Renderer.h"

namespace glgui {

GL3Box::GL3Box (const glm::mat4 &_transformation, const glm::vec4 &_color, void *_ptr)
        : transformation (_transformation), color (_color), ptr (_ptr) {
}

GL3Box::~GL3Box (void) {
}

void GL3Box::Render (GL3Renderer *renderer) const {
    renderer->GetBoxRenderer ().RenderBox (transformation, color);
}

void GL3Box::Select (const glm::mat4 &pickmatrix, GL3Renderer *renderer) const {
    renderer->GetBoxRenderer ().SelectBox (pickmatrix, transformation, ptr);
}

float GL3Box::GetDepth (void) const {
    glm::vec4 corners[8] = {
            glm::vec4 (-1.0f, -1.0f, -1.0f, 1.0f),
            glm::vec4 (-1.0f, -1.0f,  1.0f, 1.0f),
            glm::vec4 (-1.0f,  1.0f, -1.0f, 1.0f),
            glm::vec4 (-1.0f,  1.0f,  1.0f, 1.0f),
            glm::vec4 ( 1.0f, -1.0f, -1.0f, 1.0f),
            glm::vec4 ( 1.0f, -1.0f,  1.0f, 1.0f),
            glm::vec4 ( 1.0f,  1.0f, -1.0f, 1.0f),
            glm::vec4 ( 1.0f,  1.0f,  1.0f, 1.0f)
    };
    glm::vec4 center;
    for (auto i = 0; i < 8; i++) center += 0.125f * transformation * corners[i];
    return center.z;
}


GL3BoxRenderer::GL3BoxRenderer (void) {
    {
        gl::Buffer vertices;
        GLshort vertexdata[] = {
                -32767, -32767,
                32767, -32767,
                -32767, 32767,
                32767, 32767
        };
        vertices.Data (sizeof (vertexdata), vertexdata, GL_STATIC_DRAW);
        vao.AttribFormat (0, 2, GL_SHORT, GL_TRUE, 0);
        vao.EnableAttrib (0);
        vao.AttribBinding (0, 0);
        vao.VertexBuffer (0, vertices, 0, sizeof (GLshort) * 2);
    }
    {
        gl::Shader vshader (GL_VERTEX_SHADER);
        vshader.Source ("#version 330 core\n"
                                "\n"
                                "layout (location = 0) in vec2 position;\n"
                                "uniform mat4 transform;\n"
                                "\n"
                                "void main (void)\n"
                                "{\n"
                                "  gl_Position = transform * vec4 (position, 0, 1);"
                                "}\n");
        if (!vshader.Compile ()) {
            throw std::runtime_error (vshader.GetInfoLog ());
        }

        gl::Shader fshader (GL_FRAGMENT_SHADER);
        fshader.Source ("#version 330 core\n"
                                "\n"
                                "layout (location = 0) out vec4 outcolor;\n"
                                "uniform vec4 color;\n"
                                "\n"
                                "void main (void)\n"
                                "{\n"
                                "  outcolor = color;\n"
                                "}\n");
        if (!fshader.Compile ()) {
            throw std::runtime_error (fshader.GetInfoLog ());
        }

        gl::Shader selectionfshader (GL_FRAGMENT_SHADER);
        selectionfshader.Source ("#version 330 core\n"
                                         "\n"
                                         "layout (location = 0) out uvec2 outid;\n"
                                         "uniform uvec2 id;\n"
                                         "\n"
                                         "void main (void)\n"
                                         "{\n"
                                         "  outid = id;\n"
                                         "}\n");
        if (!selectionfshader.Compile ()) {
            throw std::runtime_error (selectionfshader.GetInfoLog ());
        }

        program.Attach (vshader);
        program.Attach (fshader);
        if (!program.Link ()) {
            throw std::runtime_error (program.GetInfoLog ());
        }
        program.Detach (fshader);
        program.Detach (vshader);

        selectionprogram.Attach (vshader);
        selectionprogram.Attach (selectionfshader);
        if (!selectionprogram.Link ()) {
            throw std::runtime_error (selectionprogram.GetInfoLog ());
        }
        selectionprogram.Detach (selectionfshader);
        selectionprogram.Detach (vshader);
    }
    transform_uniform = gl::SmartUniform<glm::mat4> (program["transform"], glm::mat4 (1.0f));
    color_uniform = gl::SmartUniform<glm::vec4> (program["color"], glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f));
    selection_transform_uniform = gl::SmartUniform<glm::mat4> (selectionprogram["transform"], glm::mat4 (1.0f));
    id_uniform = gl::SmartUniform<glm::uvec2> (selectionprogram["id"], glm::uvec2 (0, 0));
}

GL3BoxRenderer::~GL3BoxRenderer (void) {
}

void GL3BoxRenderer::SelectBox (const glm::mat4 &pickmatrix, const glm::mat4 &transformation, void *ptr) {
    selectionprogram.Use ();
    vao.Bind ();
    selection_transform_uniform.Set (pickmatrix * transformation);
    id_uniform.Set (glm::uvec2 (intptr_t (ptr) & 0xFFFFFFFF, (intptr_t (ptr) >> 32) & 0xFFFFFFFF));
    gl::DrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

void GL3BoxRenderer::RenderBox (const glm::mat4 &transformation, const glm::vec4 &color) {
    program.Use ();
    vao.Bind ();
    transform_uniform.Set (transformation);
    color_uniform.Set (color);
    gl::DrawArrays (GL_TRIANGLE_STRIP, 0, 4);
}

} /* namespace glgui */
