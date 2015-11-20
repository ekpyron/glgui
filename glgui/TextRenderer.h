/*
 * Copyright 2015 Daniel Kirchner
 *
 * This file is part of glgui.
 *
 * glgui is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glgui is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glgui.  If not, see <http://www.gnu.org/licenses/>.
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
