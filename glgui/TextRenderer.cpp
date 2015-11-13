/*
 * Copyright (c) 2015 daniel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "TextRenderer.h"
#include "Text.h"
#include <exception>
#include <glutil/LoadProgram.h>

GLUTIL_IMPORT_SHADER (glgui, shader, font, vertex);
GLUTIL_IMPORT_SHADER (glgui, shader, font, fragment);
GLUTIL_IMPORT_SHADER  (glyphy, sdf)

namespace glgui {

namespace detail {

TextRenderer::TextRenderer (void) :acc (glyphy_arc_accumulator_create ()) {
    glutil::LoadProgram (program, "GLGUI_TEXT", "vec4 glyphy_texture1D_func (in int offset);", {
            { "GLGUI_TEXT_GLYPHY", GL_FRAGMENT_SHADER, glyphy::sdf },
            { "GLGUI_TEXT_FRAGMENT", GL_FRAGMENT_SHADER, shader::font::fragment },
            { "GLGUI_TEXT_VERTEX", GL_VERTEX_SHADER, shader::font::vertex }
    });

    program["sdftex"] = GLint (0);

    gl::UniformBlockBinding (program.get (), gl::GetUniformBlockIndex (program.get (), "Uniforms"), 0);

    contrast = gl::SmartUniform<float> (program["contrast"], 1.0f);
    boldness = gl::SmartUniform<float> (program["boldness"], 0.0f);
    matrix = gl::SmartUniform<glm::mat4> (program["mat"], glm::mat4 (1));

    const GLshort data[] = { 0, 0, 1, 0, 0, 1, 1, 1 };
    buffer.Storage (sizeof (data), data, 0);

    vao.AttribIFormat (0, 2, GL_SHORT, 0);
    vao.AttribBinding (0, 0);
    vao.EnableAttrib (0);
    vao.VertexBuffer (0, buffer, 0, sizeof (GLshort) * 2);

    vao.AttribFormat (1, 4, GL_FLOAT, GL_FALSE, offsetof (charinfo_t, vPositions));
    vao.AttribBinding (1, 1);
    vao.EnableAttrib (1);

    vao.AttribIFormat (2, 3, GL_INT, offsetof (charinfo_t, vInfo));
    vao.AttribBinding (2, 1);
    vao.EnableAttrib (2);

    vao.BindingDivisor (1, 1);
}

TextRenderer::~TextRenderer (void) {

}

} /* namespace detail */

std::atomic_flag TextRenderer::lock = ATOMIC_FLAG_INIT;
int TextRenderer::refcount = 0;
detail::TextRenderer *TextRenderer::renderer = nullptr;

TextRenderer::TextRenderer (void) {
    std::exception_ptr exception;
    while (lock.test_and_set (std::memory_order_acquire));
    if (!refcount) {
        try {
            renderer = new detail::TextRenderer ();
        } catch (...) {
            exception = std::current_exception ();
        }
    }
    refcount++;
    lock.clear (std::memory_order_release);
    if (exception) {
        std::rethrow_exception (exception);
    }
}

TextRenderer::TextRenderer (const TextRenderer &r) {
    while (lock.test_and_set (std::memory_order_acquire));
    refcount++;
    lock.clear (std::memory_order_release);
}

TextRenderer::TextRenderer (TextRenderer &&r) {
    while (lock.test_and_set (std::memory_order_acquire));
    refcount++;
    lock.clear (std::memory_order_release);
}

TextRenderer::~TextRenderer (void) {
    while (lock.test_and_set (std::memory_order_acquire));
    if (refcount) refcount--;
    if (!refcount) {
        delete renderer;
    }
    lock.clear (std::memory_order_release);
}

TextRenderer &TextRenderer::operator= (const TextRenderer &r) {
    while (lock.test_and_set (std::memory_order_acquire));
    refcount++;
    lock.clear (std::memory_order_release);
}

TextRenderer &TextRenderer::operator= (TextRenderer &&r) noexcept {
    while (lock.test_and_set (std::memory_order_acquire));
    refcount++;
    lock.clear (std::memory_order_release);
}

} /* namespace glgui */
