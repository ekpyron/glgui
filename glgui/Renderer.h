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

#ifndef GLGUI_RENDERER_H
#define GLGUI_RENDERER_H

#include <glm/glm.hpp>

namespace glgui {

class Widget;

class Renderer {
public:
    virtual ~Renderer (void);
    virtual void ResizeViewport (unsigned int width, unsigned int height, unsigned int depth) = 0;
    virtual void Reset (void) = 0;
    virtual void RenderBox (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr = nullptr) = 0;
    virtual void RenderCube (const glm::mat4 &transformation, const glm::vec4 &color, void *ptr = nullptr) = 0;
    virtual void *Select (unsigned int x, unsigned int y) = 0;
    virtual void Display (Widget *widget) = 0;
};

}

#endif /* !defined GLGUI_RENDERER_H */
