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

#ifndef GLGUI_GL3RENDERABLE_H
#define GLGUI_GL3RENDERABLE_H

#include <memory>
#include <glm/glm.hpp>

namespace glgui {

class GL3Renderer;

class GL3Renderable {
public:
    virtual ~GL3Renderable (void) {}
    virtual void Render (GL3Renderer *renderer) const = 0;
    virtual void Select (const glm::mat4 &pickmatrix, GL3Renderer *renderer) const = 0;
    virtual float GetDepth (void) const = 0;

    struct DepthCompare {
        bool operator () (const std::unique_ptr<GL3Renderable> &lhs, const std::unique_ptr<GL3Renderable> &rhs) {
            return lhs->GetDepth () < rhs->GetDepth ();
        }
    };
};

} /* namespace glgui */

#endif /* !defined GLGUI_GL3RENDERABLE_H */
