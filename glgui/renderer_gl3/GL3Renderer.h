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

#ifndef GLGUI_GL3RENDERER_H
#define GLGUI_GL3RENDERER_H

#include <set>
#include <glgui/Renderer.h>
#include "GL3BoxRenderer.h"
#include "GL3Renderable.h"

namespace glgui {

class GL3Renderer : public Renderer {
public:
    GL3Renderer (void);
    ~GL3Renderer (void);
    virtual void ResizeViewport (unsigned int width, unsigned int height, unsigned int depth);
    virtual void Reset (void);
    virtual void RenderBox (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr);
    virtual void RenderCube (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr);
    virtual void Display (Widget *widget);
    virtual void *Select (unsigned int x, unsigned int y);
    GL3BoxRenderer &GetBoxRenderer (void) {
        return boxrenderer;
    }
private:
    gl::Framebuffer selectionframebuffer;
    gl::Texture selectiontexture;
    GL3BoxRenderer boxrenderer;
    std::multiset<std::unique_ptr<GL3Renderable>, GL3Renderable::DepthCompare> renderables;
    unsigned int width;
    unsigned int height;
};

} /* namespace glgui */


#endif /* !defined GLGUI_GL3RENDERER_H */
