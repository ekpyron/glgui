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

#ifndef GLGUI_TEXTRENDERER_H
#define GLGUI_TEXTRENDERER_H

#include <atomic>
#include <oglp/oglp.h>
#include <glyphy.h>
#include "Atlas.h"

namespace glgui {
namespace detail {
class TextRenderer {
public:
    TextRenderer (void);
    TextRenderer (const TextRenderer&) = delete;
    ~TextRenderer (void);
    TextRenderer &operator= (const TextRenderer&) = delete;
    gl::VertexArray &GetVertexArray (void) {
        return vao;
    }
    gl::Program &GetProgram (void) {
        return program;
    }
    Atlas &GetAtlas (void) {
        return atlas;
    }
    void SetMatrix (const glm::mat4 &mat) {
        matrix.Set (mat);
    }
    void SetContrast (const float &_contrast) {
        contrast.Set (_contrast);
    }
    void SetBoldness (const float &_boldness) {
        boldness.Set (_boldness);
    }
    glyphy_arc_accumulator_t *GetArcAccumulator (void) {
        return acc;
    }
private:
    gl::Program program;
    gl::SmartUniform<float> contrast;
    gl::SmartUniform<float> boldness;
    gl::SmartUniform<glm::mat4> matrix;
    gl::VertexArray vao;
    gl::Buffer buffer;
    Atlas atlas;
    glyphy_arc_accumulator_t *acc;
};
} /* namespace detail */

class TextRenderer {
public:
    TextRenderer (void);
    TextRenderer (const TextRenderer &r);
    TextRenderer (TextRenderer&&);
    ~TextRenderer (void);
    TextRenderer &operator= (const TextRenderer &r);
    TextRenderer &operator= (TextRenderer &&) noexcept;
    gl::VertexArray &GetVertexArray (void) {
        return renderer->GetVertexArray ();
    }
    gl::Program &GetProgram (void) {
        return renderer->GetProgram ();
    }
    Atlas &GetAtlas (void) {
        return renderer->GetAtlas ();
    }
    void SetMatrix (const glm::mat4 &mat) {
        renderer->SetMatrix (mat);
    }
    void SetContrast (const float &contrast) {
        renderer->SetContrast (contrast);
    }
    void SetBoldness (const float &boldness) {
        renderer->SetBoldness (boldness);
    }
    glyphy_arc_accumulator_t *GetArcAccumulator (void) {
        return renderer->GetArcAccumulator ();
    }
private:
    static std::atomic_flag lock;
    static int refcount;
    static detail::TextRenderer *renderer;
};

} /* namespace glgui */

#endif /* !defined GLGUI_TEXTRENDERER_H */
