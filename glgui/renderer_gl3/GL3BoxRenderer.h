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

#ifndef GLGUI_GL3BOXRENDERER_H
#define GLGUI_GL3BOXRENDERER_H

#include "GL3Renderable.h"
#include <oglp/oglp.h>

namespace glgui {

class GL3Box : public GL3Renderable {
public:
    GL3Box (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr);
    virtual ~GL3Box (void);

    virtual void Render (GL3Renderer *renderer) const;
    virtual void Select (const glm::mat4 &pickmatrix, GL3Renderer *renderer) const;
    virtual float GetDepth (void) const;
private:
    glm::mat4 transformation;
    glm::vec4 color;
    void *ptr;
};

class GL3BoxRenderer {
public:
    GL3BoxRenderer (void);
    ~GL3BoxRenderer (void);
    void RenderBox (const glm::mat4 &transformation, const glm::vec4 &color);
    void SelectBox (const glm::mat4 &pickmatrix, const glm::mat4 &transformation, void *ptr);
private:
    gl::VertexArray vao;
    gl::Program program;
    gl::Program selectionprogram;
    gl::SmartUniform<glm::mat4> transform_uniform;
    gl::SmartUniform<glm::vec4> color_uniform;
    gl::SmartUniform<glm::mat4> selection_transform_uniform;
    gl::SmartUniform<glm::uvec2> id_uniform;
};

} /* namespace glgui */

#endif /* !defined GLGUI_GL3BOXRENDERER_H */
